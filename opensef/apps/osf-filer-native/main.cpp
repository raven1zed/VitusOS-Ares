/**
 * Filer - Native openSEF File Manager
 *
 * First native app using C++ NSButton API (no Cairo, pure Qt Quick rendering)
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

using namespace OpenSEF;

// Register our C++ types with QML
void registerTypes() {
  qmlRegisterType<QQuickNSButton>("OpenSEF.AppKit", 1, 0, "NSButton");
  qmlRegisterType<QQuickWindowButton>("OpenSEF.AppKit", 1, 0, "NSWindowButton");
}

// OpenSEF AppKit Application Class (GNUstep C++ Fork pattern)
class FilerApplication : public QObject {
  Q_OBJECT
public:
  FilerApplication(int &argc, char **argv) : app(argc, argv) {}

  int run() {
    // Enforce Alpha Buffer for Transparent Windows (Rounded Corners)
    QQuickWindow::setDefaultAlphaBuffer(true);
    // qputenv("QSG_RHI_BACKEND", "vulkan"); // Let Qt decide (fixes WSL2 black
    // screen)

    app.setApplicationName("Filer");
    app.setOrganizationName("VitusOS");
    app.setQuitOnLastWindowClosed(false);

    // Load Inter Font (Ares Design Standard)
    // From build/apps/osf-filer-native/osf-filer-native -> ../../../../assets
    QString fontPath =
        QDir::cleanPath(QCoreApplication::applicationDirPath() +
                        "/../../../../assets/fonts/Inter-Regular.ttf");
    if (!QFile::exists(fontPath)) {
      fontPath = "/mnt/c/Users/hp/Documents/VitusOS-Ares/assets/fonts/"
                 "Inter-Regular.ttf";
    }
    int fontId = QFontDatabase::addApplicationFont(fontPath);
    if (fontId != -1) {
      QFont font("Inter");
      app.setFont(font);
      qDebug() << "[Filer] Successfully loaded Inter font from" << fontPath;
    } else {
      qCritical() << "[Filer] CRITICAL: Failed to load Inter font from"
                  << fontPath;
    }

    // --- OpenSEF Native Integration ---
    auto *desktop = OSFDesktop::shared();
    desktop->initialize();

    // Announce Launch
    OSFEvent launchEvent;
    launchEvent.set("appId", std::string("osf-filer-native"));
    launchEvent.set("name", std::string("Filer"));
    desktop->eventBus()->publish(OSFEventBus::APP_LAUNCHED, launchEvent);

    registerTypes();
    applicationDidFinishLaunching();

    int ret = app.exec();

    // Announce Termination
    OSFEvent closeEvent;
    closeEvent.set("appId", std::string("osf-filer-native"));
    desktop->eventBus()->publish(OSFEventBus::APP_TERMINATED, closeEvent);

    return ret;
  }

protected:
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

    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objUrl) {
          if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
          else
            qDebug() << "[Filer] SUCCESS: QML Window created.";
        },
        Qt::QueuedConnection);

    engine.load(url);
  }

private:
  QGuiApplication app;
  QQmlApplicationEngine engine;
};

int main(int argc, char *argv[]) {
  FilerApplication filer(argc, argv);
  return filer.run();
}

#include "main.moc"
