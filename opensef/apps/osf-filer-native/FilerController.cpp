/**
 * FilerController.cpp - Native File Manager Implementation
 *
 * GNUstep-inspired C++ file manager.
 * Real filesystem operations for VitusOS Filer.
 */

#include "FilerController.h"
#include <QDebug>
#include <QDesktopServices>
#include <QMimeDatabase>
#include <QStandardPaths>
#include <algorithm>

FilerController::FilerController(QObject *parent) : QObject(parent) {
  m_watcher = new QFileSystemWatcher(this);
  connect(m_watcher, &QFileSystemWatcher::directoryChanged, this,
          &FilerController::onDirectoryChanged);

  // Start at home directory
  goHome();
}

FilerController::~FilerController() {}

QString FilerController::currentPathName() const {
  QDir dir(m_currentPath);
  if (m_currentPath == QDir::homePath()) {
    return "Home";
  }
  return dir.dirName();
}

void FilerController::setCurrentPath(const QString &path) {
  if (m_currentPath != path) {
    // Remove old path from watcher
    if (!m_currentPath.isEmpty()) {
      m_watcher->removePath(m_currentPath);
    }

    m_currentPath = path;

    // Add new path to watcher
    m_watcher->addPath(path);

    refreshFileList();
    emit currentPathChanged();
  }
}

void FilerController::navigateTo(const QString &path) {
  QFileInfo info(path);
  if (info.exists() && info.isDir()) {
    addToHistory(path);
    setCurrentPath(path);
  } else {
    emit errorOccurred("Directory does not exist: " + path);
  }
}

void FilerController::addToHistory(const QString &path) {
  // Remove any forward history
  while (m_navigationHistory.size() > m_historyIndex + 1) {
    m_navigationHistory.removeLast();
  }

  m_navigationHistory.append(path);
  m_historyIndex = m_navigationHistory.size() - 1;
  emit navigationHistoryChanged();
}

void FilerController::goBack() {
  if (canGoBack()) {
    m_historyIndex--;
    setCurrentPath(m_navigationHistory[m_historyIndex].toString());
    emit navigationHistoryChanged();
  }
}

void FilerController::goForward() {
  if (canGoForward()) {
    m_historyIndex++;
    setCurrentPath(m_navigationHistory[m_historyIndex].toString());
    emit navigationHistoryChanged();
  }
}

void FilerController::goUp() {
  QDir dir(m_currentPath);
  if (dir.cdUp()) {
    navigateTo(dir.absolutePath());
  }
}

void FilerController::goHome() { navigateTo(QDir::homePath()); }

