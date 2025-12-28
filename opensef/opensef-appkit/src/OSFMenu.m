/**
 * OSFMenu.m - Menu implementation
 */

#import <opensef/OpenSEFAppKit.h>

// ============================================================================
// OSFMenu
// ============================================================================

@implementation OSFMenu

- (instancetype)initWithTitle:(NSString *)title {
  self = [super init];
  if (self) {
    _title = [title copy];
    _items = [NSMutableArray array];
  }
  return self;
}

- (void)addItem:(id)item {
  [_items addObject:item];
}

- (void)addSeparator {
  // Add separator marker
  [_items addObject:[NSNull null]];
}

@end

// ============================================================================
// OSFMenuItem
// ============================================================================

@implementation OSFMenuItem

- (instancetype)initWithTitle:(NSString *)title {
  self = [super init];
  if (self) {
    _title = [title copy];
    _shortcut = nil;
    _enabled = YES;
    _checked = NO;
  }
  return self;
}

@end

// ============================================================================
// OSFMenuBar
// ============================================================================

@implementation OSFMenuBar

static OSFMenuBar *_sharedMenuBar = nil;

+ (instancetype)shared {
  static dispatch_once_t onceToken;
  dispatch_once(&onceToken, ^{
    _sharedMenuBar = [[OSFMenuBar alloc] init];
  });
  return _sharedMenuBar;
}

- (instancetype)init {
  self = [super init];
  if (self) {
    // TODO: Initialize D-Bus menu for global menu bar
    NSLog(@"[openSEF] Menu bar initialized");
  }
  return self;
}

- (void)addMenu:(OSFMenu *)menu {
  // TODO: Register menu with compositor/DE
  NSLog(@"[openSEF] Added menu: %@", menu.title);
}

- (void)removeMenu:(OSFMenu *)menu {
  // TODO: Unregister menu
  NSLog(@"[openSEF] Removed menu: %@", menu.title);
}

- (void)setApplicationMenu:(OSFMenu *)menu {
  // TODO: Set as application menu (leftmost)
  NSLog(@"[openSEF] Set application menu: %@", menu.title);
}

@end
