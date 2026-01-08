#include "SystemTrayController.h"
#include <QDebug>
#include <QTimer>

SystemTrayController::SystemTrayController(QObject *parent) : QObject(parent) {
  initTrayIcons();
  connectToStatusNotifier();

  // Periodic battery refresh
  QTimer *batteryTimer = new QTimer(this);
  connect(batteryTimer, &QTimer::timeout, this,
          &SystemTrayController::refreshBattery);
  batteryTimer->start(30000); // Every 30 seconds
}

void SystemTrayController::initTrayIcons() {
  // Core system tray icons (always visible)
  m_trayIcons.clear();

  // These are template icons that adapt to light/dark theme
  // Additional app icons will be added via StatusNotifierItem

  emit trayIconsChanged();
}

void SystemTrayController::connectToStatusNotifier() {
  // TODO: Connect to org.kde.StatusNotifierWatcher via DBus
  // Watch for new tray icons from applications

  qDebug() << "[SystemTrayController] Connecting to StatusNotifierWatcher...";
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
  qDebug() << "[SystemTrayController] Tray icon clicked:" << iconId;

  // TODO: Activate the corresponding StatusNotifierItem
}

void SystemTrayController::refreshBattery() {
  // TODO: Read actual battery state from /sys/class/power_supply
  // For now, simulate

  // In production:
  // QFile batteryFile("/sys/class/power_supply/BAT0/capacity");
  // m_batteryLevel = batteryFile.readAll().trimmed().toInt();

  qDebug() << "[SystemTrayController] Battery level:" << m_batteryLevel << "%";
}
