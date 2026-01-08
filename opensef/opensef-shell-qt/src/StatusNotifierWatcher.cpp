#include "StatusNotifierWatcher.h"
#include <QDBusArgument>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusReply>
#include <QDebug>


// DBus service names
static const QString WATCHER_SERVICE = "org.kde.StatusNotifierWatcher";
static const QString WATCHER_PATH = "/StatusNotifierWatcher";
static const QString WATCHER_INTERFACE = "org.kde.StatusNotifierWatcher";
static const QString HOST_SERVICE = "org.kde.StatusNotifierHost-";
static const QString ITEM_INTERFACE = "org.kde.StatusNotifierItem";

StatusNotifierWatcher::StatusNotifierWatcher(QObject *parent)
    : QObject(parent) {
  connectToWatcher();
}

StatusNotifierWatcher::~StatusNotifierWatcher() { delete m_watcher; }

void StatusNotifierWatcher::connectToWatcher() {
  // Connect to the StatusNotifierWatcher service
  m_watcher =
      new QDBusInterface(WATCHER_SERVICE, WATCHER_PATH, WATCHER_INTERFACE,
                         QDBusConnection::sessionBus(), this);

  if (!m_watcher->isValid()) {
    qDebug() << "[StatusNotifierWatcher] Watcher service not available";
    qDebug() << "[StatusNotifierWatcher] Error:"
             << m_watcher->lastError().message();
    m_running = false;
    return;
  }

  m_running = true;

  // Subscribe to item registration signals
  QDBusConnection::sessionBus().connect(
      WATCHER_SERVICE, WATCHER_PATH, WATCHER_INTERFACE,
      "StatusNotifierItemRegistered", this, SLOT(onItemRegistered(QString)));

  QDBusConnection::sessionBus().connect(WATCHER_SERVICE, WATCHER_PATH,
                                        WATCHER_INTERFACE,
                                        "StatusNotifierItemUnregistered", this,
                                        SLOT(onItemUnregistered(QString)));

  // Register as a host
  registerAsHost();

  // Get initial list of items
  refreshItems();

  qDebug() << "[StatusNotifierWatcher] Connected to watcher service";
}

void StatusNotifierWatcher::registerAsHost() {
  // Register ourselves as a StatusNotifierHost
  QString hostName =
      HOST_SERVICE + QString::number(QCoreApplication::applicationPid());

  QDBusConnection::sessionBus().registerService(hostName);

  // Tell the watcher we're a host
  m_watcher->call("RegisterStatusNotifierHost", hostName);

  qDebug() << "[StatusNotifierWatcher] Registered as host:" << hostName;
}

void StatusNotifierWatcher::refreshItems() {
  // Get RegisteredStatusNotifierItems property
  QDBusMessage msg = QDBusMessage::createMethodCall(
      WATCHER_SERVICE, WATCHER_PATH, "org.freedesktop.DBus.Properties", "Get");
  msg << WATCHER_INTERFACE << "RegisteredStatusNotifierItems";

  QDBusReply<QDBusVariant> reply = QDBusConnection::sessionBus().call(msg);

  if (reply.isValid()) {
    m_registeredItems = reply.value().variant().toStringList();
    qDebug() << "[StatusNotifierWatcher] Found" << m_registeredItems.size()
             << "items";
    emit itemsChanged();
  }
}

void StatusNotifierWatcher::onItemRegistered(const QString &service) {
  qDebug() << "[StatusNotifierWatcher] Item registered:" << service;

  if (!m_registeredItems.contains(service)) {
    m_registeredItems.append(service);
    emit itemRegistered(service);
    emit itemsChanged();
  }
}

void StatusNotifierWatcher::onItemUnregistered(const QString &service) {
  qDebug() << "[StatusNotifierWatcher] Item unregistered:" << service;

  m_registeredItems.removeAll(service);
  emit itemUnregistered(service);
  emit itemsChanged();
}

QVariantMap StatusNotifierWatcher::getItemInfo(const QString &service) const {
  QVariantMap info;

  // Service format: "name/path" or "name"
  QString serviceName = service;
  QString path = "/StatusNotifierItem";

  if (service.contains('/')) {
    QStringList parts = service.split('/');
    serviceName = parts[0];
    path = "/" + parts.mid(1).join('/');
  }

  // Query the item for its properties
  QDBusInterface item(serviceName, path, ITEM_INTERFACE,
                      QDBusConnection::sessionBus());

  if (!item.isValid()) {
    return info;
  }

  // Get standard properties
  auto getProperty = [&](const QString &name) -> QVariant {
    QDBusMessage msg = QDBusMessage::createMethodCall(
        serviceName, path, "org.freedesktop.DBus.Properties", "Get");
    msg << ITEM_INTERFACE << name;
    QDBusReply<QDBusVariant> reply = QDBusConnection::sessionBus().call(msg);
    if (reply.isValid()) {
      return reply.value().variant();
    }
    return QVariant();
  };

  info["id"] = service;
  info["title"] = getProperty("Title");
  info["iconName"] = getProperty("IconName");
  info["iconThemePath"] = getProperty("IconThemePath");
  info["toolTip"] = getProperty("ToolTip");
  info["category"] = getProperty("Category");
  info["status"] = getProperty("Status");

  return info;
}
