#include "PanelController.h"
#include <OSFDesktop.h>
#include <OSFEventBus.h>
#include <OSFStateManager.h>
#include <QDBusArgument>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusInterface>
#include <QDBusMessage>
#include <QDBusObjectPath>
#include <QDBusVariant>
#include <QDateTime>
#include <QDebug>
#include <QMetaObject>
#include <QTimer>

namespace {

struct DBusMenuNode {
  int id = 0;
  QVariantMap properties;
  QList<DBusMenuNode> children;
};

DBusMenuNode parseMenuLayout(QDBusArgument &arg) {
  DBusMenuNode node;
  arg.beginStructure();
  arg >> node.id;
  arg >> node.properties;
  arg.beginArray();
  while (!arg.atEnd()) {
    node.children.append(parseMenuLayout(arg));
  }
  arg.endArray();
  arg.endStructure();
  return node;
}

QVariantList buildMenuItems(const QList<DBusMenuNode> &nodes) {
  QVariantList items;
  for (const auto &node : nodes) {
    const QString type = node.properties.value("type").toString();
    const QString label = node.properties.value("label").toString();
    if (type == "separator" || label.isEmpty()) {
      continue;
    }

    QVariantMap item;
    item["label"] = label;
    item["enabled"] = node.properties.value("enabled", true).toBool();
    item["dbusId"] = node.id;
    items.append(item);
  }
  return items;
}

} // namespace

PanelController::PanelController(QObject *parent) : QObject(parent) {
  // Initialize clock update timer
  QTimer *clockTimer = new QTimer(this);
  connect(clockTimer, &QTimer::timeout, this, &PanelController::updateClock);
  clockTimer->start(1000); // Update every second

  // Initial clock update
  updateClock();

  // Connect to framework for window events
  connectToFramework();

  // Start with Filer menu (default idle state)
  m_activeWindowTitle = "";
  emit activeWindowTitleChanged();

  initializeDefaultMenus(); // Show Filer menu by default
}

PanelController::~PanelController() {}

void PanelController::connectToFramework() {
  qDebug() << "[PanelController] Subscribing to OSFEventBus...";

  auto *eventBus = OpenSEF::OSFDesktop::shared()->eventBus();

  // Subscribe to window focus events
  eventBus->subscribe(
      OpenSEF::OSFEventBus::WINDOW_FOCUSED, [this](const OpenSEF::OSFEvent &e) {
        QString windowId =
            QString::fromStdString(e.get<std::string>("window_id"));
        QString title = QString::fromStdString(e.get<std::string>("title"));
        QString appId = QString::fromStdString(e.get<std::string>("app_id"));
        QString menuService =
            QString::fromStdString(e.get<std::string>("menu_service"));
        QString menuPath =
            QString::fromStdString(e.get<std::string>("menu_path"));

        if (title.isEmpty() || appId.isEmpty()) {
          auto *window =
              OpenSEF::OSFDesktop::shared()->stateManager()->windowById(
                  windowId.toStdString());
          if (window) {
            if (title.isEmpty()) {
              title = QString::fromStdString(window->title());
            }
            if (appId.isEmpty()) {
              appId = QString::fromStdString(window->appId());
            }
          }
        }

        // Update on UI thread
        QMetaObject::invokeMethod(
            this, [this, title, appId, windowId, menuService, menuPath]() {
              onWindowFocused(windowId, title, appId, menuService, menuPath);
            });
      });

  qDebug() << "[PanelController] Success: Connected to OSF framework.";
}

void PanelController::updateClock() {
  QDateTime now = QDateTime::currentDateTime();
  // Standard Ares format: Monday, Jan 12 10:15 PM
  QString time = now.toString("dddd, MMM d h:mm AP");
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

  if (menuIndex < 0 || menuIndex >= m_globalMenuItems.size()) {
    return;
  }

  const QVariantMap menuMap = m_globalMenuItems.at(menuIndex).toMap();
  const QVariantList itemList = menuMap.value("items").toList();
  if (itemIndex < 0 || itemIndex >= itemList.size()) {
    return;
  }

  const QVariantMap item = itemList.at(itemIndex).toMap();
  const int dbusId = item.value("dbusId", -1).toInt();
  if (dbusId <= 0) {
    return;
  }

  if (m_menuService.isEmpty() || m_menuPath.isEmpty()) {
    qDebug() << "[PanelController] No DBus menu target for activation.";
    return;
  }

  QDBusInterface menuInterface(m_menuService, m_menuPath,
                               "com.canonical.dbusmenu",
                               QDBusConnection::sessionBus());
  if (!menuInterface.isValid()) {
    qDebug() << "[PanelController] DBus menu interface invalid for"
             << m_menuService << m_menuPath;
    return;
  }

  const quint32 timestamp =
      static_cast<quint32>(QDateTime::currentMSecsSinceEpoch() & 0xffffffff);
  QDBusMessage reply = menuInterface.call(
      "Event", dbusId, QStringLiteral("clicked"),
      QVariant::fromValue(QDBusVariant(QVariant())), timestamp);
  if (reply.type() == QDBusMessage::ErrorMessage) {
    qDebug() << "[PanelController] DBus menu activation failed:"
             << reply.errorMessage();
  }
}

