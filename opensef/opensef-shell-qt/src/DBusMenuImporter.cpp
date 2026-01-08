#include "DBusMenuImporter.h"
#include <QDBusArgument>
#include <QDBusConnection>
#include <QDBusReply>
#include <QDebug>


DBusMenuImporter::DBusMenuImporter(const QString &service, const QString &path,
                                   QObject *parent)
    : QObject(parent), m_service(service), m_path(path) {
  // Connect to DBusMenu service
  m_interface = new QDBusInterface(service, path, "com.canonical.dbusmenu",
                                   QDBusConnection::sessionBus(), this);

  if (m_interface->isValid()) {
    // Subscribe to menu updates
    QDBusConnection::sessionBus().connect(
        service, path, "com.canonical.dbusmenu", "LayoutUpdated", this,
        SLOT(onLayoutUpdated(uint, int)));

    QDBusConnection::sessionBus().connect(
        service, path, "com.canonical.dbusmenu", "ItemsPropertiesUpdated", this,
        SLOT(onItemsPropertiesUpdated(QVariantList, QVariantList)));

    // Initial fetch
    fetchMenu();

    qDebug() << "[DBusMenuImporter] Connected to" << service << path;
  } else {
    qWarning() << "[DBusMenuImporter] Failed to connect to" << service << path;
  }
}

DBusMenuImporter::~DBusMenuImporter() = default;

bool DBusMenuImporter::isValid() const {
  return m_interface && m_interface->isValid();
}

void DBusMenuImporter::refresh() { fetchMenu(); }

void DBusMenuImporter::fetchMenu() {
  if (!isValid())
    return;

  // GetLayout(parentId, recursionDepth, propertyNames)
  QDBusMessage msg = m_interface->call("GetLayout",
                                       0,  // parent ID (0 = root)
                                       -1, // recursion depth (-1 = full tree)
                                       QStringList() // all properties
  );

  if (msg.type() == QDBusMessage::ReplyMessage) {
    QVariantList args = msg.arguments();
    if (args.size() >= 2) {
      // Response: (revision, layout)
      // layout is (id, properties, children)
      parseMenuStructure(args[1], 0);
      emit menuUpdated();
    }
  } else {
    emit menuError(msg.errorMessage());
  }
}

void DBusMenuImporter::parseMenuStructure(const QVariant &data, int parentId) {
  if (parentId == 0) {
    m_menuItems.clear();
  }

  // DBusMenu layout structure:
  // struct { int id; map properties; array children; }

  QDBusArgument arg = data.value<QDBusArgument>();

  arg.beginStructure();

  int id;
  arg >> id;

  QVariantMap properties;
  arg >> properties;

  // Extract menu item info
  QString label = properties.value("label", "").toString();
  bool enabled = properties.value("enabled", true).toBool();
  bool visible = properties.value("visible", true).toBool();
  QString type = properties.value("type", "standard").toString();
  QString iconName = properties.value("icon-name", "").toString();

  if (visible && !label.isEmpty()) {
    QVariantMap item;
    item["id"] = id;
    item["label"] = label;
    item["enabled"] = enabled;
    item["type"] = type; // "standard", "separator", "checkbox", etc.
    item["iconName"] = iconName;
    item["parentId"] = parentId;

    m_menuItems.append(item);
  }

  // Parse children recursively
  arg.beginArray();
  while (!arg.atEnd()) {
    QVariant child;
    arg >> child;
    parseMenuStructure(child, id);
  }
  arg.endArray();

  arg.endStructure();
}

void DBusMenuImporter::activateItem(int id) {
  if (!isValid())
    return;

  // Event(id, eventType, data, timestamp)
  m_interface->call("Event", id, QString("clicked"), QDBusVariant(QVariant()),
                    (uint)QDateTime::currentSecsSinceEpoch());

  qDebug() << "[DBusMenuImporter] Activated item:" << id;
}

void DBusMenuImporter::onLayoutUpdated(uint revision, int parent) {
  Q_UNUSED(revision)
  Q_UNUSED(parent)

  qDebug() << "[DBusMenuImporter] Layout updated, refreshing...";
  fetchMenu();
}

void DBusMenuImporter::onItemsPropertiesUpdated(const QVariantList &updated,
                                                const QVariantList &removed) {
  Q_UNUSED(updated)
  Q_UNUSED(removed)

  // For simplicity, just refresh the whole menu
  fetchMenu();
}
