#include "StatusNotifierWatcher.h"

#include <QDBusInterface>
#include <QDBusServiceWatcher>
#include <QDebug>

StatusNotifierWatcher::StatusNotifierWatcher(QObject *parent)
    : QObject(parent), m_bus(QDBusConnection::sessionBus()),
      m_serviceWatcher(nullptr) {
  registerOnBus();
}

QStringList StatusNotifierWatcher::registeredStatusNotifierItems() const {
  return m_items.keys();
}

bool StatusNotifierWatcher::isStatusNotifierHostRegistered() const {
  return m_isHostRegistered;
}

int StatusNotifierWatcher::protocolVersion() const { return 0; }

QList<StatusNotifierWatcher::TrayItem> StatusNotifierWatcher::items() const {
  return m_items.values();
}

void StatusNotifierWatcher::RegisterStatusNotifierItem(
    const QString &service, const QDBusMessage &message) {
  QString resolvedService = service;
  QString objectPath = resolveObjectPath(service);

  if (service.startsWith('/')) {
    resolvedService = message.service();
  }

  if (resolvedService.isEmpty()) {
    qWarning() << "[StatusNotifierWatcher] Missing service for item registration";
    return;
  }

  registerItem(resolvedService, objectPath);
}

void StatusNotifierWatcher::RegisterStatusNotifierHost(const QString &service) {
  Q_UNUSED(service)
  if (!m_isHostRegistered) {
    m_isHostRegistered = true;
    emit StatusNotifierHostRegistered();
  }
}

void StatusNotifierWatcher::handleServiceUnregistered(const QString &service) {
  if (!m_items.contains(service)) {
    return;
  }

  m_items.remove(service);
  emit StatusNotifierItemUnregistered(service);
  emit itemsChanged();
}

void StatusNotifierWatcher::handlePropertiesChanged(
    const QString &interface, const QVariantMap &changed,
    const QStringList &invalidated, const QDBusMessage &message) {
  Q_UNUSED(invalidated)
  if (interface != QLatin1String("org.kde.StatusNotifierItem")) {
    return;
  }

  if (!changed.contains("IconName")) {
    return;
  }

  const QString service = message.service();
  if (service.isEmpty() || !m_items.contains(service)) {
    return;
  }

  auto item = m_items.value(service);
  const QString iconName = changed.value("IconName").toString();
  if (!iconName.isEmpty() && iconName != item.iconName) {
    item.iconName = iconName;
    m_items[service] = item;
    emit itemsChanged();
  }
}

void StatusNotifierWatcher::registerOnBus() {
  if (!m_bus.isConnected()) {
    qWarning() << "[StatusNotifierWatcher] Session bus not available";
    return;
  }

  m_serviceWatcher = new QDBusServiceWatcher(this);
  m_serviceWatcher->setConnection(m_bus);
  m_serviceWatcher->setWatchMode(QDBusServiceWatcher::WatchForUnregistration);
  connect(m_serviceWatcher, &QDBusServiceWatcher::serviceUnregistered, this,
          &StatusNotifierWatcher::handleServiceUnregistered);

  if (!m_bus.registerService("org.kde.StatusNotifierWatcher")) {
    qWarning() << "[StatusNotifierWatcher] Could not register watcher service:"
               << m_bus.lastError();
  }

  if (!m_bus.registerObject("/StatusNotifierWatcher", this,
                            QDBusConnection::ExportAllSlots |
                                QDBusConnection::ExportAllSignals |
                                QDBusConnection::ExportAllProperties)) {
    qWarning() << "[StatusNotifierWatcher] Failed to register watcher object:"
               << m_bus.lastError();
  }
}

void StatusNotifierWatcher::registerItem(const QString &service,
                                         const QString &objectPath) {
  if (!m_bus.isConnected()) {
    qWarning() << "[StatusNotifierWatcher] Cannot register item without session bus";
    return;
  }

  const QString path = objectPath.isEmpty() ? QStringLiteral("/StatusNotifierItem")
                                            : objectPath;

  if (m_items.contains(service)) {
    auto existing = m_items.value(service);
    existing.iconName = fetchIconName(service, path);
    existing.objectPath = path;
    m_items[service] = existing;
    emit itemsChanged();
    return;
  }

  TrayItem item{service, path, fetchIconName(service, path)};
  m_items.insert(service, item);
  if (m_serviceWatcher) {
    m_serviceWatcher->addWatchedService(service);
  }

  if (!m_bus.connect(service, path, "org.freedesktop.DBus.Properties",
                     "PropertiesChanged", this,
                     SLOT(handlePropertiesChanged(QString,QVariantMap,QStringList,QDBusMessage)))) {
    qWarning() << "[StatusNotifierWatcher] Failed to connect PropertiesChanged for" << service;
  }

  emit StatusNotifierItemRegistered(service);
  emit itemsChanged();
}

QString StatusNotifierWatcher::resolveObjectPath(const QString &service) const {
  if (service.startsWith('/')) {
    return service;
  }
  return QStringLiteral("/StatusNotifierItem");
}

QString StatusNotifierWatcher::fetchIconName(const QString &service,
                                             const QString &objectPath) const {
  if (!m_bus.isConnected()) {
    return QStringLiteral("application-x-executable");
  }
  QDBusInterface iface(service, objectPath, "org.kde.StatusNotifierItem",
                       m_bus);
  if (!iface.isValid()) {
    return QStringLiteral("application-x-executable");
  }
  QVariant iconName = iface.property("IconName");
  if (iconName.isValid() && !iconName.toString().isEmpty()) {
    return iconName.toString();
  }
  return QStringLiteral("application-x-executable");
}
