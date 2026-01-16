#include "MultitaskController.h"
#include <opensef/OSFDesktop.h>
#include <opensef/OSFEventBus.h>
#include <opensef/OSFStateManager.h>
#include <opensef/OSFWindowManager.h>
#include <opensef/OSFDesktop.h>
#include <opensef/OSFEventBus.h>
#include <opensef/OSFStateManager.h>
#include <QDebug>
#include <QMetaObject>
#include <QProcess>
#include <algorithm>

MultitaskController::MultitaskController(QObject *parent)
    : QObject(parent), m_active(false), m_selectedIndex(0) {
  qDebug()
      << "[MultitaskController] Initializing. Active state forced to FALSE.";
  connectToFramework();
}

void MultitaskController::connectToFramework() {
  qDebug() << "[MultitaskController] Connecting to framework...";

  auto *desktop = OpenSEF::OSFDesktop::shared();
  if (!desktop)
    return;

  desktop->eventBus()->subscribe(
      OpenSEF::OSFEventBus::WINDOW_CREATED,
      [this](const OpenSEF::OSFEvent &event) { refreshWindows(); });

  desktop->eventBus()->subscribe(
      OpenSEF::OSFEventBus::WINDOW_DESTROYED,
      [this](const OpenSEF::OSFEvent &event) { refreshWindows(); });

  qDebug() << "[MultitaskController] Connected to OSF framework.";
}

void MultitaskController::setActive(bool active) {
  if (m_active != active) {
    m_active = active;
    if (m_active) {
      refreshWindows();
    }
    emit activeChanged();
  }
}

void MultitaskController::toggle() {
  qDebug() << "[MultitaskController] Toggled via UI/Shortcut";
  setActive(!m_active);
}

void MultitaskController::refreshWindows() {
  m_windows.clear();

  // Native windows for Fusion environment
  QVariantMap win1;
  win1["id"] = "window-1";
  win1["title"] = "Filer";
  win1["appId"] = "com.vitusos.filer";
  win1["thumbnail"] = "";
  win1["name"] = "Filer";
  m_windows.append(win1);

  QVariantMap win2;
  win2["id"] = "window-2";
  win2["title"] = "Safari";
  win2["appId"] = "com.vitusos.safari";
  win2["thumbnail"] = "";
  win2["name"] = "Safari";
  m_windows.append(win2);

  QVariantMap win3;
  win3["id"] = "window-3";
  win3["title"] = "Terminal";
  win3["appId"] = "com.vitusos.terminal";
  win3["thumbnail"] = "";
  win3["name"] = "Terminal";
  m_windows.append(win3);

  emit windowsChanged();
  qDebug() << "[MultitaskController] Refreshed windows:" << m_windows.size();

  if (m_selectedIndex >= m_windows.size()) {
    setSelectedIndex(qMax(0, m_windows.size() - 1));
  }
}

void MultitaskController::setSelectedIndex(int index) {
  if (index >= 0 && index < m_windows.size() && m_selectedIndex != index) {
    m_selectedIndex = index;
    emit selectedIndexChanged();
  }
}

void MultitaskController::focusWindow(const QString &windowId) {
  qDebug() << "[MultitaskController] Focus window:" << windowId;

  // TODO: Tell compositor to focus window
  // osf_window_focus(windowId.toStdString().c_str());

  setActive(false);
}

void MultitaskController::activateWindow(const QString &windowId) {
  focusWindow(windowId);
}

void MultitaskController::closeWindow(const QString &windowId) {
  qDebug() << "[MultitaskController] Close window:" << windowId;
  // TODO: Tell compositor to close window
  refreshWindows();
}

void MultitaskController::minimizeWindow(const QString &windowId) {
  qDebug() << "[MultitaskController] Minimize window:" << windowId;
  // TODO: Tell compositor to minimize window
  setActive(false);
}

void MultitaskController::maximizeWindow(const QString &windowId) {
  qDebug() << "[MultitaskController] Maximize window:" << windowId;
  // TODO: Tell compositor to maximize/toggle window
  setActive(false);
}

void MultitaskController::selectNext() {
  if (m_windows.isEmpty())
    return;
  setSelectedIndex((m_selectedIndex + 1) % m_windows.size());
}

void MultitaskController::selectPrevious() {
  if (m_windows.isEmpty())
    return;
  setSelectedIndex((m_selectedIndex - 1 + m_windows.size()) % m_windows.size());
}

void MultitaskController::activateSelected() {
  if (m_selectedIndex >= 0 && m_selectedIndex < m_windows.size()) {
    focusWindow(m_windows[m_selectedIndex].toMap()["id"].toString());
  }
}
