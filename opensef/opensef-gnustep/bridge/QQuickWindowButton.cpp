/**
 * QQuickWindowButton.cpp - Qt Quick Traffic Light Implementation
 */

#include "QQuickWindowButton.h"
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

// ============================================================================
// QQuickWindowButton
// ============================================================================

QQuickWindowButton::QQuickWindowButton(QQuickItem *parent)
    : QQuickPaintedItem(parent) {

  // Default to close button
  m_button = std::make_unique<OpenSEF::NSWindowButton>(
      OpenSEF::NSWindowButtonType::Close);

  setWidth(OpenSEF::NSWindowButton::ButtonSize);
  setHeight(OpenSEF::NSWindowButton::ButtonSize);
  setAcceptHoverEvents(true);
  setAcceptedMouseButtons(Qt::LeftButton);
  setAntialiasing(true);
}

QQuickWindowButton::~QQuickWindowButton() = default;

int QQuickWindowButton::buttonType() const {
  return static_cast<int>(m_button->windowButtonType());
}

void QQuickWindowButton::setButtonType(int type) {
  auto newType = static_cast<OpenSEF::NSWindowButtonType>(type);
  if (m_button->windowButtonType() != newType) {
    m_button = std::make_unique<OpenSEF::NSWindowButton>(newType);
    emit buttonTypeChanged();
    update();
  }
}

bool QQuickWindowButton::isHovered() const { return m_button->isHovered(); }

void QQuickWindowButton::paint(QPainter *painter) {
  renderButton(painter);
  if (m_button->isHovered()) {
    renderSymbol(painter);
  }
}

void QQuickWindowButton::renderButton(QPainter *painter) {
  painter->setRenderHint(QPainter::Antialiasing, true);

  // Get color from NSWindowButton
  uint32_t color = m_button->currentColor();
  QColor baseColor((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);

  QRectF rect = boundingRect();

  // 1. Subtle Border (0.5px dark ring)
  QPen borderPen(QColor(0, 0, 0, 30)); // 12% black opacity
  borderPen.setWidthF(1.0);
  painter->setPen(borderPen);

  // 2. Vertical Gradient for depth
  QLinearGradient gradient(rect.topLeft(), rect.bottomLeft());
  gradient.setColorAt(0.0, baseColor.lighter(105)); // Top highlight
  gradient.setColorAt(1.0, baseColor);              // Bottom base

  painter->setBrush(gradient);

  // Adjust rect for border width
  painter->drawEllipse(rect.adjusted(0.5, 0.5, -0.5, -0.5));

  // 3. Inner Shadow when pressed
  if (m_isPressed) {
    QColor shadow(0, 0, 0, 60);
    painter->setBrush(shadow);
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(rect.adjusted(1, 1, -1, -1));
  }
}

void QQuickWindowButton::renderSymbol(QPainter *painter) {
  painter->setRenderHint(QPainter::Antialiasing, true);

  QRectF rect = boundingRect();
  QPointF center = rect.center();

  QPen symbolPen(Qt::white);
  symbolPen.setWidthF(1.5);
  symbolPen.setCapStyle(Qt::RoundCap);
  painter->setPen(symbolPen);
  painter->setBrush(Qt::NoBrush);

  switch (m_button->windowButtonType()) {
  case OpenSEF::NSWindowButtonType::Close: {
    // X symbol
    qreal size = 3.0;
    painter->drawLine(QPointF(center.x() - size, center.y() - size),
                      QPointF(center.x() + size, center.y() + size));
    painter->drawLine(QPointF(center.x() + size, center.y() - size),
                      QPointF(center.x() - size, center.y() + size));
    break;
  }
  case OpenSEF::NSWindowButtonType::Minimize: {
    // Horizontal line
    qreal size = 3.0;
    painter->drawLine(QPointF(center.x() - size, center.y()),
                      QPointF(center.x() + size, center.y()));
    break;
  }
  case OpenSEF::NSWindowButtonType::Maximize: {
    // Small square
    qreal size = 2.5;
    painter->drawRect(
        QRectF(center.x() - size, center.y() - size, size * 2, size * 2));
    break;
  }
  }
}

void QQuickWindowButton::mousePressEvent(QMouseEvent *event) {
  m_isPressed = true;
  update();
  event->accept();
}

void QQuickWindowButton::mouseReleaseEvent(QMouseEvent *event) {
  if (m_isPressed && boundingRect().contains(event->position())) {
    emit clicked();
  }
  m_isPressed = false;
  update();
  event->accept();
}

void QQuickWindowButton::hoverEnterEvent(QHoverEvent *event) {
  m_button->setHovered(true);
  emit hoveredChanged();
  update();
  event->accept();
}

void QQuickWindowButton::hoverLeaveEvent(QHoverEvent *event) {
  m_button->setHovered(false);
  emit hoveredChanged();
  update();
  event->accept();
}

// ============================================================================
// QQuickTrafficLights
// ============================================================================

QQuickTrafficLights::QQuickTrafficLights(QQuickItem *parent)
    : QQuickItem(parent) {

  // Total width: 3 buttons (12px each) + 2 gaps (8px each) = 52px
  setWidth(OpenSEF::NSWindowButton::ButtonSize * 3 +
           OpenSEF::NSWindowButton::ButtonSpacing * 2);
  setHeight(OpenSEF::NSWindowButton::ButtonSize);

  createButtons();
}

void QQuickTrafficLights::createButtons() {
  int x = 0;

  // Close button
  m_closeButton = new QQuickWindowButton(this);
  m_closeButton->setButtonType(QQuickWindowButton::CloseButton);
  m_closeButton->setX(x);
  m_closeButton->setY(0);
  connect(m_closeButton, &QQuickWindowButton::clicked, this,
          &QQuickTrafficLights::closeClicked);

  x += OpenSEF::NSWindowButton::ButtonSize +
       OpenSEF::NSWindowButton::ButtonSpacing;

  // Minimize button
  m_minimizeButton = new QQuickWindowButton(this);
  m_minimizeButton->setButtonType(QQuickWindowButton::MinimizeButton);
  m_minimizeButton->setX(x);
  m_minimizeButton->setY(0);
  connect(m_minimizeButton, &QQuickWindowButton::clicked, this,
          &QQuickTrafficLights::minimizeClicked);

  x += OpenSEF::NSWindowButton::ButtonSize +
       OpenSEF::NSWindowButton::ButtonSpacing;

  // Maximize button
  m_maximizeButton = new QQuickWindowButton(this);
  m_maximizeButton->setButtonType(QQuickWindowButton::MaximizeButton);
  m_maximizeButton->setX(x);
  m_maximizeButton->setY(0);
  connect(m_maximizeButton, &QQuickWindowButton::clicked, this,
          &QQuickTrafficLights::maximizeClicked);
}

void QQuickTrafficLights::setActive(bool active) {
  if (m_active != active) {
    m_active = active;
    // Dim buttons when inactive
    if (m_closeButton)
      m_closeButton->setOpacity(active ? 1.0 : 0.5);
    if (m_minimizeButton)
      m_minimizeButton->setOpacity(active ? 1.0 : 0.5);
    if (m_maximizeButton)
      m_maximizeButton->setOpacity(active ? 1.0 : 0.5);
    emit activeChanged();
  }
}
