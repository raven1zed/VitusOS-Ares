#include "SystemTrayController.h"
#include <QDebug>
#include <QFile>
#include <QTimer>

SystemTrayController::SystemTrayController(QObject *parent) : QObject(parent) {
  // Initialize with empty tray (no KDE dependencies)
  // Future: Connect to OSFDesktop::shared()->systemTray()

  connectToFramework();

  // Periodic battery refresh
  QTimer *batteryTimer = new QTimer(this);
  connect(batteryTimer, &QTimer::timeout, this,
          &SystemTrayController::refreshBattery);
  batteryTimer->start(30000); // Every 30 seconds

  // Initial battery check
  refreshBattery();

  qDebug() << "[SystemTrayController] Initialized (native openSEF, no KDE)";
}

SystemTrayController::~SystemTrayController() {}

void SystemTrayController::connectToFramework() {
  // TODO: When OSFSystemTray API is ready:
  // auto *tray = OpenSEF::OSFDesktop::shared()->systemTray();
  // connect(tray, &OSFSystemTray::iconsChanged, this,
  // &SystemTrayController::onIconsChanged);

  // For now: Empty tray (system icons only)
  m_trayIcons.clear();
  emit trayIconsChanged();
}

void SystemTrayController::setVolume(int volume) {
  volume = qBound(0, volume, 100);
  if (m_volume != volume) {
    m_volume = volume;
    emit volumeChanged();

    if (m_isMuted && m_volume > 0) {
      m_isMuted = false;
      emit isMutedChanged();
    }
  }
}

void SystemTrayController::toggleMute() {
  m_isMuted = !m_isMuted;
  emit isMutedChanged();
  qDebug() << "[SystemTrayController] Mute toggled:" << m_isMuted;
}

void SystemTrayController::trayIconClicked(const QString &iconId) {
  qDebug() << "[SystemTrayController] Icon clicked:" << iconId;
  // TODO: Trigger native openSEF action
}

void SystemTrayController::refreshBattery() {
  // Try to read battery status from sysfs
  QFile capacityFile("/sys/class/power_supply/BAT0/capacity");
  if (capacityFile.open(QIODevice::ReadOnly)) {
    QString content = capacityFile.readAll().trimmed();
    int newLevel = content.toInt();
    if (newLevel != m_batteryLevel) {
      m_batteryLevel = newLevel;
      emit batteryLevelChanged();
    }
    capacityFile.close();
  }

  QFile statusFile("/sys/class/power_supply/BAT0/status");
  if (statusFile.open(QIODevice::ReadOnly)) {
    QString status = statusFile.readAll().trimmed();
    bool charging = (status == "Charging" || status == "Full");
    if (charging != m_isCharging) {
      m_isCharging = charging;
      emit isChargingChanged();
    }
    statusFile.close();
  }
}
