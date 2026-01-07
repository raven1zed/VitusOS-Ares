#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace OpenSEF {

/**
 * OSFService - Base interface for services
 */
class OSFService {
public:
  virtual ~OSFService() = default;

  virtual std::string name() const = 0;
  virtual void start() = 0;
  virtual void stop() = 0;
};

/**
 * OSFServiceRegistry - Service Discovery and Management
 *
 * Components register themselves as services.
 * Other components discover and use them.
 *
 * Usage:
 *   // Register
 *   desktop->serviceRegistry()->registerService("panel", panelService);
 *
 *   // Discover
 *   auto* panel = desktop->serviceRegistry()->getService("panel");
 */
class OSFServiceRegistry {
public:
  OSFServiceRegistry();
  ~OSFServiceRegistry();

  // Service registration
  void registerService(const std::string &name, OSFService *service);
  void unregisterService(const std::string &name);

  // Service discovery
  OSFService *getService(const std::string &name);
  bool hasService(const std::string &name);

  // Service queries
  std::vector<std::string> allServices();

private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

} // namespace OpenSEF
