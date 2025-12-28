/**
 * openSEF UI: OS1 Design System
 *
 * Colors, typography, spacing, and animations for VitusOS.
 * Inspired by OS1 (Her), Aqua, and Aero aesthetics.
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
// OSFColors - OS1 Color Palette
// ============================================================================

@interface OSFColors : NSObject

// Primary palette (OS1-inspired warm tones)
+ (NSColor *)primary;   // Soft coral #FAB5AC
+ (NSColor *)secondary; // Muted teal #8FC2C2
+ (NSColor *)accent;    // Warm peach #FFD9BA

// Backgrounds
+ (NSColor *)background; // Warm off-white / Deep charcoal
+ (NSColor *)surface;    // Glass panel base (80% alpha)
+ (NSColor *)elevated;   // Modal/popover (95% alpha)

// Text
+ (NSColor *)textPrimary;
+ (NSColor *)textSecondary;
+ (NSColor *)textDisabled;

// Semantic
+ (NSColor *)success;
+ (NSColor *)warning;
+ (NSColor *)error;

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
// OSFStyle - OS1 Design Constants
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
