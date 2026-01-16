#include "DockController.h"
#include "OSFDesktop.h"
#include "OSFEventBus.h"
#include "OSFStateManager.h"
#include <QDebug>
#include <QDir>
#include <QProcess>

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

  QString buildRoot = "/mnt/c/Users/hp/Documents/VitusOS-Ares/opensef/build";

  QList<AppDef> apps = {
      {"Filer", "system-file-manager",
       buildRoot + "/apps/osf-filer-native/osf-filer-native", "#4A9FD4", false},
      {"Terminal", "utilities-terminal",
       buildRoot + "/apps/osf-terminal/osf-terminal", "#333333", false},
      {"Firefox", "firefox", "firefox", "#E66000", false},
      {"Settings", "preferences-system",
       buildRoot + "/apps/osf-settings/osf-settings", "#666666", false}};

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

  // Track window creation (Standard Wayland/OSF)
  desktop->eventBus()->subscribe(
      OpenSEF::OSFEventBus::WINDOW_CREATED,
      [this](const OpenSEF::OSFEvent &event) { refreshRunningStatus(); });

  desktop->eventBus()->subscribe(
      OpenSEF::OSFEventBus::WINDOW_DESTROYED,
      [this](const OpenSEF::OSFEvent &event) { refreshRunningStatus(); });

  // Track Native App Lifecycle (For apps like Filer that report directly)
  desktop->eventBus()->subscribe(
      OpenSEF::OSFEventBus::APP_LAUNCHED,
      [this, desktop](const OpenSEF::OSFEvent &event) {
        std::string name = event.get<std::string>("name");
        std::string appId = event.get<std::string>("appId");

        qDebug() << "[DockController] Native App Launched:"
                 << QString::fromStdString(name);

        // Ensure state manager knows about it (if not already via window)
        // This bridges the gap for "Native" apps that might start background
        // first
        if (desktop->stateManager()) {
          auto *win = new OpenSEF::OSFWindow(appId, name, appId);
          desktop->stateManager()->addWindow(win);
        }
        refreshRunningStatus();
      });

  desktop->eventBus()->subscribe(
      OpenSEF::OSFEventBus::APP_TERMINATED,
      [this, desktop](const OpenSEF::OSFEvent &event) {
        std::string appId = event.get<std::string>("appId");
        qDebug() << "[DockController] Native App Terminated:"
                 << QString::fromStdString(appId);

        if (desktop->stateManager()) {
          desktop->stateManager()->removeWindow(appId);
        }
        refreshRunningStatus();
      });

  qDebug() << "[DockController] Connected to OSF framework.";
  refreshRunningStatus(); // Initial sync
}

void DockController::refreshRunningStatus() {
  auto *desktop = OpenSEF::OSFDesktop::shared();
  if (!desktop || !desktop->stateManager())
    return;

  auto windows = desktop->stateManager()->allWindows();
  bool changed = false;

  // Removed: Manual Process Polling (pgrep) - Now 100% Event Driven

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

      // Improved matching: handle osf-filer-native, osf-terminal, etc.
      // Match if appId contains the name OR name is in appId
      if (appId.contains(cmdBasename) || cmdBasename.contains(appId) ||
          title.contains(cmdBasename) ||
          // Special case: "filer" matches "osf-filer-native"
          (cmdBasename == "filer" && appId.contains("filer")) ||
          (cmdBasename == "terminal" && appId.contains("terminal"))) {
        isRunning = true;
        qDebug() << "[DockController] Matched" << cmdBasename
                 << "to appId:" << appId;
        break;
      }
    }

    // Special case: Filer is ALWAYS running (like macOS Finder)
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

  qDebug() << "[DockController] Launching:" << command;

  // Environment pass-through for WSLg consistency
  const QByteArray waylandDisplay = qgetenv("WAYLAND_DISPLAY");
  const QByteArray runtimeDir = qgetenv("XDG_RUNTIME_DIR");

  // Force software rendering for absolute stability in this environment
  QString safeCommand = QString("env WAYLAND_DISPLAY=%1 XDG_RUNTIME_DIR=%2 "
                                "LIBGL_ALWAYS_SOFTWARE=1 %3")
                            .arg(QString::fromUtf8(waylandDisplay))
                            .arg(QString::fromUtf8(runtimeDir))
                            .arg(command);

  // START PROCESS TRACKING AND SIMULATION
  QProcess *proc = new QProcess(this);

  // Tag the process for identification
  proc->setProperty("appName", item["name"]);
  proc->setProperty("appId", QString("vitusos.app.") +
                                 item["name"].toString().toLower());

  connect(proc, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
          this, &DockController::onProcessFinished);

  proc->start("/bin/sh", {"-c", safeCommand});
  if (proc->waitForStarted()) {
    qDebug() << "[DockController] Launched PID:" << proc->processId();
    m_processes[proc->processId()] = proc;

    // SIMULATE WINDOW CREATION (Since IPC is broken)
    auto *desktop = OpenSEF::OSFDesktop::shared();
    if (desktop && desktop->stateManager()) {
      QString appId = proc->property("appId").toString();
      QString title = item["name"].toString();

      auto *win = new OpenSEF::OSFWindow(appId.toStdString(), // ID
                                         title.toStdString(), // Title
                                         appId.toStdString()  // AppID
      );

      desktop->stateManager()->addWindow(win);

      // Notify system
      OpenSEF::OSFEvent event;
      event.set("id", appId.toStdString());
      desktop->eventBus()->publish(OpenSEF::OSFEventBus::WINDOW_CREATED, event);
    }
  } else {
    qDebug() << "[DockController] Launch failed";
    proc->deleteLater();
  }
}

void DockController::onProcessFinished(int exitCode, int exitStatus) {
  auto *proc = qobject_cast<QProcess *>(sender());
  if (!proc)
    return;

  qDebug() << "[DockController] Process finished:" << proc->property("appName");

  // SIMULATE WINDOW DESTRUCTION
  auto *desktop = OpenSEF::OSFDesktop::shared();
  if (desktop && desktop->stateManager()) {
    QString appId = proc->property("appId").toString();

    desktop->stateManager()->removeWindow(appId.toStdString());

    // Notify system
    OpenSEF::OSFEvent event;
    event.set("id", appId.toStdString());
    desktop->eventBus()->publish(OpenSEF::OSFEventBus::WINDOW_DESTROYED, event);
  }

  m_processes.remove(proc->processId());
  proc->deleteLater();
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
  // Autohide logic place holder
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
