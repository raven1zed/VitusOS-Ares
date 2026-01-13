#ifndef STATUS_NOTIFIER_WATCHER_H
#define STATUS_NOTIFIER_WATCHER_H

#include <QDBusConnection>
#include <QDBusMessage>
#include <QHash>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariantMap>

class QDBusServiceWatcher;

class StatusNotifierWatcher : public QObject {
  Q_OBJECT
  Q_CLASSINFO("D-Bus Interface", "org.kde.StatusNotifierWatcher")
  Q_PROPERTY(QStringList RegisteredStatusNotifierItems READ registeredStatusNotifierItems)
  Q_PROPERTY(bool IsStatusNotifierHostRegistered READ isStatusNotifierHostRegistered)
  Q_PROPERTY(int ProtocolVersion READ protocolVersion)

public:
  struct TrayItem {
    QString service;
    QString objectPath;
    QString iconName;
  };

  explicit StatusNotifierWatcher(QObject *parent = nullptr);

  QStringList registeredStatusNotifierItems() const;
  bool isStatusNotifierHostRegistered() const;
  int protocolVersion() const;
  QList<TrayItem> items() const;

public slots:
  void RegisterStatusNotifierItem(const QString &service, const QDBusMessage &message);
  void RegisterStatusNotifierHost(const QString &service);

signals:
  void StatusNotifierItemRegistered(const QString &service);
  void StatusNotifierItemUnregistered(const QString &service);
  void StatusNotifierHostRegistered();
  void itemsChanged();

private slots:
  void handleServiceUnregistered(const QString &service);
  void handlePropertiesChanged(const QString &interface, const QVariantMap &changed,
                               const QStringList &invalidated, const QDBusMessage &message);

private:
  void registerOnBus();
  void registerItem(const QString &service, const QString &objectPath);
  QString resolveObjectPath(const QString &service) const;
  QString fetchIconName(const QString &service, const QString &objectPath) const;

  bool m_isHostRegistered = false;
  QHash<QString, TrayItem> m_items;
  QDBusConnection m_bus;
  QDBusServiceWatcher *m_serviceWatcher;
};

#endif // STATUS_NOTIFIER_WATCHER_H
