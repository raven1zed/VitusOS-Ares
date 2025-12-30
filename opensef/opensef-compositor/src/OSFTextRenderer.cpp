/**
 * OSFTextRenderer.cpp - Text Rendering Implementation
 *
 * Note: Full FreeType integration requires additional dependencies.
 * This implementation provides the structure and placeholder for text
 * rendering.
 */

#include "OSFTextRenderer.h"
#include "OSFAresTheme.h"

#include <cstring>
#include <iostream>


namespace opensef {

// ============================================================================
// OSFTextRenderer Implementation
// ============================================================================

OSFTextRenderer &OSFTextRenderer::instance() {
  static OSFTextRenderer instance;
  return instance;
}

OSFTextRenderer::~OSFTextRenderer() {
  // Cleanup FreeType resources
  // if (ftFace_) FT_Done_Face(static_cast<FT_Face>(ftFace_));
  // if (ftLibrary_) FT_Done_FreeType(static_cast<FT_Library>(ftLibrary_));
}

bool OSFTextRenderer::initialize(const std::string &fontPath) {
  if (initialized_)
    return true;

  // TODO: Initialize FreeType
  // FT_Library library;
  // if (FT_Init_FreeType(&library) != 0) {
  //   std::cerr << "[openSEF] Failed to initialize FreeType" << std::endl;
  //   return false;
  // }
  // ftLibrary_ = library;
  //
  // FT_Face face;
  // if (FT_New_Face(library, fontPath.c_str(), 0, &face) != 0) {
  //   std::cerr << "[openSEF] Failed to load font: " << fontPath << std::endl;
  //   return false;
  // }
  // ftFace_ = face;

  std::cout << "[openSEF] TextRenderer initialized (font: " << fontPath << ")"
            << std::endl;
  initialized_ = true;
  return true;
}

wlr_scene_buffer *OSFTextRenderer::renderText(wlr_scene_tree *parent,
                                              const std::string &text,
                                              int fontSize, uint32_t color,
                                              FontWeight weight) {

  if (!parent || text.empty())
    return nullptr;

  // TODO: Full implementation with FreeType
  // For now, return nullptr - actual text rendering requires FreeType
  // integration

  (void)fontSize;
  (void)color;
  (void)weight;

  std::cout << "[openSEF] TextRenderer::renderText: \"" << text << "\""
            << std::endl;
  return nullptr;
}

OSFTextRenderer::TextMetrics
OSFTextRenderer::measureText(const std::string &text, int fontSize,
                             FontWeight weight) {

  // Approximate metrics based on font size
  // Real implementation would use FreeType metrics

  (void)weight;

  TextMetrics metrics;
  metrics.height = fontSize;
  metrics.ascent = static_cast<int>(fontSize * 0.8f);
  metrics.descent = static_cast<int>(fontSize * 0.2f);

  // Approximate width: ~0.6 * fontSize per character
  metrics.width = static_cast<int>(text.length() * fontSize * 0.6f);

  return metrics;
}

void OSFTextRenderer::preloadGlyphs(int fontSize) {
  // Preload ASCII printable characters
  (void)fontSize;
  std::cout << "[openSEF] Preloading glyphs for size " << fontSize << std::endl;
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
