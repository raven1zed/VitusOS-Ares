/**
 * Filer - Native openSEF File Manager
 *
 * First native app using C++ NSButton API (no Cairo, pure Qt Quick rendering)
 */

#include <QDebug>
#include <QDir>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickNSButton.h>
#include <opensef/NSButton.h>

// OpenSEF Native Framework
#include <OSFDesktop.h>
#include <OSFEventBus.h>

using namespace OpenSEF;

// Register our C++ types with QML
void registerTypes() {
  qmlRegisterType<QQuickNSButton>("OpenSEF.AppKit", 1, 0, "NSButton");
}

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);
  app.setApplicationName("Filer");
  app.setOrganizationName("VitusOS");
  app.setQuitOnLastWindowClosed(false); // Finder behavior: App stays running

  // --- OpenSEF Native Integration ---
  // Initialize the native desktop bridge
  auto *desktop = OSFDesktop::shared();
  desktop->initialize();

  // Announce Launch (Native App Protocol)
  OSFEvent launchEvent;
  launchEvent.set("appId", std::string("osf-filer-native"));
  launchEvent.set("name", std::string("Filer"));
  launchEvent.set("pid", (int)QCoreApplication::applicationPid());
  desktop->eventBus()->publish(OSFEventBus::APP_LAUNCHED, launchEvent);

  // Register C++ AppKit types
  registerTypes();

  QQmlApplicationEngine engine;

  // Verbose QML diagnostic
  QObject::connect(&engine, &QQmlApplicationEngine::warnings,
                   [](const QList<QQmlError> &warnings) {
                     for (const auto &w : warnings)
                       qWarning() << "[Filer QML Warning]" << w.toString();
                   });

  qDebug() << "[Filer] Searching for QML in paths...";
  QString buildDir = QCoreApplication::applicationDirPath();
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
      qDebug() << "[Filer] Selected path:" << path;
      break;
    }
  }

  if (url.isEmpty()) {
    qCritical() << "[Filer] FATAL: Filer.qml not found!";
    return -1;
  }

  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) {
          qCritical() << "[Filer] FATAL: QML object creation failed for" << url;
          QCoreApplication::exit(-1);
        } else {
          qDebug() << "[Filer] SUCCESS: QML Window created.";
        }
      },
      Qt::QueuedConnection);

  engine.load(url);

  if (engine.rootObjects().isEmpty()) {
    qCritical() << "[Filer] FATAL: No root objects loaded after engine.load()";
    return -1;
  }

  int ret = app.exec();

  // Announce Termination
  OSFEvent closeEvent;
  closeEvent.set("appId", std::string("osf-filer-native"));
  desktop->eventBus()->publish(OSFEventBus::APP_TERMINATED, closeEvent);

  return ret;
}
