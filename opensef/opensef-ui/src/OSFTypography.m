/**
 * OSFTypography.m - Font system + spacing + animation
 * openSEF Framework
 */

#import <opensef/OpenSEFUI.h>

static NSString *const kFontFamily = @"Inter";

// ============================================================================
// OSFTypography
// ============================================================================

@implementation OSFTypography

+ (NSFont *)displayLarge {
  return [NSFont fontWithName:kFontFamily size:57]
             ?: [NSFont systemFontOfSize:57];
}
+ (NSFont *)displayMedium {
  return [NSFont fontWithName:kFontFamily size:45]
             ?: [NSFont systemFontOfSize:45];
}
+ (NSFont *)displaySmall {
  return [NSFont fontWithName:kFontFamily size:36]
             ?: [NSFont systemFontOfSize:36];
}

+ (NSFont *)headlineLarge {
  return [NSFont fontWithName:kFontFamily size:32]
             ?: [NSFont systemFontOfSize:32];
}
+ (NSFont *)headlineMedium {
  return [NSFont fontWithName:kFontFamily size:28]
             ?: [NSFont systemFontOfSize:28];
}
+ (NSFont *)headlineSmall {
  return [NSFont fontWithName:kFontFamily size:24]
             ?: [NSFont systemFontOfSize:24];
}

+ (NSFont *)bodyLarge {
  return [NSFont fontWithName:kFontFamily size:16]
             ?: [NSFont systemFontOfSize:16];
}
+ (NSFont *)bodyMedium {
  return [NSFont fontWithName:kFontFamily size:14]
             ?: [NSFont systemFontOfSize:14];
}
+ (NSFont *)bodySmall {
  return [NSFont fontWithName:kFontFamily size:12]
             ?: [NSFont systemFontOfSize:12];
}

+ (NSFont *)labelLarge {
  return [NSFont fontWithName:kFontFamily size:14]
             ?: [NSFont systemFontOfSize:14];
}
+ (NSFont *)labelMedium {
  return [NSFont fontWithName:kFontFamily size:12]
             ?: [NSFont systemFontOfSize:12];
}
+ (NSFont *)labelSmall {
  return [NSFont fontWithName:kFontFamily size:11]
             ?: [NSFont systemFontOfSize:11];
}

@end

// ============================================================================
// OSFSpacing
// ============================================================================

@implementation OSFSpacing

+ (CGFloat)xs {
  return 4.0;
}
+ (CGFloat)sm {
  return 8.0;
}
+ (CGFloat)md {
  return 16.0;
}
+ (CGFloat)lg {
  return 24.0;
}
+ (CGFloat)xl {
  return 32.0;
}
+ (CGFloat)xxl {
  return 48.0;
}

@end

// ============================================================================
// OSFAnimation
// ============================================================================

@implementation OSFAnimation

+ (NSTimeInterval)durationFast {
  return 0.1;
}
+ (NSTimeInterval)durationNormal {
  return 0.2;
}
+ (NSTimeInterval)durationSlow {
  return 0.3;
}

+ (CAMediaTimingFunction *)easeOut {
  return [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseOut];
}

+ (CAMediaTimingFunction *)easeInOut {
  return [CAMediaTimingFunction
      functionWithName:kCAMediaTimingFunctionEaseInEaseOut];
}

+ (CAMediaTimingFunction *)spring {
  return [CAMediaTimingFunction functionWithControlPoints:0.5:1.8:0.5:1.0];
}

@end

// ============================================================================
// OSFStyle
// ============================================================================

@implementation OSFStyle

+ (CGFloat)cornerRadiusSmall {
  return 4.0;
}
+ (CGFloat)cornerRadiusMedium {
  return 8.0;
}
+ (CGFloat)cornerRadiusLarge {
  return 16.0;
}

+ (CGFloat)blurRadiusLight {
  return 8.0;
}
+ (CGFloat)blurRadiusMedium {
  return 16.0;
}
+ (CGFloat)blurRadiusHeavy {
  return 24.0;
}

+ (CGFloat)shadowBlur {
  return 12.0;
}
+ (CGPoint)shadowOffset {
  return CGPointMake(0, 2);
}
+ (CGFloat)shadowOpacity {
  return 0.15;
}

@end
