/**
 * OSFObject.m - Root class implementation
 * openSEF Framework
 */

#import <opensef/OpenSEFBase.h>

@implementation OSFObject

+ (instancetype)create {
  return [[self alloc] init];
}

- (instancetype)init {
  self = [super init];
  if (self) {
    _objectID = [[NSUUID UUID] UUIDString];
  }
  return self;
}

- (NSString *)description {
  return
      [NSString stringWithFormat:@"<%@: %@>", NSStringFromClass([self class]),
                                 self.objectID];
}

@end
