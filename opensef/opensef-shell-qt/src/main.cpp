/**
 * main.cpp - Qt Quick Shell Entry Point
 *
 * VitusOS Ares Desktop Shell
 * GPU-accelerated UI with Vulkan/OpenGL rendering
 */

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include "DockController.h"
#include "IconProvider.h"
#include "MultitaskController.h"
#include "PanelController.h"
#include "SystemTrayController.h"

int main(int argc, char *argv[]) {
  // Enable Vulkan or OpenGL backend via Qt Quick
  // Qt automatically uses QRhi which selects Vulkan when available
  qputenv("QSG_RHI_BACKEND", "vulkan");

  QGuiApplication app(argc, argv);
  app.setApplicationName("VitusOS Shell");
  app.setApplicationVersion("0.1.0");
  app.setOrganizationName("VitusOS");

  // Use material or fusion style for modern look
  QQuickStyle::setStyle("Material");

  // Create controllers
  PanelController panelController;
  DockController dockController;
  MultitaskController multitaskController;
  SystemTrayController systemTrayController;

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

  // Load main QML
  const QUrl url(QStringLiteral("qrc:/qml/Main.qml"));
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
