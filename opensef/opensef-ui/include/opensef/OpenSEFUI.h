/**
 * openSEF UI: Ares Design System (C++ Version)
 *
 * Colors, typography, spacing for VitusOS
 * Theme: Ares (The Martian) - Space Orange + Lunar Gray
 */

#pragma once

#include <opensef/OpenSEFBase.h>

namespace opensef {

// ============================================================================
// OSFColors - Ares Color Palette
// ============================================================================

class OSFColors {
public:
  // Primary (Ares/Martian tones)
  static OSFColor primary() {
    return OSFColor::fromHex(0xE85D04);
  } // Space Orange
  static OSFColor secondary() {
    return OSFColor::fromHex(0x3D5A80);
  } // Mission Blue
  static OSFColor accent() { return OSFColor::fromHex(0xFB8500); } // Mars Dust

  // Backgrounds
  static OSFColor background() {
    return OSFColor::fromHex(0xF0F0F0);
  } // Lunar Gray
  static OSFColor backgroundDark() {
    return OSFColor::fromHex(0x1A1A2E);
  } // Space Charcoal
  static OSFColor surface() { return OSFColor::fromHex(0xFAFAFA, 0.85f); }
  static OSFColor elevated() { return OSFColor::fromHex(0xFFFFFF, 0.95f); }

  // Text
  static OSFColor textPrimary() { return OSFColor::fromHex(0x1A1A2E); }
  static OSFColor textSecondary() { return OSFColor::fromHex(0x6B7280); }
  static OSFColor textDisabled() { return OSFColor::fromHex(0x9CA3AF); }

  // Semantic
  static OSFColor success() { return OSFColor::fromHex(0x10B981); }
  static OSFColor warning() { return OSFColor::fromHex(0xF59E0B); }
  static OSFColor error() { return OSFColor::fromHex(0xEF4444); }

  // Ares extras
  static OSFColor marsRed() { return OSFColor::fromHex(0xC1440E); }
  static OSFColor spaceBlack() { return OSFColor::fromHex(0x0D0D1A); }
  static OSFColor nasaBlue() { return OSFColor::fromHex(0x0B3D91); }
};

// ============================================================================
// OSFSpacing - Consistent spacing scale
// ============================================================================

class OSFSpacing {
public:
  static float xs() { return 4.0f; }
  static float sm() { return 8.0f; }
  static float md() { return 16.0f; }
  static float lg() { return 24.0f; }
  static float xl() { return 32.0f; }
  static float xxl() { return 48.0f; }
};

// ============================================================================
// OSFStyle - Design constants
// ============================================================================

class OSFStyle {
public:
  // Corner radius
  static float cornerRadiusSmall() { return 4.0f; }
  static float cornerRadiusMedium() { return 8.0f; }
  static float cornerRadiusLarge() { return 16.0f; }

  // Blur
  static float blurRadiusLight() { return 8.0f; }
  static float blurRadiusMedium() { return 16.0f; }
  static float blurRadiusHeavy() { return 24.0f; }

  // Shadow
  static float shadowBlur() { return 12.0f; }
  static OSFPoint shadowOffset() { return OSFPoint(0, 2); }
  static float shadowOpacity() { return 0.15f; }
};

// ============================================================================
// OSFAnimation - Animation presets
// ============================================================================

class OSFAnimation {
public:
  static float durationFast() { return 0.1f; }
  static float durationNormal() { return 0.2f; }
  static float durationSlow() { return 0.3f; }
};

} // namespace opensef
