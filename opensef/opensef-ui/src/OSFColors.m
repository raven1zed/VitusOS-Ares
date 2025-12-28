/**
 * OSFColors.m - OS1-inspired color palette
 * openSEF Framework
 */

#import <opensef/OpenSEFUI.h>

@implementation OSFColors

// ============================================================================
// Primary Palette (OS1-inspired warm pastels)
// ============================================================================

+ (NSColor *)primary {
  // Soft coral - warm, inviting #FAB5AC
  return [NSColor colorWithRed:0.98 green:0.71 blue:0.67 alpha:1.0];
}

+ (NSColor *)secondary {
  // Muted teal - calm, stable #8FC2C2
  return [NSColor colorWithRed:0.56 green:0.76 blue:0.76 alpha:1.0];
}

+ (NSColor *)accent {
  // Warm peach - friendly #FFD9BA
  return [NSColor colorWithRed:1.0 green:0.85 blue:0.73 alpha:1.0];
}

// ============================================================================
// Backgrounds
// ============================================================================

+ (NSColor *)background {
  if ([OSFTheme currentTheme].isDark) {
    // Deep charcoal with warmth #1C1C1E
    return [NSColor colorWithRed:0.11 green:0.11 blue:0.12 alpha:1.0];
  }
  // Warm off-white #FAF8F5
  return [NSColor colorWithRed:0.98 green:0.97 blue:0.96 alpha:1.0];
}

+ (NSColor *)surface {
  if ([OSFTheme currentTheme].isDark) {
    return [NSColor colorWithRed:0.16 green:0.16 blue:0.18 alpha:0.8];
  }
  return [NSColor colorWithRed:1.0 green:1.0 blue:1.0 alpha:0.8];
}

+ (NSColor *)elevated {
  if ([OSFTheme currentTheme].isDark) {
    return [NSColor colorWithRed:0.22 green:0.22 blue:0.24 alpha:0.95];
  }
  return [NSColor colorWithRed:1.0 green:1.0 blue:1.0 alpha:0.95];
}

// ============================================================================
// Text
// ============================================================================

+ (NSColor *)textPrimary {
  if ([OSFTheme currentTheme].isDark) {
    return [NSColor colorWithRed:0.97 green:0.97 blue:0.97 alpha:1.0];
  }
  return [NSColor colorWithRed:0.13 green:0.13 blue:0.14 alpha:1.0];
}

+ (NSColor *)textSecondary {
  if ([OSFTheme currentTheme].isDark) {
    return [NSColor colorWithRed:0.70 green:0.70 blue:0.72 alpha:1.0];
  }
  return [NSColor colorWithRed:0.45 green:0.45 blue:0.47 alpha:1.0];
}

+ (NSColor *)textDisabled {
  return [NSColor colorWithRed:0.60 green:0.60 blue:0.62 alpha:0.5];
}

// ============================================================================
// Semantic
// ============================================================================

+ (NSColor *)success {
  return [NSColor colorWithRed:0.52 green:0.78 blue:0.52 alpha:1.0];
}

+ (NSColor *)warning {
  return [NSColor colorWithRed:0.95 green:0.77 blue:0.42 alpha:1.0];
}

+ (NSColor *)error {
  return [NSColor colorWithRed:0.90 green:0.45 blue:0.45 alpha:1.0];
}

@end
