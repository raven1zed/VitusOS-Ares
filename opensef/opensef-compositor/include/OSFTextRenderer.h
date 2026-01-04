/**
 * OSFTextRenderer.h - Text Rendering for Scene Graph
 *
 * Provides text rendering capability for UI elements using FreeType2.
 * Renders text to wlr_scene_buffer nodes.
 */

#pragma once

extern "C" {
#include <wlr/types/wlr_buffer.h>
#include <wlr/types/wlr_scene.h>

}

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

namespace opensef {

// Forward declarations
class OSFDesktopLayers;

/**
 * Text alignment options
 */
enum class TextAlign { Left, Center, Right };

/**
 * Font weight
 */
enum class FontWeight { Regular, Medium, Bold };

/**
 * OSFTextRenderer - Renders text to scene buffers
 */
class OSFTextRenderer {
public:
  static OSFTextRenderer &instance();

  /**
   * Initialize the text renderer with font paths.
   * Call once at startup.
   */
  bool initialize(const std::string &fontPath);

  /**
   * Render text and return a scene buffer node.
   * The caller is responsible for positioning the node.
   */
  wlr_scene_buffer *renderText(wlr_scene_tree *parent, const std::string &text,
                               int fontSize, uint32_t color,
                               FontWeight weight = FontWeight::Regular);

  /**
   * Get text dimensions without rendering.
   */
  struct TextMetrics {
    int width;
    int height;
    int ascent;
    int descent;
  };

  TextMetrics measureText(const std::string &text, int fontSize,
                          FontWeight weight = FontWeight::Regular);

  /**
   * Pre-load common characters for performance.
   */
  void preloadGlyphs(int fontSize);

  /**
   * Check if renderer is ready.
   */
  bool isInitialized() const { return initialized_; }

private:
  OSFTextRenderer() = default;
  ~OSFTextRenderer();

  bool initialized_ = false;

  // Font data (would hold FreeType handles in real implementation)
  void *ftLibrary_ = nullptr;
  void *ftFace_ = nullptr;

  // Glyph cache
  struct GlyphData {
    int width;
    int height;
    int bearingX;
    int bearingY;
    int advance;
    std::vector<uint8_t> bitmap;
  };

  std::unordered_map<uint64_t, GlyphData> glyphCache_;

  uint64_t makeGlyphKey(uint32_t codepoint, int fontSize, FontWeight weight);
};

/**
 * OSFLabel - A text label in the scene graph
 *
 * Convenience class for displaying static or dynamic text.
 */
class OSFLabel {
public:
  OSFLabel(wlr_scene_tree *parent);
  ~OSFLabel();

  void setText(const std::string &text);
  const std::string &text() const { return text_; }

  void setFontSize(int size);
  int fontSize() const { return fontSize_; }

  void setColor(uint32_t color);
  uint32_t color() const { return color_; }

  void setWeight(FontWeight weight);
  FontWeight weight() const { return weight_; }

  void setPosition(int x, int y);
  int x() const { return x_; }
  int y() const { return y_; }

  void setAlignment(TextAlign align);
  TextAlign alignment() const { return alignment_; }

  // Force re-render
  void invalidate();

  // Get rendered size
  int renderedWidth() const { return renderedWidth_; }
  int renderedHeight() const { return renderedHeight_; }

private:
  wlr_scene_tree *parent_;
  wlr_scene_buffer *buffer_ = nullptr;

  std::string text_;
  int fontSize_ = 14;
  uint32_t color_ = 0xFFFFFF;
  FontWeight weight_ = FontWeight::Regular;
  TextAlign alignment_ = TextAlign::Left;
  int x_ = 0;
  int y_ = 0;

  int renderedWidth_ = 0;
  int renderedHeight_ = 0;
  bool needsRender_ = true;

  void render();
};

} // namespace opensef
