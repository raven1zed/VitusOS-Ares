/**
 * openSEF AppKit: GUI Widget Layer
 *
 * Windows, views, and UI components for openSEF framework.
 * Forked from GNUStep-gui, modernized for VitusOS.
 */

#ifndef OPENSEF_APPKIT_H
#define OPENSEF_APPKIT_H

#import <opensef/OpenSEFBase.h>

// Forward declarations
@class OSFWindow;
@class OSFView;
@class OSFBackend;

// ============================================================================
// OSFView - Base class for all visual elements
// ============================================================================

@interface OSFView : OSFObject

@property(nonatomic) CGRect frame;
@property(nonatomic, weak) OSFView *superview;
@property(nonatomic, strong) NSMutableArray<OSFView *> *subviews;
@property(nonatomic) CGFloat alpha;
@property(nonatomic) BOOL hidden;

- (void)addSubview:(OSFView *)view;
- (void)removeFromSuperview;
- (void)draw:(CGContextRef)context;

@end

// ============================================================================
// OSFWindow - Top-level window
// ============================================================================

@interface OSFWindow : OSFView

@property(nonatomic, copy) NSString *title;
@property(nonatomic) BOOL visible;
@property(nonatomic, readonly) OSFBackend *backend;

+ (instancetype)windowWithTitle:(NSString *)title frame:(CGRect)frame;

- (void)show;
- (void)close;
- (void)setContentView:(OSFView *)view;

@end

// ============================================================================
// OSFButton - Clickable button
// ============================================================================

@interface OSFButton : OSFView

@property(nonatomic, copy) NSString *label;
@property(nonatomic, copy) void (^action)(void);

+ (instancetype)buttonWithLabel:(NSString *)label action:(void (^)(void))action;

@end

// ============================================================================
// OSFGlassPanel - OS1-style frosted glass panel
// ============================================================================

@interface OSFGlassPanel : OSFView

@property(nonatomic) CGFloat blurRadius;
@property(nonatomic) CGFloat tintAlpha;
@property(nonatomic, strong) NSColor *tintColor;

+ (instancetype)glassPanelWithFrame:(CGRect)frame;

@end

// ============================================================================
// OSFTextField - Text input field
// ============================================================================

@interface OSFTextField : OSFView

@property(nonatomic, copy) NSString *text;
@property(nonatomic, copy) NSString *placeholder;
@property(nonatomic, copy) void (^onTextChanged)(NSString *text);

@end

// ============================================================================
// OSFLabel - Text label
// ============================================================================

@interface OSFLabel : OSFView

@property(nonatomic, copy) NSString *text;
@property(nonatomic, strong) NSFont *font;
@property(nonatomic, strong) NSColor *textColor;

@end

// ============================================================================
// OSFTableView - List/table view for SeaDrop history
// ============================================================================

@interface OSFTableView : OSFView

@property(nonatomic, strong) NSArray *items;
@property(nonatomic) NSInteger selectedIndex;
@property(nonatomic, copy) void (^onSelection)(NSInteger index);

- (void)reloadData;

@end

// ============================================================================
// OSFProgressBar - Progress indicator (for installer, loading)
// ============================================================================

@interface OSFProgressBar : OSFView

@property(nonatomic) CGFloat progress;   // 0.0 to 1.0
@property(nonatomic) BOOL indeterminate; // Spinning/pulsing mode
@property(nonatomic, strong) NSColor *trackColor;
@property(nonatomic, strong) NSColor *progressColor; // Space Orange default

- (void)setProgress:(CGFloat)progress animated:(BOOL)animated;

@end

// ============================================================================
// OSFSwitch - Toggle switch (like iOS)
// ============================================================================

@interface OSFSwitch : OSFView

@property(nonatomic) BOOL on;
@property(nonatomic, copy) void (^onToggle)(BOOL on);
@property(nonatomic, strong) NSColor *onColor; // Space Orange

- (void)setOn:(BOOL)on animated:(BOOL)animated;

@end

// ============================================================================
// OSFSlider - Value slider
// ============================================================================

@interface OSFSlider : OSFView

@property(nonatomic) CGFloat value; // 0.0 to 1.0
@property(nonatomic) CGFloat minValue;
@property(nonatomic) CGFloat maxValue;
@property(nonatomic, copy) void (^onValueChanged)(CGFloat value);
@property(nonatomic, strong) NSColor *trackColor;
@property(nonatomic, strong) NSColor *thumbColor;

@end

// ============================================================================
// OSFCheckbox - Checkbox with label
// ============================================================================

@interface OSFCheckbox : OSFView

@property(nonatomic) BOOL checked;
@property(nonatomic, copy) NSString *label;
@property(nonatomic, copy) void (^onToggle)(BOOL checked);

@end

// ============================================================================
// OSFDropdown - Dropdown/select menu
// ============================================================================

@interface OSFDropdown : OSFView

@property(nonatomic, strong) NSArray<NSString *> *options;
@property(nonatomic) NSInteger selectedIndex;
@property(nonatomic, copy) void (^onSelection)(NSInteger index);
@property(nonatomic, copy) NSString *placeholder;

@end

// ============================================================================
// OSFImageView - Image display
// ============================================================================

@interface OSFImageView : OSFView

@property(nonatomic, strong) NSImage *image;
@property(nonatomic) CGFloat cornerRadius;
@property(nonatomic) BOOL aspectFit; // vs aspectFill

@end

// ============================================================================
// OSFScrollView - Scrollable container
// ============================================================================

@interface OSFScrollView : OSFView

@property(nonatomic, strong) OSFView *contentView;
@property(nonatomic) CGSize contentSize;
@property(nonatomic) CGPoint contentOffset;
@property(nonatomic) BOOL showsVerticalScrollIndicator;
@property(nonatomic) BOOL showsHorizontalScrollIndicator;

@end

// ============================================================================
// OSFMenu - Menu container
// ============================================================================

@interface OSFMenu : OSFObject

@property(nonatomic, copy) NSString *title;
@property(nonatomic, strong) NSMutableArray *items;

- (instancetype)initWithTitle:(NSString *)title;
- (void)addItem:(id)item;
- (void)addSeparator;

@end

// ============================================================================
// OSFMenuItem - Individual menu item
// ============================================================================

@interface OSFMenuItem : OSFObject

@property(nonatomic, copy) NSString *title;
@property(nonatomic, copy) NSString *shortcut;
@property(nonatomic, copy) void (^triggered)(void);
@property(nonatomic) BOOL enabled;
@property(nonatomic) BOOL checked;

- (instancetype)initWithTitle:(NSString *)title;

@end

// ============================================================================
// OSFMenuBar - Global menu bar (VitusOS uses global menus)
// ============================================================================

@interface OSFMenuBar : OSFObject

+ (instancetype)shared;

- (void)addMenu:(OSFMenu *)menu;
- (void)removeMenu:(OSFMenu *)menu;
- (void)setApplicationMenu:(OSFMenu *)menu;

@end

#endif /* OPENSEF_APPKIT_H */
