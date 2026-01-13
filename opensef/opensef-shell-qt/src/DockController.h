#ifndef DOCKCONTROLLER_H
#define DOCKCONTROLLER_H

#include <QMap>
#include <QObject>
#include <QProcess>
#include <QVariant>

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
  ~DockController() override;

  QVariantList dockItems() const;
  bool isHidden() const;
  int hoveredIndex() const;
  void setHoveredIndex(int index);

public slots:
  void launchApp(int index);
  void toggleAutoHide();
  void showDock();
  void hideDock();
  void checkOverlap();

signals:
  void dockItemsChanged();
  void isHiddenChanged();
  void hoveredIndexChanged();

private:
  // Process tracking for direct app launching
  QMap<int, QProcess *> m_processes;

private slots:
  void onProcessFinished(int exitCode, int exitStatus);

private:
  void initDockItems();
  void connectToFramework();
  void refreshRunningStatus();

  QVariantList m_dockItems;
  bool m_isHidden;
  int m_hoveredIndex;
};

#endif // DOCKCONTROLLER_H
