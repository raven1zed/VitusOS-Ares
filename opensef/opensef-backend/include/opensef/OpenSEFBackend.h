/**
 * openSEF Backend: Display Backend
 *
 * Wayland surface management and Vulkan rendering.
 * This is where pixels actually get to the screen.
 */

#ifndef OPENSEF_BACKEND_H
#define OPENSEF_BACKEND_H

#import <opensef/OpenSEFBase.h>

// ============================================================================
// OSFBackend - Main display backend coordinator
// ============================================================================

@interface OSFBackend : OSFObject

/** Shared backend instance */
+ (instancetype)sharedBackend;

/** Initialize Wayland connection */
- (BOOL)connect;

/** Main event loop */
- (void)run;

/** Disconnect and cleanup */
- (void)disconnect;

/** Check if running on Wayland */
- (BOOL)isWayland;

@end

// ============================================================================
// OSFWaylandSurface - Wayland surface wrapper
// ============================================================================

@interface OSFWaylandSurface : OSFObject

@property(nonatomic) CGSize size;
@property(nonatomic, copy) NSString *title;
@property(nonatomic, readonly) void *wl_surface;

+ (instancetype)surfaceWithSize:(CGSize)size title:(NSString *)title;

- (void)commit;
- (void)destroy;
- (void)setOpaque:(BOOL)opaque;

@end

// ============================================================================
// OSFVulkanRenderer - Vulkan rendering backend
// ============================================================================

@interface OSFVulkanRenderer : OSFObject

@property(nonatomic, readonly) BOOL available;
@property(nonatomic, readonly) BOOL supportsBlur;

+ (instancetype)sharedRenderer;

- (BOOL)initialize;
- (void)beginFrame;
- (void)endFrame;

/** Draw blur effect (OS1 glass panels) */
- (void)drawBlurWithRadius:(CGFloat)radius inRect:(CGRect)rect;

/** Draw solid color */
- (void)drawColor:(NSColor *)color inRect:(CGRect)rect;

/** Draw rounded rectangle */
- (void)drawRoundedRect:(CGRect)rect
                 radius:(CGFloat)radius
                  color:(NSColor *)color;

/** Draw shadow */
- (void)drawShadow:(CGRect)rect
              blur:(CGFloat)blur
             color:(NSColor *)color
            offset:(CGPoint)offset;

@end

// ============================================================================
// OSFInputHandler - Keyboard/mouse/touch input
// ============================================================================

@interface OSFInputHandler : OSFObject

+ (instancetype)sharedHandler;

@property(nonatomic, copy) void (^onKeyDown)
    (NSString *key, NSUInteger modifiers);
@property(nonatomic, copy) void (^onKeyUp)(NSString *key, NSUInteger modifiers);
@property(nonatomic, copy) void (^onMouseMove)(CGPoint location);
@property(nonatomic, copy) void (^onMouseDown)
    (CGPoint location, NSInteger button);
@property(nonatomic, copy) void (^onMouseUp)(CGPoint location, NSInteger button)
    ;

@end

#endif /* OPENSEF_BACKEND_H */
