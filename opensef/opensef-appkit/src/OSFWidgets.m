/**
 * OSFWidgets.m - Additional widget implementations
 * openSEF Framework
 */

#import <opensef/OpenSEFAppKit.h>
#import <opensef/OpenSEFUI.h>

// ============================================================================
// OSFTextField
// ============================================================================

@implementation OSFTextField

- (instancetype)init {
  self = [super init];
  if (self) {
    _text = @"";
    _placeholder = @"";
  }
  return self;
}

- (void)draw:(CGContextRef)context {
  // Draw background
  CGContextSetFillColorWithColor(context, [[OSFColors surface] CGColor]);
  CGContextFillRect(context, self.frame);

  // Draw border
  CGContextSetStrokeColorWithColor(context,
                                   [[OSFColors textSecondary] CGColor]);
  CGContextStrokeRect(context, self.frame);

  // TODO: Draw text or placeholder
  [super draw:context];
}

@end

// ============================================================================
// OSFLabel
// ============================================================================

@implementation OSFLabel

- (instancetype)init {
  self = [super init];
  if (self) {
    _text = @"";
    _font = [OSFTypography bodyMedium];
    _textColor = [OSFColors textPrimary];
  }
  return self;
}

- (void)draw:(CGContextRef)context {
  // TODO: Draw text with font and color
  [super draw:context];
}

@end

// ============================================================================
// OSFTableView
// ============================================================================

@implementation OSFTableView

- (instancetype)init {
  self = [super init];
  if (self) {
    _items = @[];
    _selectedIndex = -1;
  }
  return self;
}

- (void)reloadData {
  // TODO: Rebuild cell views
  NSLog(@"[openSEF] TableView reloading %lu items",
        (unsigned long)_items.count);
}

- (void)draw:(CGContextRef)context {
  CGFloat y = self.frame.origin.y;
  CGFloat rowHeight = 48.0;

  for (NSUInteger i = 0; i < _items.count; i++) {
    CGRect rowRect =
        CGRectMake(self.frame.origin.x, y, self.frame.size.width, rowHeight);

    // Highlight selected
    if ((NSInteger)i == _selectedIndex) {
      CGContextSetFillColorWithColor(context, [[OSFColors primary] CGColor]);
      CGContextFillRect(context, rowRect);
    }

    // TODO: Draw item text
    y += rowHeight;
  }

  [super draw:context];
}

@end

// ============================================================================
// OSFProgressBar
// ============================================================================

@implementation OSFProgressBar

- (instancetype)init {
  self = [super init];
  if (self) {
    _progress = 0.0;
    _indeterminate = NO;
    _trackColor = [OSFColors surface];
    _progressColor = [OSFColors primary]; // Space Orange
  }
  return self;
}

- (void)setProgress:(CGFloat)progress animated:(BOOL)animated {
  if (animated) {
    // TODO: Animate using OSFAnimation durations
  }
  _progress = MIN(1.0, MAX(0.0, progress));
}

- (void)draw:(CGContextRef)context {
  CGFloat cornerRadius = [OSFStyle cornerRadiusSmall];

  // Draw track
  CGContextSetFillColorWithColor(context, [_trackColor CGColor]);
  // TODO: Draw rounded rect for track

  // Draw progress
  CGFloat progressWidth = self.frame.size.width * _progress;
  CGRect progressRect = CGRectMake(self.frame.origin.x, self.frame.origin.y,
                                   progressWidth, self.frame.size.height);
  CGContextSetFillColorWithColor(context, [_progressColor CGColor]);
  CGContextFillRect(context, progressRect);

  [super draw:context];
}

@end

// ============================================================================
// OSFSwitch
// ============================================================================

@implementation OSFSwitch

- (instancetype)init {
  self = [super init];
  if (self) {
    _on = NO;
    _onColor = [OSFColors primary]; // Space Orange
  }
  return self;
}

- (void)setOn:(BOOL)on animated:(BOOL)animated {
  _on = on;
  if (_onToggle) {
    _onToggle(on);
  }
  // TODO: Animate thumb position
}

- (void)draw:(CGContextRef)context {
  CGFloat height = self.frame.size.height;
  CGFloat width = height * 1.8; // Standard switch aspect ratio

  // Draw track
  NSColor *trackColor = _on ? _onColor : [OSFColors textDisabled];
  CGContextSetFillColorWithColor(context, [trackColor CGColor]);
  // TODO: Draw rounded pill shape

  // Draw thumb (white circle)
  CGFloat thumbSize = height - 4;
  CGFloat thumbX = _on ? (width - thumbSize - 2) : 2;
  // TODO: Draw white circle at thumbX

  [super draw:context];
}

@end

// ============================================================================
// OSFSlider
// ============================================================================

@implementation OSFSlider

- (instancetype)init {
  self = [super init];
  if (self) {
    _value = 0.5;
    _minValue = 0.0;
    _maxValue = 1.0;
    _trackColor = [OSFColors surface];
    _thumbColor = [OSFColors primary];
  }
  return self;
}