void FilerController::goToDesktop() {
  navigateTo(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
}

void FilerController::goToDocuments() {
  navigateTo(
      QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
}

void FilerController::goToDownloads() {
  navigateTo(
      QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
}

void FilerController::goToPictures() {
  navigateTo(
      QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
}

void FilerController::goToMusic() {
  navigateTo(QStandardPaths::writableLocation(QStandardPaths::MusicLocation));
}

void FilerController::refreshFileList() {
  m_fileList.clear();

  QDir dir(m_currentPath);
  if (!dir.exists()) {
    emit fileListChanged();
    return;
  }

  // Get all entries
  QFileInfoList entries = dir.entryInfoList(
      QDir::AllEntries | QDir::NoDotAndDotDot, QDir::DirsFirst | QDir::Name);

  QMimeDatabase mimeDb;

  for (const QFileInfo &info : entries) {
    QVariantMap item;
    item["name"] = info.fileName();
    item["path"] = info.absoluteFilePath();
    item["isDir"] = info.isDir();
    item["isHidden"] = info.isHidden();
    item["size"] = formatFileSize(info.size());
    item["sizeBytes"] = info.size();
    item["modified"] = info.lastModified().toString("MMM d, yyyy");
    item["modifiedDate"] = info.lastModified();

    // Get icon based on type
    if (info.isDir()) {
      item["icon"] = "folder";
      item["type"] = "Folder";
    } else {
      QMimeType mime = mimeDb.mimeTypeForFile(info);
      item["icon"] = getMimeIcon(info.suffix());
      item["type"] = mime.comment();
      item["mimeType"] = mime.name();
    }

    item["selected"] = false;
    m_fileList.append(item);
  }

  emit fileListChanged();
}

void FilerController::openItem(int index) {
  if (index < 0 || index >= m_fileList.size()) {
    return;
  }

  QVariantMap item = m_fileList[index].toMap();
  QString path = item["path"].toString();

  openItemAtPath(path);
}

void FilerController::openItemAtPath(const QString &path) {
  QFileInfo info(path);

  if (info.isDir()) {
    // Navigate into directory
    navigateTo(path);
  } else {
    // Open file with default application
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    emit fileOpened(path);
    qDebug() << "[Filer] Opened file:" << path;
  }
}

bool FilerController::createFolder(const QString &name) {
  QDir dir(m_currentPath);
  if (dir.mkdir(name)) {
    qDebug() << "[Filer] Created folder:" << name;
    return true;
  }
  emit errorOccurred("Failed to create folder: " + name);
  return false;
}

bool FilerController::createFile(const QString &name) {
  QString path = m_currentPath + "/" + name;
  QFile file(path);
  if (file.open(QIODevice::WriteOnly)) {
    file.close();
    qDebug() << "[Filer] Created file:" << name;
    return true;
  }
  emit errorOccurred("Failed to create file: " + name);
  return false;
}

bool FilerController::renameItem(const QString &oldPath,
                                 const QString &newName) {
  QFileInfo info(oldPath);
  QString newPath = info.dir().absolutePath() + "/" + newName;

  if (QFile::rename(oldPath, newPath)) {
    qDebug() << "[Filer] Renamed:" << oldPath << "to" << newPath;
    return true;
  }
  emit errorOccurred("Failed to rename: " + info.fileName());
  return false;
}

bool FilerController::moveToTrash(const QVariantList &paths) {
  bool success = true;
  for (const QVariant &v : paths) {
    QString path = v.toString();
    if (!QFile::moveToTrash(path)) {
      emit errorOccurred("Failed to trash: " + path);
      success = false;
    }
  }
  return success;
}

bool FilerController::copyItems(const QVariantList &paths,
                                const QString &destination) {
  bool success = true;
  for (const QVariant &v : paths) {
    QString src = v.toString();
    QFileInfo info(src);
    QString dst = destination + "/" + info.fileName();

    if (info.isDir()) {
      // TODO: Recursive directory copy
      emit errorOccurred("Directory copy not yet implemented");
      success = false;
    } else {
      if (!QFile::copy(src, dst)) {
        emit errorOccurred("Failed to copy: " + info.fileName());
        success = false;
      }
    }
  }
  return success;
}

bool FilerController::moveItems(const QVariantList &paths,
                                const QString &destination) {
  bool success = true;
  for (const QVariant &v : paths) {
    QString src = v.toString();
    QFileInfo info(src);
    QString dst = destination + "/" + info.fileName();

    if (!QFile::rename(src, dst)) {
      emit errorOccurred("Failed to move: " + info.fileName());
      success = false;
    }
  }
  return success;
}

void FilerController::selectItem(int index, bool append) {
  if (index < 0 || index >= m_fileList.size()) {
    return;
  }

  if (!append) {
    // Clear existing selection
    for (int i = 0; i < m_fileList.size(); i++) {
      QVariantMap item = m_fileList[i].toMap();
      item["selected"] = false;
      m_fileList[i] = item;
    }
    m_selectedItems.clear();
  }

  QVariantMap item = m_fileList[index].toMap();
  item["selected"] = true;
  m_fileList[index] = item;
  m_selectedItems.append(item["path"]);

  emit selectionChanged();
  emit fileListChanged();
}

void FilerController::selectAll() {
  m_selectedItems.clear();
  for (int i = 0; i < m_fileList.size(); i++) {
    QVariantMap item = m_fileList[i].toMap();
    item["selected"] = true;
    m_fileList[i] = item;
    m_selectedItems.append(item["path"]);
  }
  emit selectionChanged();
  emit fileListChanged();
}

void FilerController::clearSelection() {
  for (int i = 0; i < m_fileList.size(); i++) {
    QVariantMap item = m_fileList[i].toMap();
    item["selected"] = false;
    m_fileList[i] = item;
  }
  m_selectedItems.clear();
  emit selectionChanged();
  emit fileListChanged();
}

QVariantMap FilerController::getFileInfo(const QString &path) {
  QFileInfo info(path);
  QVariantMap result;

  result["name"] = info.fileName();
  result["path"] = info.absoluteFilePath();
  result["size"] = formatFileSize(info.size());
  result["sizeBytes"] = info.size();
  result["isDir"] = info.isDir();
  result["isReadable"] = info.isReadable();
  result["isWritable"] = info.isWritable();
  result["isExecutable"] = info.isExecutable();
  result["created"] = info.birthTime().toString(Qt::ISODate);
  result["modified"] = info.lastModified().toString(Qt::ISODate);
  result["owner"] = info.owner();
  result["group"] = info.group();

  return result;
}

void FilerController::onDirectoryChanged(const QString &path) {
  Q_UNUSED(path);
  qDebug() << "[Filer] Directory changed, refreshing...";
  refreshFileList();
}

QString FilerController::formatFileSize(qint64 bytes) const {
  if (bytes < 1024) {
    return QString::number(bytes) + " B";
  } else if (bytes < 1024 * 1024) {
    return QString::number(bytes / 1024.0, 'f', 1) + " KB";
  } else if (bytes < 1024 * 1024 * 1024) {
    return QString::number(bytes / (1024.0 * 1024.0), 'f', 1) + " MB";
  } else {
    return QString::number(bytes / (1024.0 * 1024.0 * 1024.0), 'f', 2) + " GB";
  }
}

QString FilerController::getMimeIcon(const QString &suffix) const {
  // Map file extensions to icon types
  QString ext = suffix.toLower();

  if (ext == "pdf")
    return "document-pdf";
  if (ext == "doc" || ext == "docx" || ext == "odt")
    return "document-text";
  if (ext == "xls" || ext == "xlsx" || ext == "ods")
    return "document-spreadsheet";
  if (ext == "ppt" || ext == "pptx" || ext == "odp")
    return "document-presentation";
  if (ext == "jpg" || ext == "jpeg" || ext == "png" || ext == "gif" ||
      ext == "bmp" || ext == "svg" || ext == "webp")
    return "image";
  if (ext == "mp3" || ext == "wav" || ext == "ogg" || ext == "flac" ||
      ext == "m4a")
    return "audio";
  if (ext == "mp4" || ext == "mkv" || ext == "avi" || ext == "mov" ||
      ext == "webm")
    return "video";
  if (ext == "zip" || ext == "tar" || ext == "gz" || ext == "7z" ||
      ext == "rar")
    return "archive";
  if (ext == "txt" || ext == "md" || ext == "log")
    return "text";
  if (ext == "cpp" || ext == "c" || ext == "h" || ext == "hpp" || ext == "py" ||
      ext == "js" || ext == "ts" || ext == "rs" || ext == "go" || ext == "java")
    return "code";
  if (ext == "html" || ext == "css" || ext == "xml" || ext == "json")
    return "web";
  if (ext == "sh" || ext == "bash" || ext == "zsh")
    return "script";
  if (ext == "nix")
    return "nix";
  if (ext == "iso" || ext == "img")
    return "disk";

  return "file"; // Default
}
