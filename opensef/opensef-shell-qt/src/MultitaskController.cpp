#include "MultitaskController.h"
#include "OSFDesktop.h"
#include "OSFEventBus.h"
#include "OSFStateManager.h"
#include "OSFWindowManager.h"
#include <OSFDesktop.h>
#include <OSFEventBus.h>
#include <OSFStateManager.h>
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
  // TODO: Connect to OSFEventBus for:
  // - window.created
  // - window.destroyed
  // - window.focused

  qDebug() << "[MultitaskController] Connecting to framework...";
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

void MultitaskController::toggle() { setActive(!m_active); }

void MultitaskController::refreshWindows() {
  m_windows.clear();

  // Mock windows for UI development
  QVariantMap win1;
  win1["id"] = "window-1";
  win1["title"] = "Filer - Documents";
  win1["appId"] = "org.gnome.Nautilus";
  win1["thumbnail"] = ""; // Placeholder
  m_windows.append(win1);

  QVariantMap win2;
  win2["id"] = "window-2";
  win2["title"] = "Firefox";
  win2["appId"] = "firefox";
  win2["thumbnail"] = "";
  m_windows.append(win2);

  QVariantMap win3;
  win3["id"] = "window-3";
  win3["title"] = "Terminal";
  win3["appId"] = "org.gnome.Terminal";
  win3["thumbnail"] = "";
  m_windows.append(win3);

  emit windowsChanged();

  qDebug() << "[MultitaskController] Refreshed windows:" << m_windows.size();

  if (m_selectedIndex >= m_windows.size()) {
    setSelectedIndex(qMax(0, m_windows.size() - 1));
  }
}

void MultitaskController::connectToFramework() {
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

void MultitaskController::closeWindow(const QString &windowId) {
  qDebug() << "[MultitaskController] Close window:" << windowId;

  // TODO: Tell compositor to close window
  // osf_window_close(windowId.toStdString().c_str());

  refreshWindows();
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
