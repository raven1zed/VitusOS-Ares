#ifndef DOCK_CONTROLLER_H
#define DOCK_CONTROLLER_H

#include <QObject>
#include <QString>
#include <QVariantList>

/**
 * DockController - Application dock controller
 *
 * Manages the bottom dock including:
 * - Pinned applications
 * - Running applications
 * - App launching
 * - Autohide behavior
 */
class DockController : public QObject {
  Q_OBJECT

  Q_PROPERTY(QVariantList dockItems READ dockItems NOTIFY dockItemsChanged)
  Q_PROPERTY(bool isHidden READ isHidden NOTIFY isHiddenChanged)
  Q_PROPERTY(int hoveredIndex READ hoveredIndex WRITE setHoveredIndex NOTIFY
                 hoveredIndexChanged)

public:
  explicit DockController(QObject *parent = nullptr);
  ~DockController() override = default;

  QVariantList dockItems() const { return m_dockItems; }
  bool isHidden() const { return m_isHidden; }
  int hoveredIndex() const { return m_hoveredIndex; }
  void setHoveredIndex(int index);

public slots:
  void launchApp(int index);
  void showDock();
  void hideDock();
  void checkOverlap();

signals:
  void dockItemsChanged();
  void isHiddenChanged();
  void hoveredIndexChanged();

private:
  void initDockItems();
  void connectToFramework();

  QVariantList m_dockItems;
  bool m_isHidden = false;
  int m_hoveredIndex = -1;
};

#endif // DOCK_CONTROLLER_H
