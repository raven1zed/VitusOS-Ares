/**
 * openSEF Base: Foundation Layer
 *
 * Open SeagrEnv (Seamless Integration Environment) Framework
 * Core object system forked from GNUStep-base, modernized for VitusOS.
 */

#ifndef OPENSEF_BASE_H
#define OPENSEF_BASE_H

#import <Foundation/Foundation.h>

// ============================================================================
// OSFObject - Root class for all openSEF objects
// ============================================================================

@interface OSFObject : NSObject

/** Unique identifier for this object */
@property(nonatomic, readonly) NSString *objectID;

/** Create a new openSEF object */
+ (instancetype)create;

@end

// ============================================================================
// OSFString - Enhanced string with openSEF features
// ============================================================================

@interface OSFString : NSString

/** Create from C++ std::string (for C++ interop) */
+ (instancetype)stringWithStdString:(const char *)str;

@end

// ============================================================================
// OSFArray - Enhanced array with openSEF features
// ============================================================================

@interface OSFArray<ObjectType> : NSArray <ObjectType>

/** Reactive observation support (future) */
- (void)observe:(void (^)(ObjectType obj, NSUInteger idx))block;

@end

#endif /* OPENSEF_BASE_H */
