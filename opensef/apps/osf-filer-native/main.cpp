/**
 * Filer - Native openSEF File Manager (Always-Running Service)
 *
 * First native app using C++ NSButton API (no Cairo, pure Qt Quick rendering)
 *
 * Key Features:
 * - Always running (like macOS Finder)
 * - Hosts Pathfinder (universal search)
 * - Responds to activation requests
 * - Template for future native openSEF apps
 */

#include <QDebug>
#include <QDir>
#include <QFontDatabase>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickNSButton.h>
#include <QQuickWindow>
#include <QQuickWindowButton.h>
#include <opensef/NSButton.h>

// OpenSEF Native Framework
#include <opensef/OSFDesktop.h>
#include <opensef/OSFEventBus.h>
#include <opensef/OSFPathfinder.h>

using namespace OpenSEF;

// Register our C++ types with QML
void registerTypes() {
  qmlRegisterType<QQuickNSButton>("OpenSEF.AppKit", 1, 0, "NSButton");
  qmlRegisterType<QQuickWindowButton>("OpenSEF.AppKit", 1, 0, "NSWindowButton");
}

/**
 * FilerApplication - GNUstep NSApplication Pattern (C++ Port)
 *
 * This is the always-running service that hosts:
 * - File management windows
 * - Pathfinder (universal search)
 * - Clipboard history integration
 */
class FilerApplication : public QObject {
  Q_OBJECT
public:
  FilerApplication(int &argc, char **argv) : app(argc, argv) {}

  int run() {
    // Enforce Alpha Buffer for Transparent Windows (Rounded Corners)
    QQuickWindow::setDefaultAlphaBuffer(true);

    app.setApplicationName("Filer");
    app.setOrganizationName("VitusOS");

    // CRITICAL: Do NOT quit when last window closes - Filer is always running
    app.setQuitOnLastWindowClosed(false);

    // Load Inter Font (Ares Design Standard)
    loadInterFont();

    // --- OpenSEF Native Integration ---
    auto *desktop = OSFDesktop::shared();
    desktop->initialize();

    // Announce Service Startup (not just app launch)
    OSFEvent startEvent;
    startEvent.set("serviceId", std::string("filer"));
    startEvent.set("name", std::string("Filer Service"));
    startEvent.set("type", std::string("always-running"));
    desktop->eventBus()->publish(OSFEventBus::SERVICE_STARTED, startEvent);

    // Subscribe to activation requests
    subscribeToEvents(desktop);

    registerTypes();
    applicationDidFinishLaunching();

    int ret = app.exec();

    // Announce Service Shutdown
    OSFEvent stopEvent;
    stopEvent.set("serviceId", std::string("filer"));
    desktop->eventBus()->publish(OSFEventBus::SERVICE_STOPPED, stopEvent);

    return ret;
  }

  // Open new Filer window (can be called via EventBus)
  Q_INVOKABLE void openNewWindow() {
    qDebug() << "[Filer] Opening new window";
    applicationDidFinishLaunching();
  }

  // Show Pathfinder (Cmd+Space handler delegates here)
  Q_INVOKABLE void showPathfinder() {
    qDebug() << "[Filer] Showing Pathfinder";
    Pathfinder::shared().show();
  }

  Q_INVOKABLE void hidePathfinder() { Pathfinder::shared().hide(); }

  Q_INVOKABLE void togglePathfinder() { Pathfinder::shared().toggle(); }

protected:
  void subscribeToEvents(OSFDesktop *desktop) {
    // Listen for Filer activation requests
    desktop->eventBus()->subscribe(
        OSFEventBus::APP_ACTIVATED,
        [this](const OSFEvent &event) {
          auto appId = event.getString("appId");
          if (appId == "filer" || appId == "osf-filer-native") {
            qDebug() << "[Filer] Received activation request";
            QMetaObject::invokeMethod(this, "openNewWindow",
                                      Qt::QueuedConnection);
          }
        },
        this);

    // Listen for Pathfinder toggle (Cmd+Space)
    desktop->eventBus()->subscribe(
        OSFEventBus::SHORTCUT_ACTIVATED,
        [this](const OSFEvent &event) {
          auto shortcut = event.getString("shortcut");
          if (shortcut == "cmd+space" || shortcut == "meta+space") {
            qDebug() << "[Filer] Pathfinder shortcut triggered";
            QMetaObject::invokeMethod(this, "togglePathfinder",
                                      Qt::QueuedConnection);
          }
        },
        this);
  }

  void loadInterFont() {
    QString buildDir = QCoreApplication::applicationDirPath();
    QString fontPath = QDir::cleanPath(
        buildDir + "/../../../../assets/fonts/Inter-Regular.ttf");
    if (!QFile::exists(fontPath)) {
      fontPath = "/mnt/c/Users/hp/Documents/VitusOS-Ares/assets/fonts/"
                 "Inter-Regular.ttf";
    }
    int fontId = QFontDatabase::addApplicationFont(fontPath);
    if (fontId != -1) {
      QFont font("Inter");
      app.setFont(font);
      qDebug() << "[Filer] Loaded Inter font from" << fontPath;
    } else {
      qWarning() << "[Filer] Failed to load Inter font from" << fontPath;
    }
  }

  void applicationDidFinishLaunching() {
    qDebug() << "[Filer] NSApplication-style initialization started.";

    // Determine path to Shell QML components
    QString buildDir = QCoreApplication::applicationDirPath();
    QString shellQmlPath =
        QDir::cleanPath(buildDir + "/../../opensef-shell-qt/qml");
    if (!QFile::exists(shellQmlPath)) {
      shellQmlPath =
          "/mnt/c/Users/hp/Documents/VitusOS-Ares/opensef/opensef-shell-qt/qml";
    }

    engine.addImportPath(shellQmlPath);

    QStringList searchPaths = {buildDir + "/Filer.qml",
                               buildDir +
                                   "/../../apps/osf-filer-native/Filer.qml",
                               QDir::currentPath() + "/Filer.qml",
                               "/mnt/c/Users/hp/Documents/VitusOS-Ares/opensef/"
                               "apps/osf-filer-native/Filer.qml"};

    QUrl url;
    for (const QString &path : searchPaths) {
      if (QFile::exists(path)) {
        url = QUrl::fromLocalFile(QDir::cleanPath(path));
        break;
      }
    }

    if (url.isEmpty()) {
      qCritical() << "[Filer] FATAL: Filer.qml not found!";
      return;
    }

    // Expose FilerApplication to QML for Pathfinder integration
    engine.rootContext()->setContextProperty("FilerApp", this);

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objUrl) {
          if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
          else
            qDebug() << "[Filer] SUCCESS: Window created.";
        },
        Qt::QueuedConnection);

    engine.load(url);
  }

private:
  QGuiApplication app;
  QQmlApplicationEngine engine;
};

int main(int argc, char *argv[]) {
  qDebug() << "[Filer] Starting as always-running service...";
  FilerApplication filer(argc, argv);
  return filer.run();
}

#include "main.moc"
