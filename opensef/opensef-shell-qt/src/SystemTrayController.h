#ifndef SYSTEM_TRAY_CONTROLLER_H
#define SYSTEM_TRAY_CONTROLLER_H

#include <QObject>
#include <QVariantList>

/**
 * SystemTrayController - macOS-style minimalist system tray
 *
 * Features:
 * - Icon-only display (no text labels)
 * - StatusNotifierItem protocol support
 * - Template icons (adapt to theme)
 * - Contextual icons (show only when active)
 */
class SystemTrayController : public QObject {
  Q_OBJECT

  Q_PROPERTY(QVariantList trayIcons READ trayIcons NOTIFY trayIconsChanged)
  Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)
  Q_PROPERTY(bool isMuted READ isMuted NOTIFY isMutedChanged)
  Q_PROPERTY(int batteryLevel READ batteryLevel NOTIFY batteryLevelChanged)
  Q_PROPERTY(bool isCharging READ isCharging NOTIFY isChargingChanged)

public:
  explicit SystemTrayController(QObject *parent = nullptr);
  ~SystemTrayController() override = default;

  QVariantList trayIcons() const { return m_trayIcons; }

  int volume() const { return m_volume; }
  void setVolume(int volume);

  bool isMuted() const { return m_isMuted; }

  int batteryLevel() const { return m_batteryLevel; }
  bool isCharging() const { return m_isCharging; }

public slots:
  void toggleMute();
  void trayIconClicked(const QString &iconId);
  void refreshBattery();

signals:
  void trayIconsChanged();
  void volumeChanged();
  void isMutedChanged();
  void batteryLevelChanged();
  void isChargingChanged();

private:
  void initTrayIcons();
  void connectToStatusNotifier();

  QVariantList m_trayIcons;
  int m_volume = 75;
  bool m_isMuted = false;
  int m_batteryLevel = 85;
  bool m_isCharging = false;
};

#endif // SYSTEM_TRAY_CONTROLLER_H
