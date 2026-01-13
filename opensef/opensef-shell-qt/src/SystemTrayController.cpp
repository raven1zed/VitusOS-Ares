#include "SystemTrayController.h"
#include "StatusNotifierWatcher.h"
#include <QDebug>
#include <QFile>
#include <QTimer>
#include <QVariantMap>

#include <OSFDesktop.h>
#include <OSFEventBus.h>

SystemTrayController::SystemTrayController(QObject *parent) : QObject(parent) {
  connectToFramework();

  // Periodic battery refresh
  QTimer *batteryTimer = new QTimer(this);
  connect(batteryTimer, &QTimer::timeout, this,
          &SystemTrayController::refreshBattery);
  batteryTimer->start(30000); // Every 30 seconds

  // Initial battery check
  refreshBattery();

  qDebug() << "[SystemTrayController] Initialized (StatusNotifier watcher)";
}

SystemTrayController::~SystemTrayController() {}

void SystemTrayController::connectToFramework() {
  m_statusNotifierWatcher = new StatusNotifierWatcher(this);
  connect(m_statusNotifierWatcher, &StatusNotifierWatcher::itemsChanged, this,
          &SystemTrayController::updateTrayIconsFromWatcher);

  updateTrayIconsFromWatcher();

  if (auto *desktop = OpenSEF::OSFDesktop::shared()) {
    if (auto *eventBus = desktop->eventBus()) {
      eventBus->subscribe("tray.refresh", [this](const OpenSEF::OSFEvent &) {
        updateTrayIconsFromWatcher();
      });
    }
  }
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

void SystemTrayController::updateTrayIconsFromWatcher() {
  QVariantList icons;
  if (m_statusNotifierWatcher) {
    const auto items = m_statusNotifierWatcher->items();
    icons.reserve(items.size());
    for (const auto &item : items) {
      QVariantMap entry;
      entry.insert("id", item.service);
      entry.insert("iconName", item.iconName);
      icons.append(entry);
    }
  }

  if (icons != m_trayIcons) {
    m_trayIcons = icons;
    emit trayIconsChanged();

    if (auto *desktop = OpenSEF::OSFDesktop::shared()) {
      if (auto *eventBus = desktop->eventBus()) {
        OpenSEF::OSFEvent event;
        event.set("count", static_cast<int>(m_trayIcons.size()));
        eventBus->publish("tray.iconsChanged", event);
      }
    }
  }
}
