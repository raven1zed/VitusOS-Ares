/**
 * OSFArray.m - Enhanced array implementation
 * openSEF Framework
 */

#import <opensef/OpenSEFBase.h>

@implementation OSFArray

- (void)observe:(void (^)(id obj, NSUInteger idx))block {
  [self enumerateObjectsUsingBlock:^(id obj, NSUInteger idx, BOOL *stop) {
    block(obj, idx);
  }];
}

@end