- (void)draw:(CGContextRef)context {
  CGFloat trackHeight = 4.0;
  CGFloat thumbRadius = 8.0;

  // Draw track
  CGFloat trackY =
      self.frame.origin.y + (self.frame.size.height - trackHeight) / 2;
  CGRect trackRect = CGRectMake(self.frame.origin.x, trackY,
                                self.frame.size.width, trackHeight);
  CGContextSetFillColorWithColor(context, [_trackColor CGColor]);
  CGContextFillRect(context, trackRect);

  // Draw filled portion
  CGFloat normalizedValue = (_value - _minValue) / (_maxValue - _minValue);
  CGFloat filledWidth = self.frame.size.width * normalizedValue;
  CGRect filledRect =
      CGRectMake(self.frame.origin.x, trackY, filledWidth, trackHeight);
  CGContextSetFillColorWithColor(context, [[OSFColors primary] CGColor]);
  CGContextFillRect(context, filledRect);

  // Draw thumb
  CGFloat thumbX = self.frame.origin.x + filledWidth - thumbRadius;
  CGFloat thumbY =
      self.frame.origin.y + (self.frame.size.height - thumbRadius * 2) / 2;
  CGRect thumbRect =
      CGRectMake(thumbX, thumbY, thumbRadius * 2, thumbRadius * 2);
  CGContextSetFillColorWithColor(context, [_thumbColor CGColor]);
  CGContextFillEllipseInRect(context, thumbRect);

  [super draw:context];
}

@end

// ============================================================================
// OSFCheckbox
// ============================================================================

@implementation OSFCheckbox

- (instancetype)init {
  self = [super init];
  if (self) {
    _checked = NO;
    _label = @"";
  }
  return self;
}

- (void)draw:(CGContextRef)context {
  CGFloat boxSize = 20.0;
  CGFloat padding = 8.0;

  // Draw checkbox box
  CGRect boxRect =
      CGRectMake(self.frame.origin.x, self.frame.origin.y, boxSize, boxSize);

  if (_checked) {
    CGContextSetFillColorWithColor(context, [[OSFColors primary] CGColor]);
    CGContextFillRect(context, boxRect);
    // TODO: Draw checkmark
  } else {
    CGContextSetStrokeColorWithColor(context,
                                     [[OSFColors textSecondary] CGColor]);
    CGContextStrokeRect(context, boxRect);
  }

  // TODO: Draw label text

  [super draw:context];
}

@end

// ============================================================================
// OSFDropdown
// ============================================================================

@implementation OSFDropdown

- (instancetype)init {
  self = [super init];
  if (self) {
    _options = @[];
    _selectedIndex = -1;
    _placeholder = @"Select...";
  }
  return self;
}

- (void)draw:(CGContextRef)context {
  // Draw background
  CGContextSetFillColorWithColor(context, [[OSFColors surface] CGColor]);
  CGContextFillRect(context, self.frame);

  // Draw border
  CGContextSetStrokeColorWithColor(context,
                                   [[OSFColors textSecondary] CGColor]);
  CGContextStrokeRect(context, self.frame);

  // TODO: Draw selected text or placeholder
  // TODO: Draw dropdown arrow

  [super draw:context];
}

@end

// ============================================================================
// OSFImageView
// ============================================================================

@implementation OSFImageView

- (instancetype)init {
  self = [super init];
  if (self) {
    _cornerRadius = 0.0;
    _aspectFit = YES;
  }
  return self;
}

- (void)draw:(CGContextRef)context {
  if (_image) {
    // TODO: Draw image with aspect ratio and corner radius
  }
  [super draw:context];
}

@end

// ============================================================================
// OSFScrollView
// ============================================================================

@implementation OSFScrollView

- (instancetype)init {
  self = [super init];
  if (self) {
    _contentOffset = CGPointZero;
    _contentSize = CGSizeZero;
    _showsVerticalScrollIndicator = YES;
    _showsHorizontalScrollIndicator = NO;
  }
  return self;
}

- (void)draw:(CGContextRef)context {
  // Apply content offset transform
  CGContextSaveGState(context);
  CGContextTranslateCTM(context, -_contentOffset.x, -_contentOffset.y);

  // Draw content
  if (_contentView) {
    [_contentView draw:context];
  }

  CGContextRestoreGState(context);

  // TODO: Draw scroll indicators

  [super draw:context];
}

@end

// ============================================================================
// OSFGlassPanel
// ============================================================================

@implementation OSFGlassPanel

- (instancetype)init {
  self = [super init];
  if (self) {
    _blurRadius = [OSFStyle blurRadiusMedium];
    _tintAlpha = 0.85;
    _tintColor = [OSFColors surface];
  }
  return self;
}

+ (instancetype)glassPanelWithFrame:(CGRect)frame {
  OSFGlassPanel *panel = [[OSFGlassPanel alloc] init];
  panel.frame = frame;
  return panel;
}

- (void)draw:(CGContextRef)context {
  // TODO: Apply blur effect using Vulkan
  // For now, just draw semi-transparent background

  NSColor *tintWithAlpha = [_tintColor colorWithAlphaComponent:_tintAlpha];
  CGContextSetFillColorWithColor(context, [tintWithAlpha CGColor]);
  CGContextFillRect(context, self.frame);

  [super draw:context];
}

@end
