#include "PanelController.h"
#include <OSFDesktop.h>
#include <OSFEventBus.h>
#include <QDateTime>
#include <QDebug>
#include <QMetaObject>
#include <QTimer>

PanelController::PanelController(QObject *parent) : QObject(parent) {
  // Initialize clock update timer
  QTimer *clockTimer = new QTimer(this);
  connect(clockTimer, &QTimer::timeout, this, &PanelController::updateClock);
  clockTimer->start(1000); // Update every second

  // Initial clock update
  updateClock();

  // Connect to framework for window events
  connectToFramework();

  // Start with empty title (will update when window focused)
  m_activeWindowTitle = "";
  emit activeWindowTitleChanged();

  loadDefaultMenu();
}

PanelController::~PanelController() {}

void PanelController::connectToFramework() {
  qDebug() << "[PanelController] Subscribing to OSFEventBus...";

  auto *eventBus = OpenSEF::OSFDesktop::shared()->eventBus();

  // Subscribe to window focus events
  eventBus->subscribe(
      OpenSEF::OSFEventBus::WINDOW_FOCUSED, [this](const OpenSEF::OSFEvent &e) {
        QString title = QString::fromStdString(e.get<std::string>("title"));
        QString appId = QString::fromStdString(e.get<std::string>("app_id"));
        QString windowId =
            QString::fromStdString(e.get<std::string>("window_id"));

        // Update on UI thread
        QMetaObject::invokeMethod(this, [this, title, appId, windowId]() {
          onWindowFocused(windowId, title, appId);
        });
      });

  qDebug() << "[PanelController] Success: Connected to OSF framework.";
}

void PanelController::updateClock() {
  QDateTime now = QDateTime::currentDateTime();
  // Minimal clock per Design Reference: HH:mm only
  QString time = now.toString("HH:mm");
  if (time != m_currentTime) {
    m_currentTime = time;
    emit currentTimeChanged();
  }
}

void PanelController::toggleMultitask() {
  m_multitaskActive = !m_multitaskActive;
  emit multitaskActiveChanged();

  qDebug() << "[PanelController] Multitask toggled:" << m_multitaskActive;
}

void PanelController::menuItemClicked(int menuIndex, int itemIndex) {
  qDebug() << "[PanelController] Menu item clicked:" << menuIndex << itemIndex;

  // TODO: When native OSFMenuManager API is ready:
  // auto *menuMgr = OpenSEF::OSFDesktop::shared()->menuManager();
  // menuMgr->activateItem(menuIndex, itemIndex);
}

void PanelController::showMenu(int menuIndex, int x, int y) {
  qDebug() << "[PanelController] Show menu:" << menuIndex << "at" << x << y;
  emit menuRequested(menuIndex, x, y);
}

void PanelController::hideMenu() { qDebug() << "[PanelController] Hide menu"; }

void PanelController::onWindowFocused(const QString &windowId,
                                      const QString &title,
                                      const QString &appId) {
  qDebug() << "[PanelController] Window focused:" << title << "(" << appId
           << ")";

  // Update window title
  if (title != m_activeWindowTitle) {
    m_activeWindowTitle = title;
    emit activeWindowTitleChanged();
  }

  // Update app ID
  if (appId != m_activeAppId) {
    m_activeAppId = appId;
    emit activeAppIdChanged();
  }

  // TODO: Load menu for this app from native OSFMenuManager
  // For now: keep default menu
}

void PanelController::loadDefaultMenu() {
  // Default menu for VitusOS shell
  QVariantList menus;

  // Filer menu
  QVariantMap filerMenu;
  filerMenu["label"] = "Filer";
  QVariantList filerItems;
  filerItems.append(QVariantMap{{"label", "New Window"}, {"id", 1}});
  filerItems.append(QVariantMap{{"label", "New Folder"}, {"id", 2}});
  filerItems.append(QVariantMap{{"separator", true}});
  filerItems.append(QVariantMap{{"label", "Preferences"}, {"id", 3}});
  filerMenu["items"] = filerItems;
  menus.append(filerMenu);

  // Menu menu
  QVariantMap menuMenu;
  menuMenu["label"] = "Menu";
  QVariantList menuItems;
  menuItems.append(QVariantMap{{"label", "About VitusOS"}, {"id", 10}});
  menuItems.append(QVariantMap{{"separator", true}});
  menuItems.append(QVariantMap{{"label", "Settings"}, {"id", 11}});
  menuMenu["items"] = menuItems;
  menus.append(menuMenu);

  // Settings menu
  QVariantMap settingsMenu;
  settingsMenu["label"] = "Settings";
  QVariantList settingsItems;
  settingsItems.append(QVariantMap{{"label", "Display"}, {"id", 20}});
  settingsItems.append(QVariantMap{{"label", "Network"}, {"id", 21}});
  settingsItems.append(QVariantMap{{"label", "Sound"}, {"id", 22}});
  settingsMenu["items"] = settingsItems;
  menus.append(settingsMenu);

  // Help menu
  QVariantMap helpMenu;
  helpMenu["label"] = "Help";
  QVariantList helpItems;
  helpItems.append(QVariantMap{{"label", "Documentation"}, {"id", 30}});
  helpItems.append(QVariantMap{{"label", "Report Bug"}, {"id", 31}});
  helpMenu["items"] = helpItems;
  menus.append(helpMenu);

  m_globalMenuItems = menus;
  emit globalMenuItemsChanged();

  qDebug() << "[PanelController] Loaded default menu (no KDE dependencies)";
}
