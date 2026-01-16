/**
 * OSFClipboard.h - Universal Clipboard System
 *
 * Unified clipboard for all openSEF applications
 * Foundation for SeaDrop cross-device sharing
 */

#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace OpenSEF {

/**
 * Clipboard content types
 */
enum class OSFClipboardType { Empty, Text, Files, Image, RichText, Custom };

/**
 * Clipboard data container
 */
struct OSFClipboardData {
  OSFClipboardType type = OSFClipboardType::Empty;
  std::string text;
  std::vector<std::string> files;
  std::vector<uint8_t> imageData;
  std::string imageMimeType;
  std::string customMimeType;
  std::vector<uint8_t> customData;

  bool isEmpty() const { return type == OSFClipboardType::Empty; }
};

/**
 * OSFClipboard - Universal clipboard manager
 *
 * Features:
 * - Text, file, and image clipboard support
 * - History (last N items)
 * - Change notifications via EventBus
 * - Foundation for SeaDrop sync
 */
class OSFClipboard {
public:
  static OSFClipboard &shared();

  // Text operations
  void setText(const std::string &text);
  std::string text() const;
  bool hasText() const;

  // File operations
  void setFiles(const std::vector<std::string> &paths);
  std::vector<std::string> files() const;
  bool hasFiles() const;

  // Image operations
  void setImage(const std::vector<uint8_t> &data,
                const std::string &mimeType = "image/png");
  std::vector<uint8_t> imageData() const;
  std::string imageMimeType() const;
  bool hasImage() const;

  // Generic data
  void setData(const std::vector<uint8_t> &data, const std::string &mimeType);

  // Current content
  OSFClipboardData currentData() const;
  OSFClipboardType currentType() const;

  // Clear
  void clear();

  // History
  void enableHistory(size_t maxItems = 10);
  void disableHistory();
  std::vector<OSFClipboardData> history() const;
  void clearHistory();

  // Change notifications (also published to EventBus)
  void onChanged(std::function<void(const OSFClipboardData &)> callback);

private:
  OSFClipboard();
  OSFClipboard(const OSFClipboard &) = delete;
  OSFClipboard &operator=(const OSFClipboard &) = delete;

  void notifyChanged();
  void addToHistory(const OSFClipboardData &data);

  OSFClipboardData m_current;
  std::vector<OSFClipboardData> m_history;
  size_t m_maxHistoryItems = 0;
  std::vector<std::function<void(const OSFClipboardData &)>> m_callbacks;
};

} // namespace OpenSEF
