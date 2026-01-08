#include "SystemTrayController.h"
#include "StatusNotifierWatcher.h"
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDebug>
#include <QFile>
#include <QTimer>


SystemTrayController::SystemTrayController(QObject *parent) : QObject(parent) {
  // Connect to StatusNotifierWatcher for app icons
  connectToStatusNotifier();

  // Periodic battery refresh
  QTimer *batteryTimer = new QTimer(this);
  connect(batteryTimer, &QTimer::timeout, this,
          &SystemTrayController::refreshBattery);
  batteryTimer->start(30000); // Every 30 seconds

  // Initial battery check
  refreshBattery();
}

SystemTrayController::~SystemTrayController() { delete m_notifierWatcher; }

void SystemTrayController::connectToStatusNotifier() {
  m_notifierWatcher = new StatusNotifierWatcher(this);

  if (m_notifierWatcher->isRunning()) {
    connect(m_notifierWatcher, &StatusNotifierWatcher::itemsChanged, this,
            &SystemTrayController::onItemsChanged);

    // Initial rebuild
    rebuildTrayIcons();

    qDebug() << "[SystemTrayController] Connected to StatusNotifierWatcher";
  } else {
    qDebug() << "[SystemTrayController] StatusNotifierWatcher not available";
    initTrayIcons();
  }
}

void SystemTrayController::onItemsChanged() { rebuildTrayIcons(); }

void SystemTrayController::rebuildTrayIcons() {
  m_trayIcons.clear();

  if (m_notifierWatcher && m_notifierWatcher->isRunning()) {
    QStringList items = m_notifierWatcher->registeredItems();

    for (const QString &service : items) {
      QVariantMap info = m_notifierWatcher->getItemInfo(service);

      if (!info.isEmpty()) {
        QVariantMap icon;
        icon["id"] = info["id"];
        icon["iconName"] = info["iconName"];
        icon["title"] = info["title"];
        icon["category"] = info["category"];
        icon["status"] = info["status"];

        // Only show icons that are "Active" or "NeedsAttention"
        QString status = info["status"].toString();
        if (status != "Passive") {
          m_trayIcons.append(icon);
        }
      }
    }
  }

  emit trayIconsChanged();
  qDebug() << "[SystemTrayController] Rebuilt tray icons:"
           << m_trayIcons.size();
}

void SystemTrayController::initTrayIcons() {
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
  qDebug() << "[SystemTrayController] Tray icon clicked:" << iconId;

  QString serviceName = iconId;
  QString path = "/StatusNotifierItem";

  if (iconId.contains('/')) {
    QStringList parts = iconId.split('/');
    serviceName = parts[0];
    path = "/" + parts.mid(1).join('/');
  }

  QDBusInterface item(serviceName, path, "org.kde.StatusNotifierItem",
                      QDBusConnection::sessionBus());

  if (item.isValid()) {
    item.call("Activate", 0, 0);
  }
}

void SystemTrayController::trayIconRightClicked(const QString &iconId) {
  qDebug() << "[SystemTrayController] Tray icon right-clicked:" << iconId;

  QString serviceName = iconId;
  QString path = "/StatusNotifierItem";

  if (iconId.contains('/')) {
    QStringList parts = iconId.split('/');
    serviceName = parts[0];
    path = "/" + parts.mid(1).join('/');
  }

  QDBusInterface item(serviceName, path, "org.kde.StatusNotifierItem",
                      QDBusConnection::sessionBus());

  if (item.isValid()) {
    item.call("ContextMenu", 0, 0);
  }
}

void SystemTrayController::refreshBattery() {
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
