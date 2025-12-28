/**
 * OSFView.m - Base view implementation
 * openSEF Framework
 */

#import <opensef/OpenSEFAppKit.h>

@implementation OSFView

- (instancetype)init {
  self = [super init];
  if (self) {
    _frame = CGRectZero;
    _subviews = [NSMutableArray array];
    _alpha = 1.0;
    _hidden = NO;
  }
  return self;
}

- (void)addSubview:(OSFView *)view {
  view.superview = self;
  [self.subviews addObject:view];
}

- (void)removeFromSuperview {
  [self.superview.subviews removeObject:self];
  self.superview = nil;
}

- (void)draw:(CGContextRef)context {
  // Override in subclasses for custom drawing
  for (OSFView *subview in self.subviews) {
    if (!subview.hidden) {
      [subview draw:context];
    }
  }
}

@end
