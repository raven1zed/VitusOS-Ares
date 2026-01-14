#include "DockController.h"
#include "OSFDesktop.h"
#include "OSFEventBus.h"
#include "OSFStateManager.h"
#include <QDebug>
#include <QDir>
#include <QList>
#include <QMap>
#include <QProcess>
#include <QString>
#include <QTimer>
#include <QVariantMap>

DockController::DockController(QObject *parent)
    : QObject(parent), m_hoveredIndex(-1), m_isHidden(false) {
  initDockItems();
  connectToFramework();

  // DISABLED: Polling causes flickering and erratic behavior
  // QTimer *timer = new QTimer(this);
  // connect(timer, &QTimer::timeout, this, &DockController::checkOverlap);
  // timer->start(500);
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
        // This bridges the gap for "Native" apps that might start
        // background first
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

  for (int i = 0; i < m_dockItems.size(); ++i) {
    QVariantMap item = m_dockItems[i].toMap();
    bool isRunning = false;
    QString cmdBasename = item["name"].toString().toLower();

    // FORCE FILER ALWAYS ACTIVE (like macOS Finder)
    if (cmdBasename == "filer") {
      isRunning = true;
    } else {
      // Check if app has windows
      for (auto *win : windows) {
        if (!win)
          continue;
        QString appId = QString::fromStdString(win->appId()).toLower();
        QString title = QString::fromStdString(win->title()).toLower();

        if (appId.contains(cmdBasename) || cmdBasename.contains(appId) ||
            title.contains(cmdBasename) ||
            (cmdBasename == "terminal" && appId.contains("terminal"))) {
          isRunning = true;
          break;
        }
      }
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

  // Environment pass-through for WSLg consistency and Qt Loading
  const QByteArray waylandDisplay = qgetenv("WAYLAND_DISPLAY");
  const QByteArray runtimeDir = qgetenv("XDG_RUNTIME_DIR");
  const QByteArray qtPluginPath = qgetenv("QT_PLUGIN_PATH");
  const QByteArray qmlImportPath = qgetenv("QML2_IMPORT_PATH");

  // Force hardware rendering (remove software override)
  // AND explicitly pass Qt paths that might be lost in shell transition
  QString safeCommand =
      QString("env WAYLAND_DISPLAY=%1 XDG_RUNTIME_DIR=%2 "
              "QT_PLUGIN_PATH=%3 QML2_IMPORT_PATH=%4 QML_IMPORT_PATH=%4 "
              "%5")
          .arg(QString::fromUtf8(waylandDisplay))
          .arg(QString::fromUtf8(runtimeDir))
          .arg(QString::fromUtf8(qtPluginPath))
          .arg(QString::fromUtf8(qmlImportPath))
          .arg(command);

  // START PROCESS TRACKING AND SIMULATION
  QProcess *proc = new QProcess(this);

  // CRITICAL DEBUGGING: Log output to file
  proc->setStandardOutputFile("/tmp/vitus_child_out.log");
  proc->setStandardErrorFile("/tmp/vitus_child_err.log");

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
      desktop->stateManager()->setActiveWindow(win);

      // Notify system
      OpenSEF::OSFEvent event;
      event.set("window_id", appId.toStdString());
      event.set("title", title.toStdString());
      event.set("app_id", appId.toStdString());
      event.set("menu_service", std::string(""));
      event.set("menu_path", std::string(""));
      desktop->eventBus()->publish(OpenSEF::OSFEventBus::WINDOW_FOCUSED, event);
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

    // Notify system (Restore Desktop Focus)
    OpenSEF::OSFEvent event;
    event.set("window_id", std::string("desktop"));
    event.set("title", std::string("Ares Desktop"));
    event.set("app_id", std::string(""));
    event.set("menu_service", std::string(""));
    event.set("menu_path", std::string(""));
    desktop->eventBus()->publish(OpenSEF::OSFEventBus::WINDOW_FOCUSED, event);

    OpenSEF::OSFEvent destroyEvent;
    destroyEvent.set("id", appId.toStdString());
    desktop->eventBus()->publish(OpenSEF::OSFEventBus::WINDOW_DESTROYED,
                                 destroyEvent);
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
  auto *desktop = OpenSEF::OSFDesktop::shared();
  if (!desktop || !desktop->stateManager())
    return;

  auto windows = desktop->stateManager()->allWindows();
  bool overlaps = false;

  // Dock Area Definition (1920x1080)
  const int SCREEN_HEIGHT = 1080;
  const int DOCK_HEIGHT = 90;
  const int DOCK_THRESHOLD_Y = SCREEN_HEIGHT - DOCK_HEIGHT - 20;

  for (auto *win : windows) {
    if (!win || win->isMinimized())
      continue;

    bool possiblyOverlaps = false;

    // 1. Maximized windows always overlap
    if (win->isMaximized())
      possiblyOverlaps = true;

    // 2. Focused Filer always overlaps (Assumed centered/large in Ares)
    if (win->appId().find("filer") != std::string::npos && win->isFocused()) {
      possiblyOverlaps = true;
    }

    // 3. Fallback: Any window in the bottom 20% of the screen
    if (win->y() + win->height() > DOCK_THRESHOLD_Y) {
      possiblyOverlaps = true;
    }

    if (possiblyOverlaps) {
      overlaps = true;
      break;
    }
  }

  if (overlaps) {
    hideDock();
  } else {
    // Reveal dock if NO overlaps
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
