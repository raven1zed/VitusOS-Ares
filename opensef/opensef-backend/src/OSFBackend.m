/**
 * OSFBackend.m - Main display backend coordinator
 * openSEF Framework
 */

#import <opensef/OpenSEFBackend.h>

@implementation OSFBackend

static OSFBackend *_sharedBackend = nil;

+ (instancetype)sharedBackend {
  static dispatch_once_t onceToken;
  dispatch_once(&onceToken, ^{
    _sharedBackend = [[self alloc] init];
  });
  return _sharedBackend;
}

- (BOOL)connect {
  // TODO: Connect to Wayland compositor
  // wl_display_connect(NULL)
  NSLog(@"[openSEF] Connecting to Wayland compositor...");
  return YES;
}

- (void)run {
  NSLog(@"[openSEF] Starting event loop...");
  // TODO: Wayland event loop
  // while (wl_display_dispatch(display) != -1) { }
}

- (void)disconnect {
  NSLog(@"[openSEF] Disconnecting from Wayland...");
  // TODO: wl_display_disconnect(display)
}

- (BOOL)isWayland {
  return YES; // VitusOS is Wayland-only
}

@end
