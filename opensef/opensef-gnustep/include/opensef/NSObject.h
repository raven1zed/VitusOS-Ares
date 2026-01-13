#pragma once

#include <memory>
#include <string>


/**
 * NSObject - Base class for all openSEF objects
 *
 * Minimal Cocoa NSObject equivalent in C++
 */
class NSObject {
public:
  NSObject() = default;
  virtual ~NSObject() = default;

  // Object lifecycle
  virtual void retain() { m_refCount++; }
  virtual void release() {
    m_refCount--;
    if (m_refCount <= 0)
      delete this;
  }
  int retainCount() const { return m_refCount; }

  // Description (similar to Objective-C description)
  virtual std::string description() const { return "NSObject"; }

protected:
  int m_refCount = 1;
};
