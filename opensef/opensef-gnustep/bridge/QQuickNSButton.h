#pragma once

#include "opensef/NSButton.h"
#include <QPainter>
#include <QQuickPaintedItem>
#include <memory>


/**
 * QQuickNSButton - Qt Quick renderer for NSButton
 *
 * Bridges C++ NSButton to Qt Quick Scene Graph (Vulkan rendering)
 */
class QQuickNSButton : public QQuickPaintedItem {
  Q_OBJECT
  Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
  Q_PROPERTY(int state READ state WRITE setState NOTIFY stateChanged)
  Q_PROPERTY(
      bool bordered READ bordered WRITE setBordered NOTIFY borderedChanged)
  Q_PROPERTY(
      QString iconName READ iconName WRITE setIconName NOTIFY iconNameChanged)
  Q_PROPERTY(QString bezelStyle READ bezelStyle WRITE setBezelStyle NOTIFY
                 bezelStyleChanged)
  Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)

public:
  explicit QQuickNSButton(QQuickItem *parent = nullptr);
  virtual ~QQuickNSButton();

  // Properties for QML
  QString title() const;
  void setTitle(const QString &title);

  int state() const;
  void setState(int state);

  bool bordered() const;
  void setBordered(bool bordered);

  QString iconName() const;
  void setIconName(const QString &iconName);

  QString bezelStyle() const;
  void setBezelStyle(const QString &style);

  bool isEnabled() const;
  void setEnabled(bool enabled);

  // Access underlying C++ button
  NSButton *button() const { return m_button.get(); }

signals:
  void titleChanged();
  void stateChanged();
  void borderedChanged();
  void iconNameChanged();
  void bezelStyleChanged();
  void enabledChanged();
  void clicked();

protected:
  // Qt Quick rendering
  void paint(QPainter *painter) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void hoverEnterEvent(QHoverEvent *event) override;
  void hoverLeaveEvent(QHoverEvent *event) override;

private:
  void renderButton(QPainter *painter);
  void renderBackground(QPainter *painter);
  void renderBorder(QPainter *painter);
  void renderText(QPainter *painter);
  void renderIcon(QPainter *painter);

  std::unique_ptr<NSButton> m_button;
  bool m_isPressed = false;
  bool m_isHovered = false;
};
