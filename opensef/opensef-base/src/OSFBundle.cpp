/**
 * OSFBundle.cpp - Resource bundle metadata
 */

#include <opensef/OpenSEFBase.h>


namespace opensef {

OSFBundle::OSFBundle(std::string identifier, std::string resourcePath)
    : identifier_(std::move(identifier)),
      resourcePath_(std::move(resourcePath)) {}

OSFBundle OSFBundle::mainBundle() {
  return OSFBundle("opensef.app", ".");
}

} // namespace opensef
