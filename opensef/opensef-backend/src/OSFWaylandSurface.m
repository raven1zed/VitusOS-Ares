/**
 * OSFWaylandSurface.m - Wayland surface wrapper
 * openSEF Framework
 */

#import <opensef/OpenSEFBackend.h>

@implementation OSFWaylandSurface

+ (instancetype)surfaceWithSize:(CGSize)size title:(NSString *)title {
  OSFWaylandSurface *surface = [[self alloc] init];
  surface->_size = size;
  surface->_title = [title copy];
  return surface;
}

- (void)commit {
  // TODO: wl_surface_commit(self.wl_surface)
  NSLog(@"[openSEF] Surface commit: %@", self.title);
}

- (void)destroy {
  // TODO: wl_surface_destroy(self.wl_surface)
  NSLog(@"[openSEF] Surface destroyed: %@", self.title);
}

- (void)setOpaque:(BOOL)opaque {
  // TODO: Configure surface transparency for glass effects
}

@end
