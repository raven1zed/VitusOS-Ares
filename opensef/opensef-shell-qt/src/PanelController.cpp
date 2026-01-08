#include "PanelController.h"
#include "DBusMenuImporter.h"
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDateTime>
#include <QDebug>
#include <QTimer>

// DBusMenu registrar service name
static const QString APPMENU_REGISTRAR_SERVICE =
    "com.canonical.AppMenu.Registrar";
static const QString APPMENU_REGISTRAR_PATH =
    "/com/canonical/AppMenu/Registrar";
static const QString APPMENU_REGISTRAR_INTERFACE =
    "com.canonical.AppMenu.Registrar";

PanelController::PanelController(QObject *parent) : QObject(parent) {
  // Initialize clock update timer
  QTimer *clockTimer = new QTimer(this);
  connect(clockTimer, &QTimer::timeout, this, &PanelController::updateClock);
  clockTimer->start(1000); // Update every second

  // Initial clock update
  updateClock();

  // Connect to framework for window events
  connectToFramework();

  // Set initial title and default menu
  m_activeWindowTitle = "VitusOS Ares";
  emit activeWindowTitleChanged();

  loadDefaultMenu();
}

PanelController::~PanelController() { delete m_menuImporter; }

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

  // Execute DBusMenu action if we have an importer
  if (m_menuImporter && m_menuImporter->isValid()) {
    // Get the item ID from our menu structure
    if (menuIndex >= 0 && menuIndex < m_globalMenuItems.size()) {
      QVariantMap menu = m_globalMenuItems[menuIndex].toMap();
      QVariantList items = menu["items"].toList();
      if (itemIndex >= 0 && itemIndex < items.size()) {
        QVariantMap item = items[itemIndex].toMap();
        int id = item["id"].toInt();
        if (id > 0) {
          m_menuImporter->activateItem(id);
          return;
        }
      }
    }
  }

  // Fallback: log the action
  qDebug() << "[PanelController] No DBusMenu action available for item";
}

void PanelController::showMenu(int menuIndex) {
  qDebug() << "[PanelController] Show menu:" << menuIndex;
  // Menu expansion is handled by QML
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
  // Query the AppMenu Registrar for this window's menu
  QDBusInterface registrar(APPMENU_REGISTRAR_SERVICE, APPMENU_REGISTRAR_PATH,
                           APPMENU_REGISTRAR_INTERFACE,
                           QDBusConnection::sessionBus());

  if (!registrar.isValid()) {
    qDebug() << "[PanelController] AppMenu Registrar not available, using "
                "default menu";
    loadDefaultMenu();
    return;
  }

  // GetMenuForWindow(uint windowId) -> (service, path)
  // For now, we don't have the actual window ID, so we'll use a placeholder
  // In production, this would come from the compositor

  // TODO: Get actual window ID from framework
  // For now, load default menu
  loadDefaultMenu();
}

void PanelController::connectToDBusMenu(const QString &service,
                                        const QString &path) {
  // Clean up existing importer
  if (m_menuImporter) {
    disconnect(m_menuImporter, nullptr, this, nullptr);
    delete m_menuImporter;
    m_menuImporter = nullptr;
  }

  if (service.isEmpty() || path.isEmpty()) {
    loadDefaultMenu();
    return;
  }

  m_currentMenuService = service;
  m_currentMenuPath = path;

  // Create new importer
  m_menuImporter = new DBusMenuImporter(service, path, this);

  if (m_menuImporter->isValid()) {
    connect(m_menuImporter, &DBusMenuImporter::menuUpdated, this,
            &PanelController::onMenuUpdated);

    qDebug() << "[PanelController] Connected to DBusMenu:" << service << path;

    // Trigger initial menu load
    m_menuImporter->refresh();
  } else {
    qDebug()
        << "[PanelController] Failed to connect to DBusMenu, using default";
    delete m_menuImporter;
    m_menuImporter = nullptr;
    loadDefaultMenu();
  }
}

