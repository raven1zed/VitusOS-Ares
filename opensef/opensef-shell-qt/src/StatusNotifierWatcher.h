#ifndef STATUS_NOTIFIER_WATCHER_H
#define STATUS_NOTIFIER_WATCHER_H

#include <QDBusInterface>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariantList>


/**
 * StatusNotifierWatcher - Client for org.kde.StatusNotifierWatcher
 *
 * Watches for StatusNotifierItem registrations on DBus.
 * Applications like Discord, Dropbox, etc. register here.
 */
class StatusNotifierWatcher : public QObject {
  Q_OBJECT

public:
  explicit StatusNotifierWatcher(QObject *parent = nullptr);
  ~StatusNotifierWatcher() override;

  bool isRunning() const { return m_running; }
  QStringList registeredItems() const { return m_registeredItems; }

  // Get icon info for a registered item
  QVariantMap getItemInfo(const QString &service) const;

signals:
  void itemRegistered(const QString &service);
  void itemUnregistered(const QString &service);
  void itemsChanged();

private slots:
  void onItemRegistered(const QString &service);
  void onItemUnregistered(const QString &service);
  void refreshItems();

private:
  void connectToWatcher();
  void registerAsHost();

  QDBusInterface *m_watcher = nullptr;
  QStringList m_registeredItems;
  bool m_running = false;
};

#endif // STATUS_NOTIFIER_WATCHER_H
