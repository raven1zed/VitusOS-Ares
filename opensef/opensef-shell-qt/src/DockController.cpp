#include "DockController.h"
#include <QDebug>
#include <QDir>
#include <QProcess>
#include <opensef/OSFDesktop.h>
#include <opensef/OSFEventBus.h>
#include <opensef/OSFStateManager.h>

DockController::DockController(QObject *parent)
    : QObject(parent), m_hoveredIndex(-1), m_isHidden(false) {
  initDockItems();
  connectToFramework();
}

void DockController::initDockItems() {
  m_dockItems.clear();

  struct AppDef {
    QString name;
    QString icon;
    QString command;
    QString color;
    bool running;
  };

  QString projectRoot = "/mnt/c/Users/hp/Documents/VitusOS-Ares";

  QList<AppDef> apps = {
      {"Filer", "system-file-manager",
       projectRoot + "/opensef/apps/osf-filer-native/build/osf-filer-native",
       "#4A9FD4", false},
      {"Terminal", "utilities-terminal",
       projectRoot + "/opensef/apps/osf-terminal/build/osf-terminal", "#333333",
       false},
      {"Firefox", "firefox", "firefox", "#E66000", false},
      {"Settings", "preferences-system",
       projectRoot + "/opensef/apps/osf-settings/build/osf-settings", "#666666",
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
  auto *desktop = OpenSEF::OSFDesktop::shared();
  if (!desktop)
    return;

  // Track window creation
  desktop->eventBus()->subscribe(
      OpenSEF::OSFEventBus::WINDOW_CREATED,
      [this](const OpenSEF::OSFEvent &event) { refreshRunningStatus(); });

  // Track window destruction
  desktop->eventBus()->subscribe(OpenSEF::OSFEventBus::WINDOW_DESTROYED,
                                 [this](const OpenSEF::OSFEvent &event) {
                                   refreshRunningStatus();
                                   checkOverlap();
                                 });

  // Track window geometry moves to trigger autohide
  desktop->eventBus()->subscribe(
      OpenSEF::OSFEventBus::WINDOW_GEOMETRY_CHANGED,
      [this](const OpenSEF::OSFEvent &event) { checkOverlap(); });

  qDebug() << "[DockController] Connected to OSF framework.";
  refreshRunningStatus(); // Initial sync
  checkOverlap();         // Initial check
}

void DockController::refreshRunningStatus() {
  auto *desktop = OpenSEF::OSFDesktop::shared();
  if (!desktop || !desktop->stateManager())
    return;

  auto windows = desktop->stateManager()->allWindows();
  bool changed = false;

  for (int i = 0; i < m_dockItems.size(); ++i) {
    QVariantMap item = m_dockItems[i].toMap();
    bool isRunning = false;
    QString cmdBasename = item["name"].toString().toLower();

    // 1. Framework Check (Source of Truth)
    for (auto *win : windows) {
      if (!win)
        continue;
      QString appId = QString::fromStdString(win->appId()).toLower();
      QString title = QString::fromStdString(win->title()).toLower();

      if (appId.contains(cmdBasename) || cmdBasename.contains(appId) ||
          title.contains(cmdBasename) ||
          (cmdBasename == "filer" && appId.contains("filer")) ||
          (cmdBasename == "terminal" && appId.contains("terminal"))) {
        isRunning = true;
        break;
      }
    }

    // Special case: Filer is a core component and is ALWAYS running (like macOS
    // Finder)
    if (cmdBasename == "filer") {
      isRunning = true;
    }

    if (item["running"].toBool() != isRunning) {
      item["running"] = isRunning;
      m_dockItems[i] = item;
      changed = true;
    }
  }

  if (changed) {
    emit dockItemsChanged();
  }
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
  QString name = item["name"].toString();

  qDebug() << "[DockController] Launching:" << command;

  // Environment pass-through
  const QByteArray waylandDisplay = qgetenv("WAYLAND_DISPLAY");
  const QByteArray runtimeDir = qgetenv("XDG_RUNTIME_DIR");

  // Use GPU acceleration (Discrete GPU detected)
  QString safeCommand = QString("env WAYLAND_DISPLAY=%1 XDG_RUNTIME_DIR=%2 "
                                "LIBGL_ALWAYS_SOFTWARE=0 %3")
                            .arg(QString::fromUtf8(waylandDisplay))
                            .arg(QString::fromUtf8(runtimeDir))
                            .arg(command);

  QProcess::startDetached("/bin/sh", {"-c", safeCommand});

  // NOTE: We no longer simulate windows here because Filer/Terminal
  // report their own creation via the openSEF framework once they start.
}

void DockController::onProcessFinished(int exitCode, int exitStatus) {
  // Deprecated for now - moving to full framework tracking
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
  auto *desktop = OpenSEF::OSFDesktop::shared();
  if (!desktop || !desktop->stateManager())
    return;

  bool shouldHide = false;
  auto windows = desktop->stateManager()->allWindows();

  // Screen resolution (assume 1920x1080 for now or get from shell)
  // Dock trigger zone is bottom 120 pixels
  int dockZoneY = 1080 - 150;

  for (auto *win : windows) {
    if (!win)
      continue;
    // If any window's bottom Y is in/below the dock zone, hide it
    if (win->y() + win->height() > dockZoneY) {
      shouldHide = true;
      break;
    }
  }

  if (shouldHide) {
    hideDock();
  } else {
    showDock();
  }
}

DockController::~DockController() {}

QVariantList DockController::dockItems() const { return m_dockItems; }

bool DockController::isHidden() const { return m_isHidden; }

int DockController::hoveredIndex() const { return m_hoveredIndex; }

void DockController::toggleAutoHide() {
  if (m_isHidden) {
    showDock();
  } else {
    hideDock();
  }
}
