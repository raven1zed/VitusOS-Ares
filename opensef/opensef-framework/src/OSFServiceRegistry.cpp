#include <opensef/OSFServiceRegistry.h>
#include <map>
#include <mutex>

namespace OpenSEF {

struct OSFServiceRegistry::Impl {
  std::map<std::string, OSFService *> services;
  std::mutex mutex;
};

OSFServiceRegistry::OSFServiceRegistry() : impl_(std::make_unique<Impl>()) {}

OSFServiceRegistry::~OSFServiceRegistry() = default;

void OSFServiceRegistry::registerService(const std::string &name,
                                         OSFService *service) {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  impl_->services[name] = service;
}

void OSFServiceRegistry::unregisterService(const std::string &name) {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  impl_->services.erase(name);
}

OSFService *OSFServiceRegistry::getService(const std::string &name) {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  auto it = impl_->services.find(name);
  return (it != impl_->services.end()) ? it->second : nullptr;
}

bool OSFServiceRegistry::hasService(const std::string &name) {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  return impl_->services.find(name) != impl_->services.end();
}

std::vector<std::string> OSFServiceRegistry::allServices() {
  std::lock_guard<std::mutex> lock(impl_->mutex);
  std::vector<std::string> result;
  for (auto &pair : impl_->services) {
    result.push_back(pair.first);
  }
  return result;
}

} // namespace OpenSEF
