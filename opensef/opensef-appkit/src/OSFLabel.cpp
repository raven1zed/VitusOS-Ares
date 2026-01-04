/**
 * OSFLabel.cpp - Text Display Widget
 */

#include <opensef/OpenSEFAppKit.h>
#include <pango/pangocairo.h>

namespace opensef {

namespace AresTheme {
constexpr const char *FontFamily = "Inter";
constexpr double FontSizeNormal = 13.0;
constexpr double FontSizeLarge = 16.0;
constexpr double FontSizeTitle = 20.0;
} // namespace AresTheme

OSFLabel::OSFLabel() = default;
OSFLabel::OSFLabel(const std::string &text) : text_(text) {}

std::shared_ptr<OSFLabel> OSFLabel::create(const std::string &text) {
  return std::make_shared<OSFLabel>(text);
}

std::shared_ptr<OSFLabel> OSFLabel::createTitle(const std::string &text) {
  auto label = std::make_shared<OSFLabel>(text);
  label->setFontSize(AresTheme::FontSizeTitle);
  label->setFontWeight(FontWeight::Bold);
  return label;
}

std::shared_ptr<OSFLabel> OSFLabel::createSubtitle(const std::string &text) {
  auto label = std::make_shared<OSFLabel>(text);
  label->setFontSize(AresTheme::FontSizeLarge);
  label->setTextColor(OSFColor(0.7, 0.7, 0.7, 1.0));
  return label;
}

void OSFLabel::setFontSize(double size) { fontSize_ = size; }
void OSFLabel::setFontWeight(FontWeight weight) { fontWeight_ = weight; }
void OSFLabel::setAlignment(TextAlignment alignment) { alignment_ = alignment; }

void OSFLabel::render(cairo_t *cr) {
  if (hidden_ || text_.empty())
    return;

  double x = frame_.x;
  double y = frame_.y;

  // Create Pango layout
  PangoLayout *layout = pango_cairo_create_layout(cr);
  pango_layout_set_text(layout, text_.c_str(), -1);

  // Create font description
  PangoFontDescription *fontDesc = pango_font_description_new();
  pango_font_description_set_family(fontDesc, AresTheme::FontFamily);
  pango_font_description_set_size(fontDesc,
                                  static_cast<int>(fontSize_ * PANGO_SCALE));
  pango_font_description_set_weight(
      fontDesc, fontWeight_ == FontWeight::Bold     ? PANGO_WEIGHT_BOLD
                : fontWeight_ == FontWeight::Medium ? PANGO_WEIGHT_MEDIUM
                                                    : PANGO_WEIGHT_NORMAL);

  pango_layout_set_font_description(layout, fontDesc);

  // Set width for wrapping
  if (frame_.width > 0) {
    pango_layout_set_width(layout,
                           static_cast<int>(frame_.width * PANGO_SCALE));
  }

  // Set alignment
  switch (alignment_) {
  case TextAlignment::Center:
    pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);
    break;
  case TextAlignment::Right:
    pango_layout_set_alignment(layout, PANGO_ALIGN_RIGHT);
    break;
  default:
    pango_layout_set_alignment(layout, PANGO_ALIGN_LEFT);
    break;
  }

  // Set color
  textColor_.setCairo(cr);

  // Render
  cairo_move_to(cr, x, y);
  pango_cairo_show_layout(cr, layout);

  // Cleanup
  pango_font_description_free(fontDesc);
  g_object_unref(layout);
}

} // namespace opensef
