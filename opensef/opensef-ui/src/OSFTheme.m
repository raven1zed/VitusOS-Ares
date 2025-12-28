/**
 * OSFTheme.m - Theme management
 * openSEF Framework
 */

#import <opensef/OpenSEFUI.h>

@implementation OSFTheme

static OSFTheme *_currentTheme = nil;

+ (instancetype)currentTheme {
  static dispatch_once_t onceToken;
  dispatch_once(&onceToken, ^{
    _currentTheme = [[self alloc] init];
    _currentTheme.mode = OSFThemeModeAuto;
  });
  return _currentTheme;
}

- (BOOL)isDark {
  if (self.mode == OSFThemeModeDark)
    return YES;
  if (self.mode == OSFThemeModeLight)
    return NO;
  // Auto: detect from system time or system preference
  return NO;
}

- (void)apply {
  NSLog(@"[openSEF] Applying theme: %@", self.isDark ? @"Dark" : @"Light");
}

@end
