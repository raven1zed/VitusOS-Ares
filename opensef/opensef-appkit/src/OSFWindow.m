/**
 * OSFWindow.m - Top-level window implementation
 * openSEF Framework
 */

#import <opensef/OpenSEFAppKit.h>

@implementation OSFWindow

+ (instancetype)windowWithTitle:(NSString *)title frame:(CGRect)frame {
  OSFWindow *window = [[self alloc] init];
  window.title = title;
  window.frame = frame;
  window.visible = NO;
  return window;
}

- (instancetype)init {
  self = [super init];
  if (self) {
    _title = @"VitusOS Window";
    _visible = NO;
  }
  return self;
}

- (void)show {
  self.visible = YES;
  // TODO: Connect to opensef-backend Wayland surface
  NSLog(@"[openSEF] Window shown: %@", self.title);
}

- (void)close {
  self.visible = NO;
  NSLog(@"[openSEF] Window closed: %@", self.title);
}

- (void)setContentView:(OSFView *)view {
  [self.subviews removeAllObjects];
  [self addSubview:view];
}

@end
