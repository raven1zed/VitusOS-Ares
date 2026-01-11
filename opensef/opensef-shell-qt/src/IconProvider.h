#ifndef ICON_PROVIDER_H
#define ICON_PROVIDER_H

#include <QColor>
#include <QIcon>
#include <QPainter>
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
      // Fallback: Generate a colored pixmap
      QPixmap pixmap(width, height);
      pixmap.fill(QColor("#4A9FD4")); // Vitus Blue default

      QPainter painter(&pixmap);
      painter.setPen(Qt::white);
      painter.drawRect(0, 0, width - 1, height - 1);
      return pixmap;
    }

    QPixmap pixmap = icon.pixmap(width, height);
    if (size) {
      *size = pixmap.size();
    }
    return pixmap;
  }
};

#endif // ICON_PROVIDER_H