void PanelController::showMenu(int menuIndex, int x, int y) {
  qDebug() << "[PanelController] Show menu:" << menuIndex << "at" << x << y;
  emit menuRequested(menuIndex, x, y);
}

void PanelController::hideMenu() { qDebug() << "[PanelController] Hide menu"; }

void PanelController::onWindowFocused(const QString &windowId,
                                      const QString &title,
                                      const QString &appId,
                                      const QString &menuService,
                                      const QString &menuPath) {
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

  loadMenuForFocusedApp(windowId, appId, menuService, menuPath);
}

void PanelController::clearMenuItems() {
  if (!m_globalMenuItems.isEmpty()) {
    m_globalMenuItems.clear();
    emit globalMenuItemsChanged();
  }
}

void PanelController::loadMenuForFocusedApp(const QString &windowId,
                                            const QString &appId,
                                            const QString &menuService,
                                            const QString &menuPath) {
  Q_UNUSED(appId);

  QString resolvedService = menuService;
  QString resolvedPath = menuPath;

  if (resolvedService.isEmpty() || resolvedPath.isEmpty()) {
    fetchMenuFromRegistrar(windowId, &resolvedService, &resolvedPath);
  }

  if (!resolvedService.isEmpty() && !resolvedPath.isEmpty()) {
    if (loadDbusMenu(resolvedService, resolvedPath)) {
      return;
    }
  }

  qDebug() << "[PanelController] No DBus menu available for focused app.";
  clearMenuItems();
}

bool PanelController::loadDbusMenu(const QString &service,
                                   const QString &path) {
  QDBusInterface menuInterface(service, path, "com.canonical.dbusmenu",
                               QDBusConnection::sessionBus());
  if (!menuInterface.isValid()) {
    qDebug() << "[PanelController] Invalid DBus menu interface:" << service
             << path;
    return false;
  }

  const QStringList properties = {"label", "enabled", "type",
                                  "children-display"};
  QDBusMessage reply = menuInterface.call("GetLayout", 0, -1, properties);
  if (reply.type() == QDBusMessage::ErrorMessage ||
      reply.arguments().size() < 2) {
    qDebug() << "[PanelController] Failed to fetch DBus menu layout:"
             << reply.errorMessage();
    return false;
  }

  QDBusArgument layoutArg =
      qvariant_cast<QDBusArgument>(reply.arguments().at(1));
  DBusMenuNode root = parseMenuLayout(layoutArg);

  QVariantList menus;
  for (const auto &menuNode : root.children) {
    const QString label = menuNode.properties.value("label").toString();
    if (label.isEmpty()) {
      continue;
    }

    QVariantMap menu;
    menu["title"] = label;
    menu["items"] = buildMenuItems(menuNode.children);
    menus.append(menu);
  }

  m_menuService = service;
  m_menuPath = path;
  m_globalMenuItems = menus;
  emit globalMenuItemsChanged();

  qDebug() << "[PanelController] Loaded DBus menu from" << service << path;
  return true;
}

