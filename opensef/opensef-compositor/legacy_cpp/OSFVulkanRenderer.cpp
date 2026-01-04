/**
 * OSFVulkanRenderer.cpp - Vulkan rendering
 */

#include "OSFVulkanRenderer.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <opensef/OpenSEFBackend.h>

namespace opensef {

OSFVulkanRenderer &OSFVulkanRenderer::shared() {
  static OSFVulkanRenderer instance;
  return instance;
}

bool OSFVulkanRenderer::initialize() {
  if (available_) {
      return true; // Already initialized
  }

  std::cout << "[openSEF] Initializing Vulkan renderer..." << std::endl;

  // 1. Create VkInstance
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "OpenSEF";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;

  // Enable extensions for surface creation
  // We use string literals to avoid dependency on specific headers being present
  std::vector<const char*> extensions;
  extensions.push_back("VK_KHR_surface");
  extensions.push_back("VK_KHR_wayland_surface");

  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();

  // Validation layers (simplified)
  createInfo.enabledLayerCount = 0;
  createInfo.ppEnabledLayerNames = nullptr;

  if (vkCreateInstance(&createInfo, nullptr, &instance_) != VK_SUCCESS) {
      std::cerr << "[openSEF] Failed to create Vulkan instance! Trying without extensions..." << std::endl;
      // Fallback for headless/test environments
      createInfo.enabledExtensionCount = 0;
      if (vkCreateInstance(&createInfo, nullptr, &instance_) != VK_SUCCESS) {
          std::cerr << "[openSEF] Failed to create Vulkan instance (fallback)!" << std::endl;
          return false;
      }
  }
  std::cout << "[openSEF] Vulkan instance created." << std::endl;

  // 2. Select physical device
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(instance_, &deviceCount, nullptr);
  if (deviceCount == 0) {
      std::cerr << "[openSEF] Failed to find GPUs with Vulkan support!" << std::endl;
      shutdown();
      return false;
  }

  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(instance_, &deviceCount, devices.data());

  // Pick the first discrete GPU, or just the first one
  for (const auto& device : devices) {
      VkPhysicalDeviceProperties deviceProperties;
      vkGetPhysicalDeviceProperties(device, &deviceProperties);
      if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
          physicalDevice_ = device;
          std::cout << "[openSEF] Selected Discrete GPU: " << deviceProperties.deviceName << std::endl;
          break;
      }
  }

  if (physicalDevice_ == VK_NULL_HANDLE) {
      physicalDevice_ = devices[0];
      VkPhysicalDeviceProperties deviceProperties;
      vkGetPhysicalDeviceProperties(physicalDevice_, &deviceProperties);
      std::cout << "[openSEF] Selected GPU: " << deviceProperties.deviceName << std::endl;
  }

  // 3. Create logical device
  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice_, &queueFamilyCount, nullptr);
  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice_, &queueFamilyCount, queueFamilies.data());

  bool foundGraphicsQueue = false;
  for (uint32_t i = 0; i < queueFamilyCount; i++) {
      if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
          graphicsQueueFamilyIndex_ = i;
          foundGraphicsQueue = true;
          break;
      }
  }

  if (!foundGraphicsQueue) {
       std::cerr << "[openSEF] Failed to find a graphics queue family!" << std::endl;
       shutdown();
       return false;
  }

  float queuePriority = 1.0f;
  VkDeviceQueueCreateInfo queueCreateInfo{};
  queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  queueCreateInfo.queueFamilyIndex = graphicsQueueFamilyIndex_;
  queueCreateInfo.queueCount = 1;
  queueCreateInfo.pQueuePriorities = &queuePriority;

  VkPhysicalDeviceFeatures deviceFeatures{}; // Enable features if needed

  VkDeviceCreateInfo deviceCreateInfo{};
  deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
  deviceCreateInfo.queueCreateInfoCount = 1;
  deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
  // Device extensions (swapchain)
  const char* deviceExtensions[] = { "VK_KHR_swapchain" };
  // Only enable swapchain if we successfully enabled instance extensions
  if (createInfo.enabledExtensionCount > 0) {
      deviceCreateInfo.enabledExtensionCount = 1;
      deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions;
  } else {
      deviceCreateInfo.enabledExtensionCount = 0;
  }

  if (vkCreateDevice(physicalDevice_, &deviceCreateInfo, nullptr, &device_) != VK_SUCCESS) {
      std::cerr << "[openSEF] Failed to create logical device!" << std::endl;
      shutdown();
      return false;
  }

  vkGetDeviceQueue(device_, graphicsQueueFamilyIndex_, 0, &graphicsQueue_);
  std::cout << "[openSEF] Logical device created." << std::endl;

  // 4. Set up swapchain (TODO: requires window surface)

  available_ = true;
  return true;
}

void OSFVulkanRenderer::shutdown() {
  if (device_ != VK_NULL_HANDLE) {
      vkDestroyDevice(device_, nullptr);
      device_ = VK_NULL_HANDLE;
  }
  if (instance_ != VK_NULL_HANDLE) {
      vkDestroyInstance(instance_, nullptr);
      instance_ = VK_NULL_HANDLE;
  }

  if (available_) {
    available_ = false;
    std::cout << "[openSEF] Vulkan shutdown" << std::endl;
  }
}

void OSFVulkanRenderer::beginFrame() {
  // TODO: Begin Vulkan frame
}

void OSFVulkanRenderer::endFrame() {
  // TODO: End frame, present
}

void OSFVulkanRenderer::drawRect(const OSFRect &rect, const OSFColor &color) {
  // TODO: Draw filled rectangle
}

void OSFVulkanRenderer::drawRoundedRect(const OSFRect &rect, float radius,
                                        const OSFColor &color) {
  // TODO: Draw rounded rectangle
}

void OSFVulkanRenderer::drawBlur(const OSFRect &rect, float radius) {
  // TODO: Apply Gaussian blur
}

// ============================================================================
// OSFWaylandSurface
// ============================================================================

OSFWaylandSurface::OSFWaylandSurface() = default;

OSFWaylandSurface::~OSFWaylandSurface() { destroy(); }

std::shared_ptr<OSFWaylandSurface>
OSFWaylandSurface::create(const OSFSize &size, const std::string &title) {
  auto surface = std::make_shared<OSFWaylandSurface>();
  surface->setSize(size);
  surface->setTitle(title);
  // TODO: Create actual Wayland surface
  return surface;
}

void OSFWaylandSurface::commit() {
  // TODO: wl_surface_commit
}

void OSFWaylandSurface::destroy() {
  // TODO: Cleanup Wayland surfaces
}

// ============================================================================
// OSFInputHandler
// ============================================================================

OSFInputHandler &OSFInputHandler::shared() {
  static OSFInputHandler instance;
  return instance;
}

} // namespace opensef
