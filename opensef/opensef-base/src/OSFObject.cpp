/**
 * OSFObject.cpp - Base object implementation
 */

#include <iomanip>
#include <opensef/OpenSEFBase.h>
#include <sstream>


namespace opensef {

std::atomic<uint64_t> OSFObject::nextID_{1};

OSFObject::OSFObject() {
  // Generate unique ID
  uint64_t id = nextID_++;
  std::ostringstream oss;
  oss << "osf_" << std::hex << std::setfill('0') << std::setw(8) << id;
  objectID_ = oss.str();
}

OSFObject::~OSFObject() = default;

std::shared_ptr<OSFObject> OSFObject::create() {
  return std::make_shared<OSFObject>();
}

} // namespace opensef
