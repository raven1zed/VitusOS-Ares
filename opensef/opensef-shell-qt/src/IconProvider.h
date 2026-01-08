#ifndef ICON_PROVIDER_H
#define ICON_PROVIDER_H

#include <QIcon>
#include <QPixmap>
#include <QQuickImageProvider>


/**
 * IconProvider - Resolves image://icon/<name> URLs
 *
 * Fetches icons from the system theme (e.g. hicolor, papirus).
 */
class IconProvider : public QQuickImageProvider {
public:
  IconProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap) {}

  QPixmap requestPixmap(const QString &id, QSize *size,
                        const QSize &requestedSize) override {
    int width = requestedSize.width() > 0 ? requestedSize.width() : 32;
    int height = requestedSize.height() > 0 ? requestedSize.height() : 32;

    QIcon icon = QIcon::fromTheme(id);
    if (icon.isNull()) {
      return QPixmap();
    }

    QPixmap pixmap = icon.pixmap(width, height);
    if (size) {
      *size = pixmap.size();
    }
    return pixmap;
  }
};

#endif // ICON_PROVIDER_H
