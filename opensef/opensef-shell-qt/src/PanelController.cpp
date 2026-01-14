#include "PanelController.h"
#include <OSFDesktop.h>
#include <OSFEventBus.h>
#include <OSFStateManager.h>
#include <QDateTime>
#include <QDebug>
#include <QTimer>

PanelController::PanelController(QObject *parent) : QObject(parent) {
  // Initialize clock update timer
  QTimer *clockTimer = new QTimer(this);
  connect(clockTimer, &QTimer::timeout, this, &PanelController::updateClock);
  clockTimer->start(1000); // Update every second

  // Initial clock update
  updateClock();

  // Connect to framework for window events
  connectToFramework();

  // Start with empty title (will update when window focused)
  m_activeWindowTitle = "";
  emit activeWindowTitleChanged();

  // Initialize static menus matching the mockup
  initializeDefaultMenus();
}

PanelController::~PanelController() {}

void PanelController::initializeDefaultMenus() {
  m_globalMenuItems.clear();

  auto addMenu = [this](const QString &title, const QVariantList &items) {
    QVariantMap menu;
    menu["title"] = title;
    menu["items"] = items;
    m_globalMenuItems.append(menu);
  };

  // EXACT MOCKUP STRUCTURE: Filer | Menu | Settings | Help
  addMenu("Filer", {QVariantMap{{"label", "New Window"}, {"id", "new_window"}},
                    QVariantMap{{"label", "New Folder"}, {"id", "new_folder"}},
                    QVariantMap{{"separator", true}},
                    QVariantMap{{"label", "Close"}, {"id", "close"}}});

  addMenu("Menu",
          {QVariantMap{{"label", "About VitusOS Ares"}, {"id", "about"}},
           QVariantMap{{"separator", true}},
           QVariantMap{{"label", "System Preferences..."}, {"id", "prefs"}},
           QVariantMap{{"separator", true}},
           QVariantMap{{"label", "Force Quit..."}, {"id", "force_quit"}},
           QVariantMap{{"separator", true}},
           QVariantMap{{"label", "Sleep"}, {"id", "sleep"}},
           QVariantMap{{"label", "Restart..."}, {"id", "restart"}},
           QVariantMap{{"label", "Shut Down..."}, {"id", "shutdown"}}});

  addMenu("Settings",
          {QVariantMap{{"label", "Display"}, {"id", "display"}},
           QVariantMap{{"label", "Sound"}, {"id", "sound"}},
           QVariantMap{{"label", "Network"}, {"id", "network"}},
           QVariantMap{{"separator", true}},
           QVariantMap{{"label", "Users & Groups"}, {"id", "users"}}});

  addMenu("Help", {QVariantMap{{"label", "VitusOS Help"}, {"id", "help"}},
                   QVariantMap{{"separator", true}},
                   QVariantMap{{"label", "Report a Bug"}, {"id", "bug"}}});

  emit globalMenuItemsChanged();
}

void PanelController::connectToFramework() {
  qDebug() << "[PanelController] Subscribing to OSFEventBus...";

  auto *eventBus = OpenSEF::OSFDesktop::shared()->eventBus();

  // Subscribe to window focus events
  eventBus->subscribe(
      OpenSEF::OSFEventBus::WINDOW_FOCUSED, [this](const OpenSEF::OSFEvent &e) {
        QString windowId =
            QString::fromStdString(e.get<std::string>("window_id"));
        QString title = QString::fromStdString(e.get<std::string>("title"));
        QString appId = QString::fromStdString(e.get<std::string>("app_id"));

        if (title.isEmpty() || appId.isEmpty()) {
          auto *window =
              OpenSEF::OSFDesktop::shared()->stateManager()->windowById(
                  windowId.toStdString());
          if (window) {
            if (title.isEmpty()) {
              title = QString::fromStdString(window->title());
            }
            if (appId.isEmpty()) {
              appId = QString::fromStdString(window->appId());
            }
          }
        }

        // Update on UI thread
        QMetaObject::invokeMethod(
            this, [this, title, appId]() { onWindowFocused(title, appId); });
      });

  qDebug() << "[PanelController] Success: Connected to OSF framework.";
}

void PanelController::updateClock() {
  QDateTime now = QDateTime::currentDateTime();
  // Mockup format: Tuesday,12 10:15 PM (note: no space after day, space before
  // time)
  QString time = now.toString("dddd,d h:mm AP");
  if (time != m_currentTime) {
    m_currentTime = time;
    emit currentTimeChanged();
  }
}

void PanelController::toggleMultitask() {
  m_multitaskActive = !m_multitaskActive;
  emit multitaskActiveChanged();

  qDebug() << "[PanelController] Multitask toggled:" << m_multitaskActive;
}

void PanelController::menuItemClicked(int menuIndex, int itemIndex) {
  qDebug() << "[PanelController] Menu item clicked:" << menuIndex << itemIndex;
  // Menu actions would be handled here
}

void PanelController::showMenu(int menuIndex, int x, int y) {
  qDebug() << "[PanelController] Show menu:" << menuIndex << "at" << x << y;
  emit menuRequested(menuIndex, x, y);
}

void PanelController::hideMenu() { qDebug() << "[PanelController] Hide menu"; }

void PanelController::onWindowFocused(const QString &title,
                                      const QString &appId) {
  qDebug() << "[PanelController] Window focused:" << title << "(" << appId
           << ")";

  // Update window title
  if (title != m_activeWindowTitle) {
    m_activeWindowTitle = title;
    emit activeWindowTitleChanged();
  }

  // Update app ID
  if (appId != m_activeAppId) {
    m_activeAppId = appId;
    emit activeAppIdChanged();
  }
}

void PanelController::reportWindowGeometry(const QString &idOrAppId, int x,
                                           int y, int w, int h) {
  auto *sm = OpenSEF::OSFDesktop::shared()->stateManager();
  if (!sm)
    return;

  auto *win = sm->windowById(idOrAppId.toStdString());
  if (!win) {
    auto windows = sm->allWindows();
    for (auto *w : windows) {
      if (QString::fromStdString(w->appId())
              .contains(idOrAppId, Qt::CaseInsensitive) ||
          QString::fromStdString(w->title())
              .contains(idOrAppId, Qt::CaseInsensitive)) {
        win = w;
        break;
      }
    }
  }

  if (win) {
    win->setGeometry(x, y, w, h);
  }
}
