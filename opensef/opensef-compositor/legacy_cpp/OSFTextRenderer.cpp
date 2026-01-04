/**
 * OSFTextRenderer.cpp - Text Rendering Implementation
 *
 * Implements text rendering using FreeType and wlr_buffer.
 */

#define _GNU_SOURCE
#include "OSFTextRenderer.h"
#include "OSFAresTheme.h"

#include <cstring>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftbitmap.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

// Try to include drm_fourcc.h, fallback if missing
#if __has_include(<drm_fourcc.h>)
#include <drm_fourcc.h>
#elif __has_include(<libdrm/drm_fourcc.h>)
#include <libdrm/drm_fourcc.h>
#else
// Fallback definition for ARGB8888 (Little Endian -> B G R A in memory)
#ifndef DRM_FORMAT_ARGB8888
#define DRM_FORMAT_ARGB8888 0x34325241
#endif
#endif

namespace opensef {

namespace {

// ----------------------------------------------------------------------------
// OSFMemoryBuffer: wlr_buffer implementation backed by memfd/mmap
// ----------------------------------------------------------------------------

struct OSFMemoryBuffer {
  struct wlr_buffer base;
  int fd;
  void *data;
  size_t size;
  int stride;
  uint32_t format;
};

static void buffer_destroy(struct wlr_buffer *wlr_buf) {
  OSFMemoryBuffer *buffer = (OSFMemoryBuffer *)wlr_buf;
  if (buffer->data != MAP_FAILED) {
    munmap(buffer->data, buffer->size);
  }
  if (buffer->fd >= 0) {
    close(buffer->fd);
  }
  delete buffer;
}

static bool buffer_get_dmabuf(struct wlr_buffer *wlr_buf,
                              struct wlr_dmabuf_attributes *attribs) {
  (void)wlr_buf;
  (void)attribs;
  return false; // Not a dmabuf
}

static bool buffer_get_shm(struct wlr_buffer *wlr_buf,
                           struct wlr_shm_attributes *attribs) {
  OSFMemoryBuffer *buffer = (OSFMemoryBuffer *)wlr_buf;
  attribs->fd = buffer->fd;
  attribs->format = buffer->format;
  attribs->width = buffer->base.width;
  attribs->height = buffer->base.height;
  attribs->stride = buffer->stride;
  attribs->offset = 0;
  return true;
}

static bool buffer_begin_data_ptr_access(struct wlr_buffer *wlr_buf,
                                         uint32_t flags, void **data,
                                         uint32_t *format, size_t *stride) {
  (void)flags;
  OSFMemoryBuffer *buffer = (OSFMemoryBuffer *)wlr_buf;
  if (data) *data = buffer->data;
  if (format) *format = buffer->format;
  if (stride) *stride = buffer->stride;
  return true;
}

static void buffer_end_data_ptr_access(struct wlr_buffer *wlr_buf) {
  (void)wlr_buf;
  // No-op
}

static const struct wlr_buffer_impl buffer_impl = {
    .destroy = buffer_destroy,
    .get_dmabuf = buffer_get_dmabuf,
    .get_shm = buffer_get_shm,
    .begin_data_ptr_access = buffer_begin_data_ptr_access,
    .end_data_ptr_access = buffer_end_data_ptr_access,
};

static struct wlr_buffer *create_memory_buffer(int width, int height) {
  if (width <= 0 || height <= 0) return nullptr;

  OSFMemoryBuffer *buffer = new OSFMemoryBuffer();
  // Safe initialization
  buffer->fd = -1;
  buffer->data = MAP_FAILED;
  buffer->size = 0;

  wlr_buffer_init(&buffer->base, &buffer_impl, width, height);

  buffer->format = DRM_FORMAT_ARGB8888;
  buffer->stride = width * 4;
  buffer->size = buffer->stride * height;

  // Create anonymous file
  buffer->fd = memfd_create("osf-text-buffer", MFD_CLOEXEC);
  if (buffer->fd < 0) {
    delete buffer;
    return nullptr;
  }

  // Set size
  if (ftruncate(buffer->fd, buffer->size) < 0) {
    close(buffer->fd);
    delete buffer;
    return nullptr;
  }

  // Map memory
  buffer->data = mmap(nullptr, buffer->size, PROT_READ | PROT_WRITE, MAP_SHARED, buffer->fd, 0);
  if (buffer->data == MAP_FAILED) {
    close(buffer->fd);
    delete buffer;
    return nullptr;
  }

  // Initialize to clear (transparent)
  memset(buffer->data, 0, buffer->size);

