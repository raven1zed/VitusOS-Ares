#ifndef ICON_PROVIDER_H
#define ICON_PROVIDER_H

#include <QColor>
#include <QFont>
#include <QIcon>
#include <QLinearGradient>
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
      // Professional Fusion Fallback
      QPixmap pixmap(width, height);
      pixmap.fill(Qt::transparent);

      QPainter painter(&pixmap);
      painter.setRenderHint(QPainter::Antialiasing);

      // Glassmorphism Placeholder Circle
      QLinearGradient gradient(0, 0, width, height);
      gradient.setColorAt(0, QColor("#3D5A80")); // Mission Blue
      gradient.setColorAt(1, QColor("#1A1A1A"));

      painter.setBrush(gradient);
      painter.setPen(Qt::NoPen);
      painter.drawEllipse(2, 2, width - 4, height - 4);

      // Text initial
      painter.setPen(Qt::white);
      QFont font = painter.font();
      font.setBold(true);
      font.setPixelSize(height / 2);
      painter.setFont(font);
      painter.drawText(pixmap.rect(), Qt::AlignCenter, id.left(1).toUpper());

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
