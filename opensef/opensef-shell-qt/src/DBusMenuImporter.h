#ifndef DBUS_MENU_IMPORTER_H
#define DBUS_MENU_IMPORTER_H

#include <QDBusInterface>
#include <QObject>
#include <QString>
#include <QVariantList>


/**
 * DBusMenuImporter - Import application menus via DBusMenu protocol
 *
 * Connects to com.canonical.dbusmenu to fetch menu structure
 * from Qt and GTK applications for global menu integration.
 */
class DBusMenuImporter : public QObject {
  Q_OBJECT

public:
  explicit DBusMenuImporter(const QString &service, const QString &path,
                            QObject *parent = nullptr);
  ~DBusMenuImporter() override;

  QVariantList menuItems() const { return m_menuItems; }
  bool isValid() const;

signals:
  void menuUpdated();
  void menuError(const QString &error);

public slots:
  void refresh();
  void activateItem(int id);

private slots:
  void onLayoutUpdated(uint revision, int parent);
  void onItemsPropertiesUpdated(const QVariantList &updated,
                                const QVariantList &removed);

private:
  void fetchMenu();
  void parseMenuStructure(const QVariant &data, int parentId = 0);

  QString m_service;
  QString m_path;
  QDBusInterface *m_interface = nullptr;
  QVariantList m_menuItems;
};

#endif // DBUS_MENU_IMPORTER_H
