#pragma once

#include <QDir>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QObject>
#include <QProcess>
#include <QUrl>
#include <QVariantList>

/**
 * FilerController - Native File Manager Backend
 *
 * GNUstep-inspired C++ file manager controller.
 * Provides real filesystem operations exposed to QML.
 */
class FilerController : public QObject {
  Q_OBJECT

  Q_PROPERTY(QString currentPath READ currentPath WRITE setCurrentPath NOTIFY
                 currentPathChanged)
  Q_PROPERTY(
      QString currentPathName READ currentPathName NOTIFY currentPathChanged)
  Q_PROPERTY(QVariantList fileList READ fileList NOTIFY fileListChanged)
  Q_PROPERTY(QVariantList navigationHistory READ navigationHistory NOTIFY
                 navigationHistoryChanged)
  Q_PROPERTY(int historyIndex READ historyIndex NOTIFY navigationHistoryChanged)
  Q_PROPERTY(bool canGoBack READ canGoBack NOTIFY navigationHistoryChanged)
  Q_PROPERTY(
      bool canGoForward READ canGoForward NOTIFY navigationHistoryChanged)
  Q_PROPERTY(
      QVariantList selectedItems READ selectedItems NOTIFY selectionChanged)

public:
  explicit FilerController(QObject *parent = nullptr);
  ~FilerController();

  // Properties
  QString currentPath() const { return m_currentPath; }
  QString currentPathName() const;
  QVariantList fileList() const { return m_fileList; }
  QVariantList navigationHistory() const { return m_navigationHistory; }
  int historyIndex() const { return m_historyIndex; }
  bool canGoBack() const { return m_historyIndex > 0; }
  bool canGoForward() const {
    return m_historyIndex < m_navigationHistory.size() - 1;
  }
  QVariantList selectedItems() const { return m_selectedItems; }

  void setCurrentPath(const QString &path);

  // Navigation
  Q_INVOKABLE void navigateTo(const QString &path);
  Q_INVOKABLE void goBack();
  Q_INVOKABLE void goForward();
  Q_INVOKABLE void goUp();
  Q_INVOKABLE void goHome();

  // Quick Access (Favorites)
  Q_INVOKABLE void goToDesktop();
  Q_INVOKABLE void goToDocuments();
  Q_INVOKABLE void goToDownloads();
  Q_INVOKABLE void goToPictures();
  Q_INVOKABLE void goToMusic();

  // File Operations
  Q_INVOKABLE void openItem(int index);
  Q_INVOKABLE void openItemAtPath(const QString &path);
  Q_INVOKABLE bool createFolder(const QString &name);
  Q_INVOKABLE bool createFile(const QString &name);
  Q_INVOKABLE bool renameItem(const QString &oldPath, const QString &newName);
  Q_INVOKABLE bool moveToTrash(const QVariantList &paths);
  Q_INVOKABLE bool copyItems(const QVariantList &paths,
                             const QString &destination);
  Q_INVOKABLE bool moveItems(const QVariantList &paths,
                             const QString &destination);

  // Selection
  Q_INVOKABLE void selectItem(int index, bool append = false);
  Q_INVOKABLE void selectAll();
  Q_INVOKABLE void clearSelection();

  // File Info
  Q_INVOKABLE QVariantMap getFileInfo(const QString &path);

signals:
  void currentPathChanged();
  void fileListChanged();
  void navigationHistoryChanged();
  void selectionChanged();
  void errorOccurred(const QString &message);
  void fileOpened(const QString &path);

private slots:
  void onDirectoryChanged(const QString &path);

private:
  void refreshFileList();
  void addToHistory(const QString &path);
  QString formatFileSize(qint64 bytes) const;
  QString getMimeIcon(const QString &suffix) const;

  QString m_currentPath;
  QVariantList m_fileList;
  QVariantList m_navigationHistory;
  int m_historyIndex = -1;
  QVariantList m_selectedItems;
  QFileSystemWatcher *m_watcher;
};
