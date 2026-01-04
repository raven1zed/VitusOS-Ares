/**
 * VulkanTextRenderer.h - Vulkan Text Rendering Engine
 *
 * Implements high-performance text rendering using FreeType and Vulkan.
 * Manages texture atlases and vertex generation for text.
 */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <glm/glm.hpp>
#include <vulkan/vulkan.h>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace opensef {

struct Character {
    uint32_t textureID;  // Handle to the texture atlas (if multiple)
    glm::ivec2 size;     // Size of glyph
    glm::ivec2 bearing;  // Offset from baseline to left/top of glyph
    uint32_t advance;    // Offset to advance to next glyph
    glm::vec2 uvMin;     // UV coordinates in atlas
    glm::vec2 uvMax;
};

struct TextVertex {
    glm::vec2 pos;
    glm::vec2 texCoord;
    glm::vec4 color;
};

class VulkanTextRenderer {
public:
    static VulkanTextRenderer& shared();

    // Initialization
    void initialize(VkDevice device, VkPhysicalDevice physicalDevice, VkQueue queue, VkCommandPool commandPool, VkRenderPass renderPass);
    void shutdown();

    // Font Management
    bool loadFont(const std::string& fontPath, uint32_t fontSize);

    // Rendering
    // Records draw commands into the provided command buffer
    void drawText(VkCommandBuffer commandBuffer, const std::string& text, float x, float y, float scale, const glm::vec4& color);

    // Helpers
    float measureTextWidth(const std::string& text, float scale);
    void setScreenSize(int width, int height) { screenWidth_ = width; screenHeight_ = height; }

private:
    VulkanTextRenderer() = default;
    ~VulkanTextRenderer();

    // Vulkan Resources
    VkDevice device_ = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice_ = VK_NULL_HANDLE;
    VkQueue queue_ = VK_NULL_HANDLE;
    VkCommandPool commandPool_ = VK_NULL_HANDLE;

    // Pipeline
    VkPipelineLayout pipelineLayout_ = VK_NULL_HANDLE;
    VkPipeline pipeline_ = VK_NULL_HANDLE;
    VkDescriptorSetLayout descriptorSetLayout_ = VK_NULL_HANDLE;
    VkDescriptorPool descriptorPool_ = VK_NULL_HANDLE;
    VkDescriptorSet descriptorSet_ = VK_NULL_HANDLE;

    // Font Atlas
    VkImage atlasImage_ = VK_NULL_HANDLE;
    VkDeviceMemory atlasMemory_ = VK_NULL_HANDLE;
    VkImageView atlasView_ = VK_NULL_HANDLE;
    VkSampler atlasSampler_ = VK_NULL_HANDLE;

    // Vertex Buffer
    VkBuffer vertexBuffer_ = VK_NULL_HANDLE;
    VkDeviceMemory vertexBufferMemory_ = VK_NULL_HANDLE;
    size_t vertexBufferSize_ = 0;

    // FreeType
    FT_Library ftLibrary_ = nullptr;
    FT_Face ftFace_ = nullptr;
    std::map<char, Character> characters_;

    // Atlas State
    const int ATLAS_WIDTH = 1024;
    const int ATLAS_HEIGHT = 1024;
    int atlasOffsetX_ = 0;
    int atlasOffsetY_ = 0;
    int atlasRowHeight_ = 0;

    // Screen Dimensions for Projection
    int screenWidth_ = 1920;
    int screenHeight_ = 1080;

    // Internal methods
    void createPipeline(VkRenderPass renderPass);
    void createAtlasTexture();
    void updateAtlas(const std::vector<uint8_t>& buffer, int x, int y, int w, int h);
    void createVertexBuffer();
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
};

} // namespace opensef
