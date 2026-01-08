#ifndef PANEL_CONTROLLER_H
#define PANEL_CONTROLLER_H

#include <QObject>
#include <QString>
#include <QVariantList>

/**
 * PanelController - Global menu panel controller
 *
 * Manages the top panel including:
 * - Active window title
 * - Global menu (DBusMenu integration)
 * - System tray icons
 * - Clock
 */
class PanelController : public QObject {
  Q_OBJECT

  // Properties exposed to QML
  Q_PROPERTY(QString activeWindowTitle READ activeWindowTitle NOTIFY
                 activeWindowTitleChanged)
  Q_PROPERTY(QString activeAppId READ activeAppId NOTIFY activeAppIdChanged)
  Q_PROPERTY(QVariantList globalMenuItems READ globalMenuItems NOTIFY
                 globalMenuItemsChanged)
  Q_PROPERTY(QString currentTime READ currentTime NOTIFY currentTimeChanged)
  Q_PROPERTY(
      bool multitaskActive READ multitaskActive NOTIFY multitaskActiveChanged)

public:
  explicit PanelController(QObject *parent = nullptr);
  ~PanelController() override = default;

  // Property getters
  QString activeWindowTitle() const { return m_activeWindowTitle; }
  QString activeAppId() const { return m_activeAppId; }
  QVariantList globalMenuItems() const { return m_globalMenuItems; }
  QString currentTime() const { return m_currentTime; }
  bool multitaskActive() const { return m_multitaskActive; }

public slots:
  // Actions from QML
  void toggleMultitask();
  void menuItemClicked(int menuIndex, int itemIndex);
  void showMenu(int menuIndex);
  void hideMenu();

signals:
  void activeWindowTitleChanged();
  void activeAppIdChanged();
  void globalMenuItemsChanged();
  void currentTimeChanged();
  void multitaskActiveChanged();

private slots:
  void updateClock();
  void onWindowFocused(const QString &windowId, const QString &title,
                       const QString &appId);

private:
  void connectToFramework();
  void loadMenuForWindow(const QString &appId);

  QString m_activeWindowTitle;
  QString m_activeAppId;
  QVariantList m_globalMenuItems;
  QString m_currentTime;
  bool m_multitaskActive = false;
};

#endif // PANEL_CONTROLLER_H