void PanelController::onMenuUpdated() {
  if (!m_menuImporter)
    return;

  // Convert DBusMenu items to our QVariantList format
  QVariantList menuItems = m_menuImporter->menuItems();

  // Group items by parent (0 = root menu items)
  QMap<int, QVariantList> itemsByParent;
  QMap<int, QString> menuTitles;

  for (const QVariant &item : menuItems) {
    QVariantMap itemMap = item.toMap();
    int parentId = itemMap["parentId"].toInt();

    if (parentId == 0) {
      // Root level = menu title
      menuTitles[itemMap["id"].toInt()] = itemMap["label"].toString();
      itemsByParent[itemMap["id"].toInt()] = QVariantList();
    } else {
      // Child item
      itemsByParent[parentId].append(itemMap);
    }
  }

  // Build menu structure
  m_globalMenuItems.clear();

  for (auto it = menuTitles.begin(); it != menuTitles.end(); ++it) {
    QVariantMap menu;
    menu["title"] = it.value();
    menu["items"] = itemsByParent[it.key()];
    m_globalMenuItems.append(menu);
  }

  emit globalMenuItemsChanged();
  qDebug() << "[PanelController] Menu updated with" << m_globalMenuItems.size()
           << "menus";
}

void PanelController::loadDefaultMenu() {
  m_globalMenuItems.clear();

  // Filer menu (always present as default)
  QVariantMap filerMenu;
  filerMenu["title"] = "Filer";
  filerMenu["items"] = QVariantList{
      QVariantMap{{"label", "New Window"}, {"action", "new_window"}, {"id", 0}},
      QVariantMap{{"label", "New Tab"}, {"action", "new_tab"}, {"id", 0}},
      QVariantMap{{"separator", true}},
      QVariantMap{{"label", "Open..."}, {"action", "open"}, {"id", 0}},
      QVariantMap{{"separator", true}},
      QVariantMap{{"label", "Close"}, {"action", "close"}, {"id", 0}}};
  m_globalMenuItems.append(filerMenu);

  // Edit menu
  QVariantMap editMenu;
  editMenu["title"] = "Edit";
  editMenu["items"] = QVariantList{
      QVariantMap{{"label", "Undo"}, {"action", "undo"}, {"id", 0}},
      QVariantMap{{"label", "Redo"}, {"action", "redo"}, {"id", 0}},
      QVariantMap{{"separator", true}},
      QVariantMap{{"label", "Cut"}, {"action", "cut"}, {"id", 0}},
      QVariantMap{{"label", "Copy"}, {"action", "copy"}, {"id", 0}},
      QVariantMap{{"label", "Paste"}, {"action", "paste"}, {"id", 0}}};
  m_globalMenuItems.append(editMenu);

  // View menu
  QVariantMap viewMenu;
  viewMenu["title"] = "View";
  viewMenu["items"] = QVariantList{
      QVariantMap{{"label", "Icons"}, {"action", "view_icons"}, {"id", 0}},
      QVariantMap{{"label", "List"}, {"action", "view_list"}, {"id", 0}},
      QVariantMap{{"label", "Columns"}, {"action", "view_columns"}, {"id", 0}},
      QVariantMap{{"separator", true}},
      QVariantMap{{"label", "Show Hidden Files"},
                  {"action", "show_hidden"},
                  {"id", 0}}};
  m_globalMenuItems.append(viewMenu);

  // Help menu
  QVariantMap helpMenu;
  helpMenu["title"] = "Help";
  helpMenu["items"] = QVariantList{
      QVariantMap{{"label", "VitusOS Help"}, {"action", "help"}, {"id", 0}},
      QVariantMap{{"separator", true}},
      QVariantMap{{"label", "About VitusOS"}, {"action", "about"}, {"id", 0}}};
  m_globalMenuItems.append(helpMenu);

  emit globalMenuItemsChanged();
}
