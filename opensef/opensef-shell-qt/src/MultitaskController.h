#ifndef MULTITASK_CONTROLLER_H
#define MULTITASK_CONTROLLER_H

#include <QObject>
#include <QVariantList>

/**
 * MultitaskController - Ares Hybrid Mission Control controller
 *
 * Features:
 * - Panoramic horizontal spread
 * - Real-time window thumbnails via OSFEventBus
 * - High-precision keyboard navigation
 * - Spring-physics driven focus
 */
class MultitaskController : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
  Q_PROPERTY(QVariantList windows READ windows NOTIFY windowsChanged)
  Q_PROPERTY(int selectedIndex READ selectedIndex WRITE setSelectedIndex NOTIFY
                 selectedIndexChanged)

public:
  explicit MultitaskController(QObject *parent = nullptr);
  ~MultitaskController() override = default;

  bool active() const { return m_active; }
  void setActive(bool active);

  QVariantList windows() const { return m_windows; }

  int selectedIndex() const { return m_selectedIndex; }
  void setSelectedIndex(int index);

public slots:
  void toggle();
  void focusWindow(const QString &windowId);
  void activateWindow(const QString &windowId); // Alias for focusWindow
  void closeWindow(const QString &windowId);
  void minimizeWindow(const QString &windowId);
  void maximizeWindow(const QString &windowId);

  void refreshWindows();

  // Keyboard navigation
  void selectNext();
  void selectPrevious();
  void activateSelected();

signals:
  void activeChanged();
  void windowsChanged();
  void selectedIndexChanged();

private:
  void connectToFramework();

  bool m_active = false;
  QVariantList m_windows;
  int m_selectedIndex = 0;
};

#endif // MULTITASK_CONTROLLER_H
