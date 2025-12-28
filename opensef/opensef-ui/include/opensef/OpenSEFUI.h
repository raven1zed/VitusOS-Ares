/**
 * openSEF UI: Ares Design System
 *
 * Colors, typography, spacing, and animations for VitusOS.
 * Theme: Ares (The Martian) - Space Orange + Lunar Gray
 */

#ifndef OPENSEF_UI_H
#define OPENSEF_UI_H

#import <opensef/OpenSEFBase.h>

// ============================================================================
// OSFTheme - Current active theme
// ============================================================================

typedef NS_ENUM(NSInteger, OSFThemeMode) {
  OSFThemeModeLight,
  OSFThemeModeDark,
  OSFThemeModeAuto
};

@interface OSFTheme : OSFObject

@property(nonatomic) OSFThemeMode mode;
@property(nonatomic, readonly) BOOL isDark;

+ (instancetype)currentTheme;
- (void)apply;

@end

// ============================================================================
// OSFColors - Ares Color Palette (The Martian inspired)
// ============================================================================

@interface OSFColors : NSObject

// Primary palette (Ares/Martian tones)
+ (NSColor *)primary;   // Space Orange #E85D04
+ (NSColor *)secondary; // Mission Blue #3D5A80
+ (NSColor *)accent;    // Mars Dust #FB8500

// Backgrounds
+ (NSColor *)background; // Lunar Gray #F0F0F0 / Space Charcoal #1A1A2E
+ (NSColor *)surface;    // Glass panel base (85% alpha)
+ (NSColor *)elevated;   // Modal/popover (95% alpha)

// Text
+ (NSColor *)textPrimary;
+ (NSColor *)textSecondary;
+ (NSColor *)textDisabled;

// Semantic (Mission status)
+ (NSColor *)success; // Mission success green
+ (NSColor *)warning; // Caution orange
+ (NSColor *)error;   // Critical red

// Ares extras
+ (NSColor *)marsRed;    // Actual Mars surface #C1440E
+ (NSColor *)spaceBlack; // Deep space #0D0D1A
+ (NSColor *)nasaBlue;   // NASA logo blue #0B3D91

@end

// ============================================================================
// OSFTypography - Font System (Inter family)
// ============================================================================

@interface OSFTypography : NSObject

// Display (57pt, 45pt, 36pt)
+ (NSFont *)displayLarge;
+ (NSFont *)displayMedium;
+ (NSFont *)displaySmall;

// Headlines (32pt, 28pt, 24pt)
+ (NSFont *)headlineLarge;
+ (NSFont *)headlineMedium;
+ (NSFont *)headlineSmall;

// Body (16pt, 14pt, 12pt)
+ (NSFont *)bodyLarge;
+ (NSFont *)bodyMedium;
+ (NSFont *)bodySmall;

// Labels
+ (NSFont *)labelLarge;
+ (NSFont *)labelMedium;
+ (NSFont *)labelSmall;

@end

// ============================================================================
// OSFSpacing - Consistent spacing scale
// ============================================================================

@interface OSFSpacing : NSObject

+ (CGFloat)xs;  // 4pt
+ (CGFloat)sm;  // 8pt
+ (CGFloat)md;  // 16pt
+ (CGFloat)lg;  // 24pt
+ (CGFloat)xl;  // 32pt
+ (CGFloat)xxl; // 48pt

@end

// ============================================================================
// OSFAnimation - Animation presets
// ============================================================================

@interface OSFAnimation : NSObject

+ (NSTimeInterval)durationFast;   // 100ms
+ (NSTimeInterval)durationNormal; // 200ms
+ (NSTimeInterval)durationSlow;   // 300ms

+ (CAMediaTimingFunction *)easeOut;
+ (CAMediaTimingFunction *)easeInOut;
+ (CAMediaTimingFunction *)spring;

@end

// ============================================================================
// OSFStyle - Ares Design Constants
// ============================================================================

@interface OSFStyle : NSObject

// Corner radius
+ (CGFloat)cornerRadiusSmall;  // 4pt
+ (CGFloat)cornerRadiusMedium; // 8pt  Default for buttons/panels
+ (CGFloat)cornerRadiusLarge;  // 16pt For modals

// Blur radius for glass panels
+ (CGFloat)blurRadiusLight;  // 8pt
+ (CGFloat)blurRadiusMedium; // 16pt Default
+ (CGFloat)blurRadiusHeavy;  // 24pt

// Shadow
+ (CGFloat)shadowBlur;    // 12pt
+ (CGPoint)shadowOffset;  // (0, 2)
+ (CGFloat)shadowOpacity; // 15%

@end

#endif /* OPENSEF_UI_H */
