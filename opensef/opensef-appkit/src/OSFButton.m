/**
 * OSFButton.m - Clickable button implementation
 * openSEF Framework
 */

#import <opensef/OpenSEFAppKit.h>

@implementation OSFButton

+ (instancetype)buttonWithLabel:(NSString *)label
                         action:(void (^)(void))action {
  OSFButton *button = [[self alloc] init];
  button.label = label;
  button.action = action;
  return button;
}

- (void)draw:(CGContextRef)context {
  // TODO: Draw button with OS1 styling
  // - Rounded corners (8pt radius)
  // - Subtle gradient
  // - Inter font label centered
  [super draw:context];
}

- (void)handleClick {
  if (self.action) {
    self.action();
  }
}

@end
