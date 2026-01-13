#include "QQuickNSButton.h"
#include <QDebug>
#include <QFont>
#include <QFontMetrics>
#include <QMouseEvent>


QQuickNSButton::QQuickNSButton(QQuickItem *parent)
    : QQuickPaintedItem(parent), m_button(std::make_unique<NSButton>()) {

  setAcceptedMouseButtons(Qt::LeftButton);
  setAcceptHoverEvents(true);
  setFlag(QQuickItem::ItemHasContents, true);

  // Default size
  setImplicitWidth(100);
  setImplicitHeight(30);

  // Connect C++ button action to QML signal
  m_button->setAction([this]() { emit clicked(); });
}

QQuickNSButton::~QQuickNSButton() {}

// Properties
QString QQuickNSButton::title() const {
  return QString::fromStdString(m_button->title());
}

void QQuickNSButton::setTitle(const QString &title) {
  std::string newTitle = title.toStdString();
  if (m_button->title() != newTitle) {
    m_button->setTitle(newTitle);
    emit titleChanged();
    update();
  }
}

int QQuickNSButton::state() const { return m_button->state(); }

void QQuickNSButton::setState(int state) {
  if (m_button->state() != state) {
    m_button->setState(state);
    emit stateChanged();
    update();
  }
}

bool QQuickNSButton::bordered() const { return m_button->isBordered(); }

void QQuickNSButton::setBordered(bool bordered) {
  if (m_button->isBordered() != bordered) {
    m_button->setBordered(bordered);
    emit borderedChanged();
    update();
  }
}

QString QQuickNSButton::iconName() const {
  return QString::fromStdString(m_button->iconName());
}

void QQuickNSButton::setIconName(const QString &iconName) {
  std::string newIcon = iconName.toStdString();
  if (m_button->iconName() != newIcon) {
    m_button->setIconName(newIcon);
    emit iconNameChanged();
    update();
  }
}

QString QQuickNSButton::bezelStyle() const {
  // Convert enum to string
  switch (m_button->bezelStyle()) {
  case NSBezelStyle::Rounded:
    return "rounded";
  case NSBezelStyle::RegularSquare:
    return "square";
  case NSBezelStyle::Circular:
    return "circular";
  default:
    return "rounded";
  }
}

void QQuickNSButton::setBezelStyle(const QString &style) {
  NSBezelStyle newStyle = NSBezelStyle::Rounded;
  if (style == "square")
    newStyle = NSBezelStyle::RegularSquare;
  else if (style == "circular")
    newStyle = NSBezelStyle::Circular;

  if (m_button->bezelStyle() != newStyle) {
    m_button->setBezelStyle(newStyle);
    emit bezelStyleChanged();
    update();
  }
}

bool QQuickNSButton::isEnabled() const { return m_button->isEnabled(); }

void QQuickNSButton::setEnabled(bool enabled) {
  if (m_button->isEnabled() != enabled) {
    m_button->setEnabled(enabled);
    emit enabledChanged();
    update();
  }
}

// Rendering
void QQuickNSButton::paint(QPainter *painter) { renderButton(painter); }

void QQuickNSButton::renderButton(QPainter *painter) {
  painter->setRenderHint(QPainter::Antialiasing);
  painter->setRenderHint(QPainter::TextAntialiasing);

  QRectF rect = boundingRect();

  // Render components
  if (!m_button->isTransparent()) {
    renderBackground(painter);
  }

  if (m_button->isBordered()) {
    renderBorder(painter);
  }

  if (!m_button->iconName().empty()) {
    renderIcon(painter);
  }

  if (!m_button->title().empty()) {
    renderText(painter);
  }
}

void QQuickNSButton::renderBackground(QPainter *painter) {
  QRectF rect = boundingRect();

  // Determine background color based on state
  QColor bgColor;
  if (!m_button->isEnabled()) {
    bgColor = QColor(240, 240, 240); // Disabled
  } else if (m_isPressed) {
    bgColor = QColor(200, 200, 200); // Pressed
  } else if (m_isHovered) {
    bgColor = QColor(230, 230, 230); // Hovered
  } else if (m_button->state() == 1) {
    bgColor = QColor(0, 122, 255, 100); // Selected (blue tint)
  } else {
    bgColor = QColor(255, 255, 255, 180); // Normal
  }

  painter->setBrush(bgColor);
  painter->setPen(Qt::NoPen);

  // Rounded or circular
  if (m_button->bezelStyle() == NSBezelStyle::Circular) {
    painter->drawEllipse(rect);
  } else {
    qreal radius =
        (m_button->bezelStyle() == NSBezelStyle::Rounded) ? 6.0 : 2.0;
    painter->drawRoundedRect(rect, radius, radius);
  }
}

void QQuickNSButton::renderBorder(QPainter *painter) {
  QRectF rect = boundingRect();

  QColor borderColor =
      m_button->isEnabled() ? QColor(180, 180, 180) : QColor(220, 220, 220);

  painter->setPen(QPen(borderColor, 0.5));
  painter->setBrush(Qt::NoBrush);

  if (m_button->bezelStyle() == NSBezelStyle::Circular) {
    painter->drawEllipse(rect);
  } else {
    qreal radius =
        (m_button->bezelStyle() == NSBezelStyle::Rounded) ? 6.0 : 2.0;
    painter->drawRoundedRect(rect, radius, radius);
  }
}

void QQuickNSButton::renderText(QPainter *painter) {
  QRectF rect = boundingRect();

  // Font
  QFont font("Inter", 13);
  font.setWeight(QFont::DemiBold);
  painter->setFont(font);

  // Text color
  QColor textColor =
      m_button->isEnabled() ? QColor(26, 26, 26) : QColor(150, 150, 150);
  painter->setPen(textColor);

  // Draw text centered
  QString text = QString::fromStdString(m_button->title());
  painter->drawText(rect, Qt::AlignCenter, text);
}

void QQuickNSButton::renderIcon(QPainter *painter) {
  // TODO: Icon rendering (would load from icon theme)
  // For now, just leave space for future implementation
}

// Events
void QQuickNSButton::mousePressEvent(QMouseEvent *event) {
  if (!m_button->isEnabled())
    return;

  m_isPressed = true;
  update();
  event->accept();
}

void QQuickNSButton::mouseReleaseEvent(QMouseEvent *event) {
  if (!m_button->isEnabled())
    return;

  bool wasPressed = m_isPressed;
  m_isPressed = false;
  update();

  // Only trigger click if released while inside bounds
  if (wasPressed && boundingRect().contains(event->pos())) {
    m_button->performClick();
    emit clicked();
  }

  event->accept();
}

void QQuickNSButton::hoverEnterEvent(QHoverEvent *event) {
  m_isHovered = true;
  update();
}

void QQuickNSButton::hoverLeaveEvent(QHoverEvent *event) {
  m_isHovered = false;
  update();
}