  return &buffer->base;
}

} // namespace

// ============================================================================
// OSFTextRenderer Implementation
// ============================================================================

OSFTextRenderer &OSFTextRenderer::instance() {
  static OSFTextRenderer instance;
  return instance;
}

OSFTextRenderer::~OSFTextRenderer() {
  if (ftFace_) {
    FT_Done_Face(static_cast<FT_Face>(ftFace_));
  }
  if (ftLibrary_) {
    FT_Done_FreeType(static_cast<FT_Library>(ftLibrary_));
  }
}

bool OSFTextRenderer::initialize(const std::string &fontPath) {
  if (initialized_)
    return true;

  FT_Library library;
  if (FT_Init_FreeType(&library) != 0) {
    std::cerr << "[openSEF] Failed to initialize FreeType" << std::endl;
    return false;
  }
  ftLibrary_ = library;

  FT_Face face;
  if (FT_New_Face(library, fontPath.c_str(), 0, &face) != 0) {
    std::cerr << "[openSEF] Failed to load font: " << fontPath << std::endl;
    // Clean up
    FT_Done_FreeType(library);
    ftLibrary_ = nullptr;
    return false;
  }
  ftFace_ = face;

  std::cout << "[openSEF] TextRenderer initialized (font: " << fontPath << ")"
            << std::endl;
  initialized_ = true;
  return true;
}

wlr_scene_buffer *OSFTextRenderer::renderText(wlr_scene_tree *parent,
                                              const std::string &text,
                                              int fontSize, uint32_t color,
                                              FontWeight weight) {

  if (!parent || text.empty() || !initialized_)
    return nullptr;

  FT_Face face = static_cast<FT_Face>(ftFace_);
  FT_Set_Pixel_Sizes(face, 0, fontSize);

  // Measure text first
  TextMetrics metrics = measureText(text, fontSize, weight);
  int width = metrics.width;
  int height = metrics.height;

  if (width <= 0 || height <= 0) return nullptr;

  // Create buffer
  struct wlr_buffer *wlr_buf = create_memory_buffer(width, height);
  if (!wlr_buf) return nullptr;

  OSFMemoryBuffer *buffer = (OSFMemoryBuffer *)wlr_buf;
  uint32_t *pixels = (uint32_t *)buffer->data;

  // Render glyphs
  int penX = 0;
  // metrics.ascent is usually positive (distance from baseline to top)
  // We want to draw relative to top-left of the buffer.
  // The baseline should be at y = metrics.ascent
  int baselineY = metrics.ascent;

  // Extract color components
  uint32_t a = (color >> 24) & 0xFF;
  uint32_t r = (color >> 16) & 0xFF;
  uint32_t g = (color >> 8) & 0xFF;
  uint32_t b = (color) & 0xFF;

  // If color is 0xRRGGBB (no alpha), assume opaque
  if (a == 0 && color != 0) a = 0xFF;

  for (char c : text) {
    // Load glyph
    if (FT_Load_Char(face, c, FT_LOAD_RENDER) != 0)
      continue;

    FT_Bitmap *bitmap = &face->glyph->bitmap;
    int x = penX + face->glyph->bitmap_left;
    int y = baselineY - face->glyph->bitmap_top;

    // Blit bitmap to buffer
    for (unsigned int row = 0; row < bitmap->rows; ++row) {
      for (unsigned int col = 0; col < bitmap->width; ++col) {
        int dstX = x + col;
        int dstY = y + row;

        if (dstX >= 0 && dstX < width && dstY >= 0 && dstY < height) {
           uint8_t alpha = bitmap->buffer[row * bitmap->pitch + col];
           if (alpha > 0) {
             // Simple alpha blending with pre-multiplied alpha
             // Pixel format is ARGB8888 (Little Endian -> B G R A in memory)

             // Composite with color
             uint32_t finalAlpha = (alpha * a) / 255;
             uint32_t finalR = (r * finalAlpha) / 255;
             uint32_t finalG = (g * finalAlpha) / 255;
             uint32_t finalB = (b * finalAlpha) / 255;

             // Write to buffer (B G R A order for Little Endian ARGB8888)
             pixels[dstY * width + dstX] = (finalAlpha << 24) | (finalR << 16) | (finalG << 8) | finalB;
           }
        }
      }
    }

    penX += face->glyph->advance.x >> 6;
  }

  // Create scene node
  wlr_scene_buffer *node = wlr_scene_buffer_create(parent, wlr_buf);

  // The scene node takes a reference. We can drop our reference.
  wlr_buffer_drop(wlr_buf);

  return node;
}

OSFTextRenderer::TextMetrics
OSFTextRenderer::measureText(const std::string &text, int fontSize,
                             FontWeight weight) {

  if (!initialized_) {
      return {0, 0, 0, 0};
  }

  FT_Face face = static_cast<FT_Face>(ftFace_);
  FT_Set_Pixel_Sizes(face, 0, fontSize);

  TextMetrics metrics;
  metrics.width = 0;
  // FreeType metrics are in 26.6 fixed point format, so shift by 6
  metrics.ascent = face->size->metrics.ascender >> 6;
  metrics.descent = -(face->size->metrics.descender >> 6); // Descent is usually negative
  metrics.height = metrics.ascent + metrics.descent;

  // Ensure height is at least fontSize to avoid clipping
  if (metrics.height < fontSize) metrics.height = fontSize;

  (void)weight; // Weight support TODO

  for (char c : text) {
    if (FT_Load_Char(face, c, FT_LOAD_DEFAULT) != 0)
      continue;
    metrics.width += face->glyph->advance.x >> 6;
  }

  return metrics;
}

void OSFTextRenderer::preloadGlyphs(int fontSize) {
  if (!initialized_) return;

  FT_Face face = static_cast<FT_Face>(ftFace_);
  FT_Set_Pixel_Sizes(face, 0, fontSize);

  // Preload printable ASCII
  for (unsigned char c = 32; c < 127; ++c) {
     FT_Load_Char(face, c, FT_LOAD_RENDER);
     // FreeType caches the glyph slot, repeated calls might help caching internally
  }

  std::cout << "[openSEF] Preloaded glyphs for size " << fontSize << std::endl;
}

uint64_t OSFTextRenderer::makeGlyphKey(uint32_t codepoint, int fontSize,
                                       FontWeight weight) {
  // Pack into 64-bit key: codepoint (32) | size (16) | weight (16)
  return (static_cast<uint64_t>(codepoint) << 32) |
         (static_cast<uint64_t>(fontSize) << 16) |
         static_cast<uint64_t>(weight);
}

// ============================================================================
// OSFLabel Implementation
// ============================================================================

OSFLabel::OSFLabel(wlr_scene_tree *parent) : parent_(parent) {}

OSFLabel::~OSFLabel() {
  if (buffer_) {
    wlr_scene_node_destroy(&buffer_->node);
    buffer_ = nullptr;
  }
}

void OSFLabel::setText(const std::string &text) {
  if (text_ != text) {
    text_ = text;
    needsRender_ = true;
    render();
  }
}

void OSFLabel::setFontSize(int size) {
  if (fontSize_ != size) {
    fontSize_ = size;
    needsRender_ = true;
    render();
  }
}

void OSFLabel::setColor(uint32_t color) {
  if (color_ != color) {
    color_ = color;
    needsRender_ = true;
    render();
  }
}

void OSFLabel::setWeight(FontWeight weight) {
  if (weight_ != weight) {
    weight_ = weight;
    needsRender_ = true;
    render();
  }
}

void OSFLabel::setPosition(int x, int y) {
  x_ = x;
  y_ = y;
  if (buffer_) {
    int offsetX = 0;
    if (alignment_ == TextAlign::Center) {
      offsetX = -renderedWidth_ / 2;
    } else if (alignment_ == TextAlign::Right) {
      offsetX = -renderedWidth_;
    }
    wlr_scene_node_set_position(&buffer_->node, x + offsetX, y);
  }
}

void OSFLabel::setAlignment(TextAlign align) {
  if (alignment_ != align) {
    alignment_ = align;
    setPosition(x_, y_); // Reapply position with new alignment
  }
}

void OSFLabel::invalidate() {
  needsRender_ = true;
  render();
}

void OSFLabel::render() {
  if (!needsRender_ || !parent_)
    return;

  // Destroy old buffer
  if (buffer_) {
    wlr_scene_node_destroy(&buffer_->node);
    buffer_ = nullptr;
  }

  if (text_.empty()) {
    renderedWidth_ = 0;
    renderedHeight_ = 0;
    needsRender_ = false;
    return;
  }

  // Measure text
  auto metrics =
      OSFTextRenderer::instance().measureText(text_, fontSize_, weight_);
  renderedWidth_ = metrics.width;
  renderedHeight_ = metrics.height;

  // Render text
  buffer_ = OSFTextRenderer::instance().renderText(parent_, text_, fontSize_,
                                                   color_, weight_);

  // Apply position
  setPosition(x_, y_);

  needsRender_ = false;
}

} // namespace opensef
