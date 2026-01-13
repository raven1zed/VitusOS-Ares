#include "MultitaskController.h"
#include <OSFDesktop.h>
#include <OSFEventBus.h>
#include <OSFStateManager.h>
#include <QDebug>
#include <QMetaObject>
#include <algorithm>

MultitaskController::MultitaskController(QObject *parent) : QObject(parent) {
  connectToFramework();
}

void MultitaskController::connectToFramework() {
  qDebug() << "[MultitaskController] Connecting to framework...";

  auto *eventBus = OpenSEF::OSFDesktop::shared()->eventBus();

  eventBus->subscribe(
      OpenSEF::OSFEventBus::WINDOW_CREATED, [this](const OpenSEF::OSFEvent &) {
        QMetaObject::invokeMethod(this,
                                  [this]() { refreshWindows(); });
      });

  eventBus->subscribe(OpenSEF::OSFEventBus::WINDOW_DESTROYED,
                      [this](const OpenSEF::OSFEvent &) {
                        QMetaObject::invokeMethod(
                            this, [this]() { refreshWindows(); });
                      });

  eventBus->subscribe(OpenSEF::OSFEventBus::WINDOW_FOCUSED,
                      [this](const OpenSEF::OSFEvent &) {
                        QMetaObject::invokeMethod(
                            this, [this]() { refreshWindows(); });
                      });

  qDebug() << "[MultitaskController] Success: Connected to OSF framework.";
}

void MultitaskController::setActive(bool active) {
  if (m_active != active) {
    m_active = active;

    if (m_active) {
      refreshWindows();
      m_selectedIndex = 0;
      emit selectedIndexChanged();
    }

    emit activeChanged();
  }
}

void MultitaskController::toggle() { setActive(!m_active); }

void MultitaskController::refreshWindows() {
  m_windows.clear();

  auto *stateManager = OpenSEF::OSFDesktop::shared()->stateManager();
  const auto windows = stateManager->allWindows();

  QString activeWindowId;
  if (auto *activeWindow = stateManager->activeWindow()) {
    activeWindowId = QString::fromStdString(activeWindow->id());
  }

  for (const auto *window : windows) {
    if (!window) {
      continue;
    }

    QVariantMap entry;
    entry["id"] = QString::fromStdString(window->id());
    entry["title"] = QString::fromStdString(window->title());
    entry["appId"] = QString::fromStdString(window->appId());
    entry["thumbnail"] = "";
    m_windows.append(entry);
  }

  emit windowsChanged();

  int nextIndex = -1;
  if (!activeWindowId.isEmpty()) {
    for (int i = 0; i < m_windows.size(); ++i) {
      const auto windowData = m_windows[i].toMap();
      if (windowData.value("id").toString() == activeWindowId) {
        nextIndex = i;
        break;
      }
    }
  }

  if (nextIndex == -1 && !m_windows.isEmpty()) {
    nextIndex = std::min(m_selectedIndex, m_windows.size() - 1);
  }

  if (nextIndex < 0) {
    nextIndex = 0;
  }

  if (nextIndex != m_selectedIndex) {
    m_selectedIndex = nextIndex;
    emit selectedIndexChanged();
  }

  qDebug() << "[MultitaskController] Refreshed windows:" << m_windows.size();
}

void MultitaskController::setSelectedIndex(int index) {
  if (index >= 0 && index < m_windows.size() && m_selectedIndex != index) {
    m_selectedIndex = index;
    emit selectedIndexChanged();
  }
}

void MultitaskController::focusWindow(const QString &windowId) {
  qDebug() << "[MultitaskController] Focus window:" << windowId;

  auto *windowManager = OpenSEF::OSFDesktop::shared()->windowManager();
  windowManager->focusWindow(windowId.toStdString());

  setActive(false);
}

void MultitaskController::closeWindow(const QString &windowId) {
  qDebug() << "[MultitaskController] Close window:" << windowId;

  auto *windowManager = OpenSEF::OSFDesktop::shared()->windowManager();
  windowManager->closeWindow(windowId.toStdString());
}

void MultitaskController::selectNext() {
  if (m_windows.size() > 0) {
    setSelectedIndex((m_selectedIndex + 1) % m_windows.size());
  }
}

void MultitaskController::selectPrevious() {
  if (m_windows.size() > 0) {
    setSelectedIndex((m_selectedIndex - 1 + m_windows.size()) %
                     m_windows.size());
  }
}

void MultitaskController::activateSelected() {
  if (m_selectedIndex >= 0 && m_selectedIndex < m_windows.size()) {
    QVariantMap win = m_windows[m_selectedIndex].toMap();
    focusWindow(win["id"].toString());
  }
}