bool PanelController::fetchMenuFromRegistrar(const QString &windowId,
                                             QString *service,
                                             QString *path) const {
  if (!service || !path) {
    return false;
  }

  bool ok = false;
  const quint64 windowHandle = windowId.toULongLong(&ok, 0);
  if (!ok) {
    return false;
  }

  struct RegistrarInfo {
    const char *serviceName;
    const char *objectPath;
    const char *interfaceName;
  };

  const RegistrarInfo registrars[] = {
      {"com.canonical.AppMenu.Registrar", "/com/canonical/AppMenu/Registrar",
       "com.canonical.AppMenu.Registrar"},
      {"org.kde.kappmenu", "/KAppMenu", "org.kde.kappmenu"}};

  auto *busInterface = QDBusConnection::sessionBus().interface();
  if (!busInterface) {
    return false;
  }

  for (const auto &registrar : registrars) {
    if (!busInterface->isServiceRegistered(registrar.serviceName)) {
      continue;
    }

    QDBusMessage message = QDBusMessage::createMethodCall(
        registrar.serviceName, registrar.objectPath, registrar.interfaceName,
        "GetMenuForWindow");
    message << QVariant::fromValue(windowHandle);
    QDBusMessage reply = QDBusConnection::sessionBus().call(message);
    if (reply.type() == QDBusMessage::ErrorMessage ||
        reply.arguments().size() < 2) {
      continue;
    }

    *service = reply.arguments().at(0).toString();
    const QVariant pathVariant = reply.arguments().at(1);
    if (pathVariant.canConvert<QDBusObjectPath>()) {
      *path = pathVariant.value<QDBusObjectPath>().path();
    } else {
      *path = pathVariant.toString();
    }

    if (!service->isEmpty() && !path->isEmpty()) {
      return true;
    }
  }

  return false;
}

void PanelController::initializeDefaultMenus() {
  // Default Filer menu (shown when idle or when app has no menu)
  m_globalMenuItems.clear();

  auto addMenu = [this](const QString &title, const QVariantList &items) {
    QVariantMap menu;
    menu["title"] = title;
    menu["items"] = items;
    m_globalMenuItems.append(menu);
  };

  // Filer menu structure (like macOS Finder)
  addMenu("Filer",
          {QVariantMap{{"label", "New Window"}, {"id", "new_window"}},
           QVariantMap{{"label", "New Folder"}, {"id", "new_folder"}},
           QVariantMap{{"separator", true}},
           QVariantMap{{"label", "Close Window"}, {"id", "close_window"}}});

  addMenu("File", {QVariantMap{{"label", "New Tab"}, {"id", "new_tab"}},
                   QVariantMap{{"separator", true}},
                   QVariantMap{{"label", "Get Info"}, {"id", "get_info"}},
                   QVariantMap{{"separator", true}},
                   QVariantMap{{"label", "Move to Trash"}, {"id", "trash"}}});

  addMenu("Edit", {QVariantMap{{"label", "Undo"}, {"id", "undo"}},
                   QVariantMap{{"label", "Redo"}, {"id", "redo"}},
                   QVariantMap{{"separator", true}},
                   QVariantMap{{"label", "Cut"}, {"id", "cut"}},
                   QVariantMap{{"label", "Copy"}, {"id", "copy"}},
                   QVariantMap{{"label", "Paste"}, {"id", "paste"}},
                   QVariantMap{{"separator", true}},
                   QVariantMap{{"label", "Select All"}, {"id", "select_all"}}});

  addMenu(
      "View",
      {QVariantMap{{"label", "as Icons"}, {"id", "view_icons"}},
       QVariantMap{{"label", "as List"}, {"id", "view_list"}},
       QVariantMap{{"label", "as Columns"}, {"id", "view_columns"}},
       QVariantMap{{"separator", true}},
       QVariantMap{{"label", "Show Path Bar"}, {"id", "show_path_bar"}},
       QVariantMap{{"label", "Show Status Bar"}, {"id", "show_status_bar"}}});

  addMenu("Go", {QVariantMap{{"label", "Back"}, {"id", "go_back"}},
                 QVariantMap{{"label", "Forward"}, {"id", "go_forward"}},
                 QVariantMap{{"separator", true}},
                 QVariantMap{{"label", "Home"}, {"id", "go_home"}},
                 QVariantMap{{"label", "Documents"}, {"id", "go_documents"}},
                 QVariantMap{{"label", "Downloads"}, {"id", "go_downloads"}}});

  addMenu("Window",
          {QVariantMap{{"label", "Minimize"}, {"id", "minimize"}},
           QVariantMap{{"label", "Zoom"}, {"id", "zoom"}},
           QVariantMap{{"separator", true}},
           QVariantMap{{"label", "Bring All to Front"}, {"id", "bring_all"}}});

  addMenu("Help",
          {QVariantMap{{"label", "Filer Help"}, {"id", "filer_help"}},
           QVariantMap{{"separator", true}},
           QVariantMap{{"label", "About Filer"}, {"id", "about_filer"}}});

  emit globalMenuItemsChanged();

  qDebug() << "[PanelController] Initialized default Filer menus";
}
