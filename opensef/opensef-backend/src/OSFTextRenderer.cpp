/**
 * OSFTextRenderer.cpp - FreeType text rendering implementation
 */

#include <iostream>
#include <opensef/OSFTextRenderer.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace opensef {

OSFTextRenderer &OSFTextRenderer::shared() {
  static OSFTextRenderer instance;
  return instance;
}

OSFTextRenderer::~OSFTextRenderer() { shutdown(); }

bool OSFTextRenderer::initialize() {
  if (initialized_)
    return true;

  FT_Library library = nullptr;
  FT_Error error = FT_Init_FreeType(&library);
  if (error) {
    std::cerr << "[openSEF] Failed to initialize FreeType" << std::endl;
    return false;
  }

  ftLibrary_ = library;
  std::cout << "[openSEF] FreeType initialized" << std::endl;
  initialized_ = true;

  // Try to load default fonts - bundled first, then system
  const char *fontPaths[] = {
      // BUNDLED Inter font (in VitusOS project)
      "../resources/fonts/InterVariable.ttf",
      "resources/fonts/InterVariable.ttf",
      "../../resources/fonts/InterVariable.ttf",
      // NixOS system fonts
      "/run/current-system/sw/share/fonts/truetype/dejavu-fonts/DejaVuSans.ttf",
      "/run/current-system/sw/share/fonts/truetype/noto-fonts/"
      "NotoSans-Regular.ttf",
      // Standard Linux
      "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", nullptr};

  for (const char **path = fontPaths; *path; ++path) {
    if (loadFont(kFontUI, *path)) {
      std::cout << "[openSEF] Loaded font: " << *path << std::endl;
      break;
    }
  }

  if (fonts_.empty()) {
    std::cerr << "[openSEF] Warning: No fonts loaded" << std::endl;
  }

  return true;
}

void OSFTextRenderer::shutdown() {
  FT_Library library = static_cast<FT_Library>(ftLibrary_);

  for (auto &[name, facePtr] : fonts_) {
    if (facePtr) {
      FT_Face face = static_cast<FT_Face>(facePtr);
      FT_Done_Face(face);
    }
  }
  fonts_.clear();

  if (library) {
    FT_Done_FreeType(library);
    ftLibrary_ = nullptr;
  }
  initialized_ = false;
}

bool OSFTextRenderer::loadFont(const std::string &name,
                               const std::string &path) {
  FT_Library library = static_cast<FT_Library>(ftLibrary_);
  if (!library)
    return false;

  FT_Face face = nullptr;
  FT_Error error = FT_New_Face(library, path.c_str(), 0, &face);
  if (error) {
    return false;
  }

  // Remove old font with same name
  auto it = fonts_.find(name);
  if (it != fonts_.end() && it->second) {
    FT_Done_Face(static_cast<FT_Face>(it->second));
  }

  fonts_[name] = face;
  return true;
}

bool OSFTextRenderer::setActiveFont(const std::string &name) {
  if (fonts_.find(name) != fonts_.end()) {
    activeFont_ = name;
    return true;
  }
  return false;
}

void OSFTextRenderer::drawText(uint32_t *buffer, int bufferWidth,
                               int bufferHeight, int x, int y,
                               const std::string &text, const OSFColor &color,
                               int fontSize) {
  if (!initialized_ || fonts_.empty())
    return;

  FT_Face face = static_cast<FT_Face>(fonts_[activeFont_]);
  if (!face) {
    // Fall back to first available font
    face = static_cast<FT_Face>(fonts_.begin()->second);
  }
  if (!face)
    return;

  FT_Set_Pixel_Sizes(face, 0, fontSize);

  int penX = x;
  int penY = y;

  // UTF-8 decoding and rendering
  for (size_t i = 0; i < text.size();) {
    char32_t codepoint = 0;
    unsigned char c = static_cast<unsigned char>(text[i]);

    // UTF-8 decode
    if (c < 0x80) {
      codepoint = c;
      i += 1;
    } else if ((c & 0xE0) == 0xC0) {
      codepoint = (c & 0x1F) << 6;
      if (i + 1 < text.size())
        codepoint |= (text[i + 1] & 0x3F);
      i += 2;
    } else if ((c & 0xF0) == 0xE0) {
      codepoint = (c & 0x0F) << 12;
      if (i + 1 < text.size())
        codepoint |= (text[i + 1] & 0x3F) << 6;
      if (i + 2 < text.size())
        codepoint |= (text[i + 2] & 0x3F);
      i += 3;
    } else if ((c & 0xF8) == 0xF0) {
      codepoint = (c & 0x07) << 18;
      if (i + 1 < text.size())
        codepoint |= (text[i + 1] & 0x3F) << 12;
      if (i + 2 < text.size())
        codepoint |= (text[i + 2] & 0x3F) << 6;
      if (i + 3 < text.size())
        codepoint |= (text[i + 3] & 0x3F);
      i += 4;
    } else {
      i += 1;
      continue;
    }

    // Load glyph
    FT_UInt glyphIndex = FT_Get_Char_Index(face, codepoint);
    if (FT_Load_Glyph(face, glyphIndex, FT_LOAD_RENDER)) {
      continue;
    }

    FT_GlyphSlot slot = face->glyph;
    FT_Bitmap &bitmap = slot->bitmap;

    // Draw glyph to buffer
    int glyphX = penX + slot->bitmap_left;
    int glyphY = penY - slot->bitmap_top;

    for (unsigned int row = 0; row < bitmap.rows; row++) {
      for (unsigned int col = 0; col < bitmap.width; col++) {
        int bufX = glyphX + static_cast<int>(col);
        int bufY = glyphY + static_cast<int>(row);

        if (bufX >= 0 && bufX < bufferWidth && bufY >= 0 &&
            bufY < bufferHeight) {
          uint8_t alpha = bitmap.buffer[row * bitmap.pitch + col];
          if (alpha > 0) {
            // Alpha blend with color
            float a = (alpha / 255.0f) * color.a;
            uint32_t blended = (static_cast<uint32_t>(a * 255) << 24) |
                               (static_cast<uint32_t>(color.r * 255) << 16) |
                               (static_cast<uint32_t>(color.g * 255) << 8) |
                               static_cast<uint32_t>(color.b * 255);
            buffer[bufY * bufferWidth + bufX] = blended;
          }
        }
      }
    }

    penX += slot->advance.x >> 6;
  }
}

int OSFTextRenderer::measureTextWidth(const std::string &text, int fontSize) {
  if (!initialized_ || fonts_.empty())
    return 0;

  FT_Face face = static_cast<FT_Face>(fonts_[activeFont_]);
  if (!face)
    face = static_cast<FT_Face>(fonts_.begin()->second);
  if (!face)
    return 0;

  FT_Set_Pixel_Sizes(face, 0, fontSize);

  int width = 0;
  for (unsigned char c : text) {
    if (c >= 0x80)
      continue; // Skip non-ASCII for measurement
    FT_UInt glyphIndex = FT_Get_Char_Index(face, c);
    if (FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT) == 0) {
      width += face->glyph->advance.x >> 6;
    }
  }
  return width;
}

int OSFTextRenderer::getLineHeight(int fontSize) {
  return static_cast<int>(fontSize * 1.2f);
}

} // namespace opensef
