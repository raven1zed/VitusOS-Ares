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
  QColor qcolor((color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF);

  // Draw circular button
  painter->setPen(Qt::NoPen);
  painter->setBrush(qcolor);

  QRectF rect = boundingRect();
  painter->drawEllipse(rect);

  // Subtle inner shadow when pressed
  if (m_isPressed) {
    QColor shadow(0, 0, 0, 40);
    painter->setBrush(shadow);
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
