#include "MultitaskController.h"
#include "OSFDesktop.h"
#include "OSFEventBus.h"
#include "OSFStateManager.h"
#include "OSFWindowManager.h"
#include <QDebug>
#include <QProcess>

MultitaskController::MultitaskController(QObject *parent)
    : QObject(parent), m_active(false), m_selectedIndex(0) {
  qDebug()
      << "[MultitaskController] Initializing. Active state forced to FALSE.";
  connectToFramework();
  refreshWindows();
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

  auto *desktop = OpenSEF::OSFDesktop::shared();
  if (!desktop || !desktop->stateManager()) {
    qWarning() << "[MultitaskController] StateManager not available!";
    return;
  }

  auto windows = desktop->stateManager()->allWindows();
  for (auto *window : windows) {
    // Skip the shell itself
    if (window->appId() == "vitusos.shell")
      continue;

    QVariantMap win;
    win["id"] = QString::fromStdString(window->id());
    win["title"] = QString::fromStdString(window->title());
    win["appId"] = QString::fromStdString(window->appId());

    // Derived name from appId if title is empty
    QString appId = QString::fromStdString(window->appId());
    QString name = appId.split('.').last();
    if (name.isEmpty())
      name = "Application";

    win["name"] = name;
    win["thumbnail"] = ""; // Screen capture not implemented yet
    win["icon"] = "";      // Icon path - could be populated from app metadata
    win["color"] =
        "#2D2D2D"; // Default dark gray - could be app-specific theme color

    m_windows.append(win);
  }

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
  auto *desktop = OpenSEF::OSFDesktop::shared();
  if (desktop && desktop->windowManager()) {
    desktop->windowManager()->focusWindow(windowId.toStdString());
    setActive(false);
  }
}

void MultitaskController::activateWindow(const QString &windowId) {
  // Alias for focusWindow - same behavior
  focusWindow(windowId);
}

void MultitaskController::closeWindow(const QString &windowId) {
  auto *desktop = OpenSEF::OSFDesktop::shared();
  if (desktop && desktop->windowManager()) {
    desktop->windowManager()->closeWindow(windowId.toStdString());
  }
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
