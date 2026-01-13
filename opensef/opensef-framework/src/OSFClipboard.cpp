/**
 * OSFClipboard.cpp - Universal Clipboard Implementation
 */

#include "OSFClipboard.h"
#include "OSFEventBus.h"

namespace OpenSEF {

OSFClipboard &OSFClipboard::shared() {
  static OSFClipboard instance;
  return instance;
}

OSFClipboard::OSFClipboard() {
  // Enable history by default with 10 items
  enableHistory(10);
}

void OSFClipboard::setText(const std::string &text) {
  m_current = OSFClipboardData();
  m_current.type = OSFClipboardType::Text;
  m_current.text = text;
  notifyChanged();
}

std::string OSFClipboard::text() const { return m_current.text; }

bool OSFClipboard::hasText() const {
  return m_current.type == OSFClipboardType::Text && !m_current.text.empty();
}

void OSFClipboard::setFiles(const std::vector<std::string> &paths) {
  m_current = OSFClipboardData();
  m_current.type = OSFClipboardType::Files;
  m_current.files = paths;
  notifyChanged();
}

std::vector<std::string> OSFClipboard::files() const { return m_current.files; }

bool OSFClipboard::hasFiles() const {
  return m_current.type == OSFClipboardType::Files && !m_current.files.empty();
}

void OSFClipboard::setImage(const std::vector<uint8_t> &data,
                            const std::string &mimeType) {
  m_current = OSFClipboardData();
  m_current.type = OSFClipboardType::Image;
  m_current.imageData = data;
  m_current.imageMimeType = mimeType;
  notifyChanged();
}

std::vector<uint8_t> OSFClipboard::imageData() const {
  return m_current.imageData;
}

std::string OSFClipboard::imageMimeType() const {
  return m_current.imageMimeType;
}

bool OSFClipboard::hasImage() const {
  return m_current.type == OSFClipboardType::Image &&
         !m_current.imageData.empty();
}

void OSFClipboard::setData(const std::vector<uint8_t> &data,
                           const std::string &mimeType) {
  m_current = OSFClipboardData();
  m_current.type = OSFClipboardType::Custom;
  m_current.customData = data;
  m_current.customMimeType = mimeType;
  notifyChanged();
}

OSFClipboardData OSFClipboard::currentData() const { return m_current; }

OSFClipboardType OSFClipboard::currentType() const { return m_current.type; }

void OSFClipboard::clear() {
  m_current = OSFClipboardData();
  notifyChanged();
}

void OSFClipboard::enableHistory(size_t maxItems) {
  m_maxHistoryItems = maxItems;
}

void OSFClipboard::disableHistory() {
  m_maxHistoryItems = 0;
  m_history.clear();
}

std::vector<OSFClipboardData> OSFClipboard::history() const {
  return m_history;
}

void OSFClipboard::clearHistory() { m_history.clear(); }

void OSFClipboard::onChanged(
    std::function<void(const OSFClipboardData &)> callback) {
  m_callbacks.push_back(callback);
}

void OSFClipboard::notifyChanged() {
  // Add to history if enabled
  addToHistory(m_current);

  // Notify local callbacks
  for (const auto &callback : m_callbacks) {
    callback(m_current);
  }

  // Publish to EventBus
  OSFEvent event;
  event.set("type", static_cast<int>(m_current.type));
  if (m_current.type == OSFClipboardType::Text) {
    event.set("text", m_current.text);
  }
  OSFEventBus::shared().publish(OSFEventBus::CLIPBOARD_CHANGED, event);
}

void OSFClipboard::addToHistory(const OSFClipboardData &data) {
  if (m_maxHistoryItems == 0 || data.isEmpty()) {
    return;
  }

  // Don't add duplicates (compare with most recent)
  if (!m_history.empty()) {
    const auto &last = m_history.back();
    if (last.type == data.type && last.text == data.text) {
      return; // Skip duplicate
    }
  }

  m_history.push_back(data);

  // Trim to max size
  while (m_history.size() > m_maxHistoryItems) {
    m_history.erase(m_history.begin());
  }
}

} // namespace OpenSEF
