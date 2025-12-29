/**
 * OSFTextRenderer.h - FreeType text rendering
 *
 * Uses Inter font for UI, JetBrains Mono for terminal
 */

#pragma once

#include <memory>
#include <opensef/OpenSEFBase.h>
#include <string>
#include <unordered_map>
#include <vector>


namespace opensef {

// ============================================================================
// OSFTextRenderer - FreeType-based text rendering
// ============================================================================

class OSFTextRenderer {
public:
  static OSFTextRenderer &shared();

  // Initialize FreeType and load fonts
  bool initialize();
  void shutdown();

  // Font management
  bool loadFont(const std::string &name, const std::string &path);
  bool setActiveFont(const std::string &name);

  // Text rendering - renders text to a pixel buffer (ARGB format)
  void drawText(uint32_t *buffer, int bufferWidth, int bufferHeight, int x,
                int y, const std::string &text, const OSFColor &color,
                int fontSize);

  // Text measurement
  int measureTextWidth(const std::string &text, int fontSize);
  int getLineHeight(int fontSize);

  // Default fonts
  static constexpr const char *kFontUI = "inter";
  static constexpr const char *kFontMono = "jetbrains-mono";

private:
  OSFTextRenderer() = default;
  ~OSFTextRenderer();

  // Use void* to avoid FreeType header dependency
  void *ftLibrary_ = nullptr;
  std::unordered_map<std::string, void *> fonts_;
  std::string activeFont_ = kFontUI;
  bool initialized_ = false;
};

} // namespace opensef
