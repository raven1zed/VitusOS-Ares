#include "PanelController.h"
#include <QDateTime>
#include <QDebug>
#include <QTimer>


// Framework integration (will connect via DBus or shared memory)
// #include "OSFDesktop.h"
// #include "OSFEventBus.h"

PanelController::PanelController(QObject *parent) : QObject(parent) {
  // Initialize clock update timer
  QTimer *clockTimer = new QTimer(this);
  connect(clockTimer, &QTimer::timeout, this, &PanelController::updateClock);
  clockTimer->start(1000); // Update every second

  // Initial clock update
  updateClock();

  // Connect to framework for window events
  connectToFramework();

  // Set initial title
  m_activeWindowTitle = "VitusOS Ares";
  emit activeWindowTitleChanged();
}

void PanelController::connectToFramework() {
  // TODO: Connect to OSFEventBus via DBus or direct linkage
  // For now, we'll simulate events

  // In production, this would subscribe to:
  // - window.focused
  // - window.title_changed
  // - application.launched
  // - application.closed

  qDebug() << "[PanelController] Connecting to framework...";
}

void PanelController::updateClock() {
  QString time = QDateTime::currentDateTime().toString("h:mm AP");
  if (time != m_currentTime) {
    m_currentTime = time;
    emit currentTimeChanged();
  }
}

void PanelController::toggleMultitask() {
  m_multitaskActive = !m_multitaskActive;
  emit multitaskActiveChanged();

  qDebug() << "[PanelController] Multitask toggled:" << m_multitaskActive;

  // TODO: Notify compositor to enter/exit multitask view
}

void PanelController::menuItemClicked(int menuIndex, int itemIndex) {
  qDebug() << "[PanelController] Menu item clicked:" << menuIndex << itemIndex;

  // TODO: Execute DBusMenu action
}

void PanelController::showMenu(int menuIndex) {
  qDebug() << "[PanelController] Show menu:" << menuIndex;

  // TODO: Expand DBusMenu dropdown
}

void PanelController::hideMenu() { qDebug() << "[PanelController] Hide menu"; }

void PanelController::onWindowFocused(const QString &windowId,
                                      const QString &title,
                                      const QString &appId) {
  m_activeWindowTitle = title;
  m_activeAppId = appId;

  emit activeWindowTitleChanged();
  emit activeAppIdChanged();

  // Load global menu for new app
  loadMenuForWindow(appId);
}

void PanelController::loadMenuForWindow(const QString &appId) {
  // TODO: Query DBusMenu for this app
  // For now, create static menu structure

  m_globalMenuItems.clear();

  // Filer menu (always present as default)
  QVariantMap filerMenu;
  filerMenu["title"] = "Filer";
  filerMenu["items"] = QVariantList{
      QVariantMap{{"label", "New Window"}, {"action", "new_window"}},
      QVariantMap{{"label", "New Tab"}, {"action", "new_tab"}},
      QVariantMap{{"separator", true}},
      QVariantMap{{"label", "Open..."}, {"action", "open"}},
      QVariantMap{{"separator", true}},
      QVariantMap{{"label", "Close"}, {"action", "close"}}};
  m_globalMenuItems.append(filerMenu);

  // Edit menu
  QVariantMap editMenu;
  editMenu["title"] = "Edit";
  editMenu["items"] =
      QVariantList{QVariantMap{{"label", "Undo"}, {"action", "undo"}},
                   QVariantMap{{"label", "Redo"}, {"action", "redo"}},
                   QVariantMap{{"separator", true}},
                   QVariantMap{{"label", "Cut"}, {"action", "cut"}},
                   QVariantMap{{"label", "Copy"}, {"action", "copy"}},
                   QVariantMap{{"label", "Paste"}, {"action", "paste"}}};
  m_globalMenuItems.append(editMenu);

  // View menu
  QVariantMap viewMenu;
  viewMenu["title"] = "View";
  viewMenu["items"] = QVariantList{
      QVariantMap{{"label", "Icons"}, {"action", "view_icons"}},
      QVariantMap{{"label", "List"}, {"action", "view_list"}},
      QVariantMap{{"label", "Columns"}, {"action", "view_columns"}},
      QVariantMap{{"separator", true}},
      QVariantMap{{"label", "Show Hidden Files"}, {"action", "show_hidden"}}};
  m_globalMenuItems.append(viewMenu);

  // Help menu
  QVariantMap helpMenu;
  helpMenu["title"] = "Help";
  helpMenu["items"] = QVariantList{
      QVariantMap{{"label", "VitusOS Help"}, {"action", "help"}},
      QVariantMap{{"separator", true}},
      QVariantMap{{"label", "About VitusOS"}, {"action", "about"}}};
  m_globalMenuItems.append(helpMenu);

  emit globalMenuItemsChanged();
}
