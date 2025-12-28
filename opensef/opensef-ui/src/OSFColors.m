/**
 * OSFColors.m - Ares/Martian color palette
 *
 * Inspired by "The Martian" (2015) - Ares III mission
 * Gray lunar tones + Space Orange accent
 */

#import <opensef/OpenSEFUI.h>

@implementation OSFColors

// ============================================================================
// Primary Palette (Ares/Martian-inspired)
// ============================================================================

+ (NSColor *)primary {
  // Space Orange - Mars surface, Ares mission
  // #E85D04
  return [NSColor colorWithRed:0.91 green:0.36 blue:0.02 alpha:1.0];
}

+ (NSColor *)secondary {
  // Mission Blue - NASA mission control, space suits
  // #3D5A80
  return [NSColor colorWithRed:0.24 green:0.35 blue:0.50 alpha:1.0];
}

+ (NSColor *)accent {
  // Mars Dust - subtle orange highlight
  // #FB8500
  return [NSColor colorWithRed:0.98 green:0.52 blue:0.0 alpha:1.0];
}

// ============================================================================
// Backgrounds (Lunar Gray tones)
// ============================================================================

+ (NSColor *)background {
  if ([OSFTheme currentTheme].isDark) {
    // Deep space charcoal #1A1A2E
    return [NSColor colorWithRed:0.10 green:0.10 blue:0.18 alpha:1.0];
  }
  // Lunar gray - clean, technical #F0F0F0
  return [NSColor colorWithRed:0.94 green:0.94 blue:0.94 alpha:1.0];
}

+ (NSColor *)surface {
  if ([OSFTheme currentTheme].isDark) {
    // HAB module interior #252538
    return [NSColor colorWithRed:0.15 green:0.15 blue:0.22 alpha:0.85];
  }
  // Clean white panels
  return [NSColor colorWithRed:1.0 green:1.0 blue:1.0 alpha:0.85];
}

+ (NSColor *)elevated {
  if ([OSFTheme currentTheme].isDark) {
    // Elevated surface #2D2D44
    return [NSColor colorWithRed:0.18 green:0.18 blue:0.27 alpha:0.95];
  }
  return [NSColor colorWithRed:1.0 green:1.0 blue:1.0 alpha:0.95];
}

// ============================================================================
// Text
// ============================================================================

+ (NSColor *)textPrimary {
  if ([OSFTheme currentTheme].isDark) {
    // Clean white on dark
    return [NSColor colorWithRed:0.95 green:0.95 blue:0.97 alpha:1.0];
  }
  // Dark charcoal on light #1A1A2E
  return [NSColor colorWithRed:0.10 green:0.10 blue:0.18 alpha:1.0];
}

+ (NSColor *)textSecondary {
  if ([OSFTheme currentTheme].isDark) {
    // Muted gray #8D8D9B
    return [NSColor colorWithRed:0.55 green:0.55 blue:0.61 alpha:1.0];
  }
  // Medium gray #5A5A6E
  return [NSColor colorWithRed:0.35 green:0.35 blue:0.43 alpha:1.0];
}

+ (NSColor *)textDisabled {
  return [NSColor colorWithRed:0.50 green:0.50 blue:0.55 alpha:0.5];
}

// ============================================================================
// Semantic (Mission status colors)
// ============================================================================

+ (NSColor *)success {
  // Mission success green #06D6A0
  return [NSColor colorWithRed:0.02 green:0.84 blue:0.63 alpha:1.0];
}

+ (NSColor *)warning {
  // Caution orange (Mars dust storm) #FFB703
  return [NSColor colorWithRed:1.0 green:0.72 blue:0.01 alpha:1.0];
}

+ (NSColor *)error {
  // Critical red (mission abort) #EF476F
  return [NSColor colorWithRed:0.94 green:0.28 blue:0.44 alpha:1.0];
}

// ============================================================================
// Ares Mission Colors (additional)
// ============================================================================

+ (NSColor *)marsRed {
  // Actual Mars surface color #C1440E
  return [NSColor colorWithRed:0.76 green:0.27 blue:0.05 alpha:1.0];
}

+ (NSColor *)spaceBlack {
  // Deep space #0D0D1A
  return [NSColor colorWithRed:0.05 green:0.05 blue:0.10 alpha:1.0];
}

+ (NSColor *)nasaBlue {
  // NASA logo blue #0B3D91
  return [NSColor colorWithRed:0.04 green:0.24 blue:0.57 alpha:1.0];
}

@end
