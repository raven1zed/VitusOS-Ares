/**
 * main.cpp - Qt Quick Shell Entry Point
 *
 * VitusOS Ares Desktop Shell
 * GPU-accelerated UI with Vulkan/OpenGL rendering
 */

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QFontDatabase>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include "DockController.h"
#include "IconProvider.h"
#include "MultitaskController.h"
#include "PanelController.h"
#include "SystemTrayController.h"
#include <OSFDesktop.h>

// Import GNUstep Bridge
#include <QQuickNSButton.h>
#include <QQuickWindowButton.h>
#include <opensef/NSButton.h>

// Register types for QML
void registerTypes() {
  qmlRegisterType<QQuickNSButton>("OpenSEF.AppKit", 1, 0, "NSButton");
  qmlRegisterType<QQuickWindowButton>("OpenSEF.AppKit", 1, 0, "NSWindowButton");
}

int main(int argc, char *argv[]) {
  // Enable Vulkan or OpenGL backend via Qt Quick
  // Qt automatically uses QRhi which selects Vulkan when available
  qputenv("QSG_RHI_BACKEND", "vulkan");

  // DEBUG: Verify binary version
  fprintf(
      stderr,
      "=== OSF SHELL QT V2 STARTING - BUILD_ID: VERIFY_FRAMEWORK_LIVE ===\n");
  fflush(stderr);

  QGuiApplication app(argc, argv);
  app.setApplicationName("VitusOS Shell");
  app.setApplicationVersion("0.1.0");
  app.setOrganizationName("VitusOS");

  // Set app ID for Wayland (so compositor recognizes shell window)
  app.setDesktopFileName("vitusos.shell");

  // Use material or fusion style for modern look
  QQuickStyle::setStyle("Material");

  // Load Inter Font
  QString fontPath =
      QDir::cleanPath(QCoreApplication::applicationDirPath() +
                      "/../../../assets/fonts/Inter-Regular.ttf");
  int fontId = QFontDatabase::addApplicationFont(fontPath);
  if (fontId != -1) {
    QFont font("Inter");
    app.setFont(font);
    qDebug() << "Successfully loaded Inter font from" << fontPath;
  } else {
    qCritical() << "CRITICAL: Failed to load Inter font from" << fontPath;
  }

  // Initialize OpenSEF Framework
  OpenSEF::OSFDesktop::shared()->initialize();

  // Create controllers
  PanelController panelController;
  DockController dockController;
  MultitaskController multitaskController;
  SystemTrayController systemTrayController;

  // Register C++ AppKit types (for WindowDecorations in Multitask view)
  registerTypes();

  // Set up QML engine
  QQmlApplicationEngine engine;

  // Register image providers
  engine.addImageProvider(QLatin1String("icon"), new IconProvider());

  // Expose controllers to QML
  QQmlContext *context = engine.rootContext();
  context->setContextProperty("panelController", &panelController);
  context->setContextProperty("dockController", &dockController);
  context->setContextProperty("multitaskController", &multitaskController);
  context->setContextProperty("systemTrayController", &systemTrayController);

  // Load main QML from FILESYSTEM (bypass QRC caching issue)
  // This ensures we ALWAYS use the latest QML on disk
  // Determine the QML path relative to the executable for development
  QString qmlPath = QCoreApplication::applicationDirPath() +
                    "/../../opensef-shell-qt/qml/Shell.qml";
  const QUrl url = QUrl::fromLocalFile(QDir::cleanPath(qmlPath));

  qDebug() << "Loading QML from root file:" << url.toLocalFile();

  // Ensure the engine knows where to find other QML files in the same directory
  engine.addImportPath(QFileInfo(qmlPath).absolutePath());

  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);

  engine.load(url);

  return app.exec();
}
