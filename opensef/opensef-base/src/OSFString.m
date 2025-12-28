/**
 * OSFString.m - Enhanced string implementation
 * openSEF Framework
 */

#import <opensef/OpenSEFBase.h>

@implementation OSFString

+ (instancetype)stringWithStdString:(const char *)str {
  return [self stringWithUTF8String:str];
}

@end
