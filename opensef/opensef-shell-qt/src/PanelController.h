#ifndef PANEL_CONTROLLER_H
#define PANEL_CONTROLLER_H

#include <QObject>
#include <QString>
#include <QVariantList>

/**
 * PanelController - Native openSEF Panel Controller
 *
 * DBusMenu importer + openSEF framework bridge
 * Manages:
 * - Active window title
 * - Global menu (native openSEF API)
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
  ~PanelController() override;

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
  void showMenu(int menuIndex, int x, int y);
  void hideMenu();
  void reportWindowGeometry(const QString &idOrAppId, int x, int y, int w,
                            int h);

signals:
  void activeWindowTitleChanged();
  void activeAppIdChanged();
  void globalMenuItemsChanged();
  void currentTimeChanged();
  void multitaskActiveChanged();
  void menuRequested(int index, int x, int y);

private slots:
  void updateClock();
  void onWindowFocused(const QString &windowId, const QString &title,
                       const QString &appId, const QString &menuService,
                       const QString &menuPath);

private:
  void connectToFramework();
  void initializeDefaultMenus();
  void clearMenuItems();
  void loadMenuForFocusedApp(const QString &windowId, const QString &appId,
                             const QString &menuService,
                             const QString &menuPath);
  bool loadDbusMenu(const QString &service, const QString &path);
  bool fetchMenuFromRegistrar(const QString &windowId, QString *service,
                              QString *path) const;

  QString m_activeWindowTitle;
  QString m_activeAppId;
  QVariantList m_globalMenuItems;
  QString m_currentTime;
  bool m_multitaskActive = false;
  QString m_menuService;
  QString m_menuPath;
};

#endif // PANEL_CONTROLLER_H
