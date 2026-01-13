#pragma once

#include "opensef/NSWindowButton.h"
#include <QQuickPaintedItem>
#include <memory>

/**
 * QQuickWindowButton - Qt Quick renderer for NSWindowButton
 *
 * Renders circular traffic light buttons via Qt Quick Scene Graph (Vulkan)
 */
class QQuickWindowButton : public QQuickPaintedItem {
  Q_OBJECT
  Q_PROPERTY(int buttonType READ buttonType WRITE setButtonType NOTIFY
                 buttonTypeChanged)
  Q_PROPERTY(bool hovered READ isHovered NOTIFY hoveredChanged)

public:
  enum ButtonType { CloseButton = 0, MinimizeButton = 1, MaximizeButton = 2 };
  Q_ENUM(ButtonType)

  explicit QQuickWindowButton(QQuickItem *parent = nullptr);
  virtual ~QQuickWindowButton();

  int buttonType() const;
  void setButtonType(int type);

  bool isHovered() const;

signals:
  void buttonTypeChanged();
  void hoveredChanged();
  void clicked();

protected:
  void paint(QPainter *painter) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void hoverEnterEvent(QHoverEvent *event) override;
  void hoverLeaveEvent(QHoverEvent *event) override;

private:
  void renderButton(QPainter *painter);
  void renderSymbol(QPainter *painter);

  std::unique_ptr<OpenSEF::NSWindowButton> m_button;
  bool m_isPressed = false;
  bool m_showSymbol = false;
};

/**
 * QQuickTrafficLights - Row of three window buttons
 *
 * Convenience component that creates Close, Minimize, Maximize buttons
 */
class QQuickTrafficLights : public QQuickItem {
  Q_OBJECT
  Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)

public:
  explicit QQuickTrafficLights(QQuickItem *parent = nullptr);

  bool isActive() const { return m_active; }
  void setActive(bool active);

signals:
  void activeChanged();
  void closeClicked();
  void minimizeClicked();
  void maximizeClicked();

private:
  void createButtons();

  QQuickWindowButton *m_closeButton = nullptr;
  QQuickWindowButton *m_minimizeButton = nullptr;
  QQuickWindowButton *m_maximizeButton = nullptr;
  bool m_active = true;
};
