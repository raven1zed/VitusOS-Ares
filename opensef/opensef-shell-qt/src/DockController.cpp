#include "DockController.h"
#include <QDebug>
#include <QProcess>

DockController::DockController(QObject *parent) : QObject(parent) {
  initDockItems();
  connectToFramework();
}

void DockController::initDockItems() {
  // Pinned dock items matching current dock design
  m_dockItems.clear();

  struct AppDef {
    QString name;
    QString icon;
    QString command;
    QString color;
    bool running;
  };

  QList<AppDef> apps = {
      {"Filer", "system-file-manager", "nautilus", "#4A9FD4",
       true}, // Always running like Finder
      {"Terminal", "utilities-terminal", "gnome-terminal", "#333333", false},
      {"Firefox", "firefox", "firefox", "#E66000", false},
      {"Discord", "discord", "discord", "#5865F2", false},
      {"Spotify", "spotify", "spotify", "#1DB954", false},
      {"Settings", "preferences-system", "gnome-control-center", "#666666",
       false}};

  for (const auto &app : apps) {
    QVariantMap item;
    item["name"] = app.name;
    item["icon"] = app.icon;
    item["command"] = app.command;
    item["color"] = app.color;
    item["running"] = app.running;
    m_dockItems.append(item);
  }

  emit dockItemsChanged();
}

void DockController::connectToFramework() {
  // TODO: Connect to OSFEventBus for:
  // - application.launched
  // - application.closed
  // - window.moved (for autohide)

  qDebug() << "[DockController] Connecting to framework...";
}

void DockController::setHoveredIndex(int index) {
  if (m_hoveredIndex != index) {
    m_hoveredIndex = index;
    emit hoveredIndexChanged();
  }
}

void DockController::launchApp(int index) {
  if (index < 0 || index >= m_dockItems.size())
    return;

  QVariantMap item = m_dockItems[index].toMap();
  QString command = item["command"].toString();

  qDebug() << "[DockController] Launching:" << command;

  // Launch application detached
  QProcess::startDetached(command, QStringList());

  // Mark as running
  item["running"] = true;
  m_dockItems[index] = item;
  emit dockItemsChanged();
}

void DockController::showDock() {
  if (m_isHidden) {
    m_isHidden = false;
    emit isHiddenChanged();
  }
}

void DockController::hideDock() {
  if (!m_isHidden) {
    m_isHidden = true;
    emit isHiddenChanged();
  }
}

void DockController::checkOverlap() {
  // TODO: Query StateManager for window positions
  // and hide dock if windows overlap dock region
}
