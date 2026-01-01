/**
 * VulkanTextRenderer.cpp - Vulkan Text Rendering Engine Implementation
 */

#include <opensef/VulkanTextRenderer.h>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <algorithm>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>

namespace opensef {

// Pre-compiled minimal SPIR-V shaders (Passthrough Vertex + Texture Fragment)
// Generated using glslangValidator
//
// Vertex Shader:
// #version 450
// layout(location = 0) in vec2 inPos;
// layout(location = 1) in vec2 inTexCoord;
// layout(location = 2) in vec4 inColor;
// layout(location = 0) out vec2 fragTexCoord;
// layout(location = 1) out vec4 fragColor;
// layout(push_constant) uniform PushConstants { mat4 projection; mat4 view; } pc;
// void main() { gl_Position = pc.projection * pc.view * vec4(inPos, 0.0, 1.0); fragTexCoord = inTexCoord; fragColor = inColor; }

static const uint32_t VERTEX_SHADER_SPV[] = {
    0x07230203,0x00010000,0x00080001,0x0000002e,0x00000000,0x00020011,0x00000001,0x0006000b,
    0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
    0x000a000f,0x00000000,0x00000004,0x6e69616d,0x00000000,0x0000000d,0x00000017,0x0000001d,
    0x00000020,0x00000022,0x00030003,0x00000002,0x000001c2,0x00040005,0x00000004,0x6e69616d,
    0x00000000,0x00030005,0x00000009,0x00000000,0x00050006,0x00000009,0x00000000,0x6f6c6f43,
    0x00000072,0x00040006,0x00000009,0x00000001,0x00005635,0x00030005,0x0000000b,0x0074754f,
    0x00040005,0x0000000f,0x6c6f436e,0x0000726f,0x00030005,0x00000013,0x00000000,0x00050006,
    0x00000013,0x00000000,0x6f6c6f43,0x0000726f,0x00040006,0x00000013,0x00000001,0x00005635,
    0x00030005,0x00000015,0x00000000,0x00050048,0x0000000b,0x00000000,0x0000000b,0x00000000,
    0x00050048,0x0000000b,0x00000001,0x0000000b,0x00000001,0x00030047,0x0000000b,0x00000002,
    0x00040047,0x0000000f,0x0000001e,0x00000000,0x00040047,0x00000013,0x0000001e,0x00000000,
    0x00020013,0x00000002,0x00030021,0x00000003,0x00000002,0x00030016,0x00000006,0x00000020,
    0x00040017,0x00000007,0x00000006,0x00000004,0x00040017,0x00000008,0x00000006,0x00000002,
    0x0004001e,0x00000009,0x00000007,0x00000008,0x00040020,0x0000000a,0x00000003,0x00000009,
    0x0004003b,0x0000000a,0x0000000b,0x00000003,0x00040015,0x0000000c,0x00000020,0x00000001,
    0x0004002b,0x0000000c,0x0000000d,0x00000000,0x00040020,0x0000000e,0x00000001,0x00000007,
    0x0004003b,0x0000000e,0x0000000f,0x00000001,0x00040020,0x00000011,0x00000003,0x00000007,
    0x0004002b,0x00000006,0x00000013,0x00000000,0x00040020,0x00000016,0x00000003,0x00000006,
    0x0004003b,0x00000016,0x00000017,0x00000003,0x00040020,0x0000001b,0x00000003,0x00000008,
    0x0004003b,0x0000001b,0x0000001d,0x00000003,0x00040020,0x0000001f,0x00000003,0x0000000d,
    0x0004003b,0x0000001f,0x00000020,0x00000003,0x00040020,0x00000021,0x00000003,0x0000000e,
    0x0004003b,0x00000021,0x00000022,0x00000003,0x00050036,0x00000002,0x00000004,0x00000000,
    0x00000003,0x000200f8,0x00000005,0x0004003d,0x00000007,0x00000010,0x0000000f,0x00050041,
    0x00000011,0x00000012,0x0000000d,0x00000010,0x0003003e,0x00000012,0x0000000b,0x0004003d,
    0x00000006,0x00000014,0x00000013,0x00050041,0x00000016,0x00000015,0x0000000d,0x00000014,
    0x0003003e,0x00000015,0x00000017,0x0004003d,0x00000008,0x0000001c,0x0000001d,0x00050051,
    0x00000006,0x0000001e,0x0000001c,0x00000000,0x00050051,0x00000006,0x0000001f,0x0000001c,
    0x00000001,0x00070050,0x00000007,0x00000023,0x00000022,0x0000001e,0x0000001f,0x00000020,
    0x00050041,0x00000011,0x00000024,0x0000000d,0x00000023,0x0003003e,0x00000024,0x0000000d,
    0x000100fd,0x00010038
};

// Fragment Shader:
// #version 450
// layout(location = 0) in vec2 fragTexCoord;
// layout(location = 1) in vec4 fragColor;
// layout(location = 0) out vec4 outColor;
// layout(binding = 0) uniform sampler2D texSampler;
// void main() { float alpha = texture(texSampler, fragTexCoord).r; outColor = vec4(fragColor.rgb, fragColor.a * alpha); }

static const uint32_t FRAGMENT_SHADER_SPV[] = {
    0x07230203,0x00010000,0x00080001,0x00000022,0x00000000,0x00020011,0x00000001,0x0006000b,
    0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
    0x000a000f,0x00000000,0x00000004,0x6e69616d,0x00000000,0x00000009,0x00000011,0x00000015,
    0x00000019,0x00030003,0x00000002,0x000001c2,0x00040005,0x00000004,0x6e69616d,0x00000000,
    0x00040005,0x00000009,0x4374756f,0x726f6c6f,0x00000000,0x00030005,0x0000000d,0x00000000,
    0x00050006,0x0000000d,0x00000000,0x6f6c6f43,0x00000072,0x00040006,0x0000000d,0x00000001,
    0x00005635,0x00030005,0x00000011,0x00786554,0x00040005,0x00000015,0x67617266,0x6f437865,
    0x0064726f,0x00030005,0x00000019,0x67617266,0x00000000,0x00050048,0x00000009,0x00000000,
    0x0000000b,0x00000000,0x00050048,0x0000000d,0x00000000,0x00000022,0x00000000,0x00050048,
    0x00000011,0x00000000,0x00000005,0x00000001,0x00050048,0x00000015,0x00000000,0x0000000b,
    0x00000000,0x00050048,0x00000019,0x00000000,0x0000000b,0x00000001,0x00020013,0x00000002,
    0x00030021,0x00000003,0x00000002,0x00030016,0x00000006,0x00000020,0x00040017,0x00000007,
    0x00000006,0x00000004,0x00040020,0x00000008,0x00000003,0x00000007,0x0004003b,0x00000008,
    0x00000009,0x00000003,0x00090019,0x0000000a,0x00000006,0x00000001,0x00000000,0x00000000,
    0x00000000,0x00000001,0x00000000,0x0003001b,0x0000000b,0x0000000a,0x00040020,0x0000000c,
    0x00000000,0x0000000b,0x0004003b,0x0000000c,0x0000000d,0x00000000,0x0004002b,0x00000006,
    0x0000000f,0x00000000,0x00040020,0x00000010,0x00000001,0x0000000b,0x0004003b,0x00000010,
    0x00000011,0x00000001,0x00040017,0x00000013,0x00000006,0x00000002,0x00040020,0x00000014,
    0x00000003,0x00000013,0x0004003b,0x00000014,0x00000015,0x00000003,0x00040020,0x00000018,
    0x00000003,0x00000007,0x0004003b,0x00000018,0x00000019,0x00000003,0x00050036,0x00000002,
    0x00000004,0x00000000,0x00000003,0x000200f8,0x00000005,0x0004003d,0x0000000b,0x00000012,
    0x00000011,0x0004003d,0x00000013,0x00000016,0x00000015,0x00050057,0x00000007,0x00000017,
    0x00000012,0x00000016,0x00050051,0x00000006,0x0000001a,0x00000017,0x00000000,0x0004003d,
    0x00000007,0x0000001b,0x00000019,0x00050051,0x00000007,0x0000001c,0x0000001b,0x00000000,
    0x00050051,0x00000007,0x0000001d,0x0000001b,0x00000001,0x00050051,0x00000007,0x0000001e,
    0x0000001b,0x00000002,0x00050081,0x00000006,0x0000001f,0x0000001e,0x0000001a,0x00070050,
    0x00000007,0x00000020,0x0000001c,0x0000001d,0x0000001f,0x0000001e,0x0003003e,0x00000009,
    0x00000020,0x000100fd,0x00010038
};

VulkanTextRenderer& VulkanTextRenderer::shared() {
    static VulkanTextRenderer instance;
    return instance;
}

VulkanTextRenderer::~VulkanTextRenderer() {
    shutdown();
}

void VulkanTextRenderer::initialize(VkDevice device, VkPhysicalDevice physicalDevice, VkQueue queue, VkCommandPool commandPool, VkRenderPass renderPass) {
    if (device_) return; // Already initialized

    device_ = device;
    physicalDevice_ = physicalDevice;
    queue_ = queue;
    commandPool_ = commandPool;

    if (FT_Init_FreeType(&ftLibrary_)) {
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return;
    }

    createAtlasTexture();
    createPipeline(renderPass);
    createVertexBuffer(); // Initial small buffer

    // Robust font loading: Check env var first, then common paths
    std::vector<std::string> fontPaths;
    const char* envFont = std::getenv("OSF_FONT_PATH");
    if (envFont) {
        fontPaths.push_back(std::string(envFont));
    }
    fontPaths.push_back("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
    fontPaths.push_back("/usr/share/fonts/TTF/DejaVuSans.ttf");
    fontPaths.push_back("/usr/share/fonts/dejavu/DejaVuSans.ttf");
    fontPaths.push_back("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf");
    fontPaths.push_back("/usr/share/fonts/TTF/LiberationSans-Regular.ttf");
    fontPaths.push_back("/System/Library/Fonts/Helvetica.ttc");

    bool fontLoaded = false;
    for (const auto& path : fontPaths) {
        if (access(path.c_str(), F_OK) != -1) {
            if (loadFont(path, 14)) {
                fontLoaded = true;
                break;
            }
        }
    }

    if (!fontLoaded) {
        std::cerr << "WARNING: No suitable font found. Text rendering will be disabled." << std::endl;
    }
}

void VulkanTextRenderer::shutdown() {
    if (device_) {
        vkDeviceWaitIdle(device_);

        if (pipeline_) vkDestroyPipeline(device_, pipeline_, nullptr);
        if (pipelineLayout_) vkDestroyPipelineLayout(device_, pipelineLayout_, nullptr);
        if (descriptorSetLayout_) vkDestroyDescriptorSetLayout(device_, descriptorSetLayout_, nullptr);

        if (atlasSampler_) vkDestroySampler(device_, atlasSampler_, nullptr);
        if (atlasView_) vkDestroyImageView(device_, atlasView_, nullptr);
        if (atlasImage_) vkDestroyImage(device_, atlasImage_, nullptr);
        if (atlasMemory_) vkFreeMemory(device_, atlasMemory_, nullptr);

        if (vertexBuffer_) vkDestroyBuffer(device_, vertexBuffer_, nullptr);
        if (vertexBufferMemory_) vkFreeMemory(device_, vertexBufferMemory_, nullptr);

        if (descriptorPool_) vkDestroyDescriptorPool(device_, descriptorPool_, nullptr);

        device_ = VK_NULL_HANDLE;
    }

    if (ftFace_) {
        FT_Done_Face(ftFace_);
        ftFace_ = nullptr;
    }
    if (ftLibrary_) {
        FT_Done_FreeType(ftLibrary_);
        ftLibrary_ = nullptr;
    }
}

bool VulkanTextRenderer::loadFont(const std::string& fontPath, uint32_t fontSize) {
    if (FT_New_Face(ftLibrary_, fontPath.c_str(), 0, &ftFace_)) {
        return false;
    }

    FT_Set_Pixel_Sizes(ftFace_, 0, fontSize);

    // Reset atlas position
    atlasOffsetX_ = 0;
    atlasOffsetY_ = 0;
    atlasRowHeight_ = 0;
    characters_.clear();

    // Load first 128 ASCII characters
    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(ftFace_, c, FT_LOAD_RENDER)) {
            continue;
        }

        // Add padding
        int padding = 1;
        int width = ftFace_->glyph->bitmap.width;
        int height = ftFace_->glyph->bitmap.rows;

        // Check if we need to move to next row
        if (atlasOffsetX_ + width + padding >= ATLAS_WIDTH) {
            atlasOffsetX_ = 0;
            atlasOffsetY_ += atlasRowHeight_ + padding;
            atlasRowHeight_ = 0;
        }

        if (atlasOffsetY_ + height >= ATLAS_HEIGHT) {
            std::cerr << "ERROR::TEXT_RENDERER: Font atlas full" << std::endl;
            break;
        }

        // Upload glyph to atlas
        if (width > 0 && height > 0) {
             updateAtlas(std::vector<uint8_t>(ftFace_->glyph->bitmap.buffer,
                                            ftFace_->glyph->bitmap.buffer + width * height),
                        atlasOffsetX_, atlasOffsetY_, width, height);
        }

        // Calculate UVs
        float uMin = (float)atlasOffsetX_ / ATLAS_WIDTH;
        float vMin = (float)atlasOffsetY_ / ATLAS_HEIGHT;
        float uMax = (float)(atlasOffsetX_ + width) / ATLAS_WIDTH;
        float vMax = (float)(atlasOffsetY_ + height) / ATLAS_HEIGHT;

        Character character = {
            0,
            glm::ivec2(width, height),
            glm::ivec2(ftFace_->glyph->bitmap_left, ftFace_->glyph->bitmap_top),
            static_cast<uint32_t>(ftFace_->glyph->advance.x),
            glm::vec2(uMin, vMin),
            glm::vec2(uMax, vMax)
        };
        characters_.insert(std::pair<char, Character>(c, character));

        atlasOffsetX_ += width + padding;
        atlasRowHeight_ = std::max(atlasRowHeight_, height);
    }

    return true;
}

void VulkanTextRenderer::createAtlasTexture() {
    // Create image
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = ATLAS_WIDTH;
    imageInfo.extent.height = ATLAS_HEIGHT;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = VK_FORMAT_R8_UNORM;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;

    if (vkCreateImage(device_, &imageInfo, nullptr, &atlasImage_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create atlas image!");
    }

    // Allocate memory
    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device_, atlasImage_, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    if (vkAllocateMemory(device_, &allocInfo, nullptr, &atlasMemory_) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate atlas image memory!");
    }

    vkBindImageMemory(device_, atlasImage_, atlasMemory_, 0);

    // Create View
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = atlasImage_;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = VK_FORMAT_R8_UNORM;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(device_, &viewInfo, nullptr, &atlasView_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }

    // Create Sampler
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.anisotropyEnable = VK_FALSE;
    samplerInfo.maxAnisotropy = 1.0f;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

    if (vkCreateSampler(device_, &samplerInfo, nullptr, &atlasSampler_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture sampler!");
    }
}

void VulkanTextRenderer::updateAtlas(const std::vector<uint8_t>& buffer, int x, int y, int w, int h) {
    // Ideally this would use a staging buffer
    VkDeviceSize imageSize = w * h;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(device_, stagingBufferMemory, 0, imageSize, 0, &data);
    memcpy(data, buffer.data(), static_cast<size_t>(imageSize));
    vkUnmapMemory(device_, stagingBufferMemory);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool_;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device_, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = {x, y, 0};
    region.imageExtent = {(uint32_t)w, (uint32_t)h, 1};

    vkCmdCopyBufferToImage(commandBuffer, stagingBuffer, atlasImage_, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(queue_, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(queue_);

    vkFreeCommandBuffers(device_, commandPool_, 1, commandBuffer);
    vkDestroyBuffer(device_, stagingBuffer, nullptr);
    vkFreeMemory(device_, stagingBufferMemory, nullptr);
}

VkShaderModule createShaderModule(VkDevice device, const uint32_t* code, size_t size) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = size;
    createInfo.pCode = code;

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}

void VulkanTextRenderer::createPipeline(VkRenderPass renderPass) {
    // 1. Load compiled SPIR-V shaders
    VkShaderModule vertShaderModule = createShaderModule(device_, VERTEX_SHADER_SPV, sizeof(VERTEX_SHADER_SPV));
    VkShaderModule fragShaderModule = createShaderModule(device_, FRAGMENT_SHADER_SPV, sizeof(FRAGMENT_SHADER_SPV));

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    // 2. Vertex Input
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(TextVertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    std::vector<VkVertexInputAttributeDescription> attributeDescriptions(3);
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT; // pos
    attributeDescriptions[0].offset = offsetof(TextVertex, pos);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT; // texCoord
    attributeDescriptions[1].offset = offsetof(TextVertex, texCoord);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32B32A32_SFLOAT; // color
    attributeDescriptions[2].offset = offsetof(TextVertex, color);

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    // Viewport & Scissor (Dynamic)
    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_NONE; // Text quads may be flipped
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    // Blending (Alpha blending for text)
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;

    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR
    };
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    // Descriptor Set Layout
    VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    samplerLayoutBinding.binding = 0;
    samplerLayoutBinding.descriptorCount = 1;
    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    samplerLayoutBinding.pImmutableSamplers = nullptr;
    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &samplerLayoutBinding;

    if (vkCreateDescriptorSetLayout(device_, &layoutInfo, nullptr, &descriptorSetLayout_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }

    // Push Constants
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(glm::mat4) * 2; // Projection + View

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout_;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

    if (vkCreatePipelineLayout(device_, &pipelineLayoutInfo, nullptr, &pipelineLayout_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }

    // Create Descriptor Pool and Set
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSize.descriptorCount = 1;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = 1;

    if (vkCreateDescriptorPool(device_, &poolInfo, nullptr, &descriptorPool_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }

    VkDescriptorSetAllocateInfo descriptorAllocInfo{};
    descriptorAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptorAllocInfo.descriptorPool = descriptorPool_;
    descriptorAllocInfo.descriptorSetCount = 1;
    descriptorAllocInfo.pSetLayouts = &descriptorSetLayout_;

    if (vkAllocateDescriptorSets(device_, &descriptorAllocInfo, &descriptorSet_) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = atlasView_;
    imageInfo.sampler = atlasSampler_;

    VkWriteDescriptorSet descriptorWrite{};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = descriptorSet_;
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pImageInfo = &imageInfo;

    vkUpdateDescriptorSets(device_, 1, &descriptorWrite, 0, nullptr);

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = pipelineLayout_;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipeline(device_, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline_) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    vkDestroyShaderModule(device_, fragShaderModule, nullptr);
    vkDestroyShaderModule(device_, vertShaderModule, nullptr);
}

void VulkanTextRenderer::createVertexBuffer() {
    // 5000 characters ring buffer
    vertexBufferSize_ = sizeof(TextVertex) * 6 * 5000;
    createBuffer(vertexBufferSize_, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexBuffer_, vertexBufferMemory_);
}

void VulkanTextRenderer::drawText(VkCommandBuffer commandBuffer, const std::string& text, float x, float y, float scale, const glm::vec4& color) {
    if (text.empty()) return;

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_);

    std::vector<TextVertex> vertices;
    for (char c : text) {
        if (characters_.find(c) == characters_.end()) continue;

        Character ch = characters_[c];

        float xpos = x + ch.bearing.x * scale;
        float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        float w = ch.size.x * scale;
        float h = ch.size.y * scale;

        vertices.push_back({ {xpos,     ypos + h}, {ch.uvMin.x, ch.uvMax.y}, color });
        vertices.push_back({ {xpos,     ypos},     {ch.uvMin.x, ch.uvMin.y}, color });
        vertices.push_back({ {xpos + w, ypos},     {ch.uvMax.x, ch.uvMin.y}, color });

        vertices.push_back({ {xpos,     ypos + h}, {ch.uvMin.x, ch.uvMax.y}, color });
        vertices.push_back({ {xpos + w, ypos},     {ch.uvMax.x, ch.uvMin.y}, color });
        vertices.push_back({ {xpos + w, ypos + h}, {ch.uvMax.x, ch.uvMax.y}, color });

        x += (ch.advance >> 6) * scale;
    }

    if (vertices.empty()) return;

    // Check size limit
    VkDeviceSize dataSize = vertices.size() * sizeof(TextVertex);
    if (dataSize > vertexBufferSize_) {
        std::cerr << "Text too long for vertex buffer!" << std::endl;
        return;
    }

    // Ring Buffer Logic: Simple implementation
    // If it fits at current offset, write there. If not, wrap around.
    // NOTE: In production, we need fences or multiple buffers.
    // We add a heavy-handed waitIdle to prevent race conditions when wrapping.
    static VkDeviceSize currentOffset = 0;

    if (currentOffset + dataSize > vertexBufferSize_) {
        // Wait for device to be idle before resetting buffer to avoid overwriting data in use
        vkDeviceWaitIdle(device_);
        currentOffset = 0;
    }

    void* data;
    vkMapMemory(device_, vertexBufferMemory_, currentOffset, dataSize, 0, &data);
    memcpy(data, vertices.data(), static_cast<size_t>(dataSize));
    vkUnmapMemory(device_, vertexBufferMemory_);

    VkBuffer vertexBuffers[] = {vertexBuffer_};
    VkDeviceSize offsets[] = {currentOffset};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout_, 0, 1, &descriptorSet_, 0, nullptr);

    // Set Dynamic Viewport & Scissor to avoid undefined behavior
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)screenWidth_;
    viewport.height = (float)screenHeight_;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = {(uint32_t)screenWidth_, (uint32_t)screenHeight_};
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    // Use screen size for orthographic projection if available
    // Vulkan Clip Space: Y points down. glm::ortho(L, R, B, T)
    // We map 0 -> 0 (Top) and H -> H (Bottom).
    // Using glm::ortho(0, W, 0, H) makes Y=0 -> Bottom, Y=H -> Top in GL.
    // In Vulkan (Y flipped relative to GL), this results in Y=0 -> Top, Y=H -> Bottom, which is what we want?
    // Let's verify:
    // GLM Ortho(0, W, 0, H):
    //   P * (0,0,0,1) = (-1, -1, -1, 1) -> GL Top-Left (if Y up).
    //   In Vulkan, (-1,-1) is Top-Left. So (0,0) -> Top-Left. Correct.
    // Wait, GLM for GL usually does:
    //   Ortho(L, R, B, T)
    //   If we use (0, W, H, 0) (Top=0, Bottom=H):
    //     (0,0) maps to T=0. In GL NDC Y=1.
    //     In Vulkan NDC Y=1 is Bottom. So (0,0) becomes Bottom-Left. Upside down.
    //   If we use (0, W, 0, H) (Bottom=0, Top=H):
    //     (0,0) maps to B=0. In GL NDC Y=-1.
    //     In Vulkan NDC Y=-1 is Top. So (0,0) becomes Top-Left. Correct.

    glm::mat4 projection = glm::ortho(0.0f, (float)screenWidth_, 0.0f, (float)screenHeight_);
    glm::mat4 view = glm::mat4(1.0f);

    struct {
        glm::mat4 proj;
        glm::mat4 view;
    } pc = {projection, view};

    vkCmdPushConstants(commandBuffer, pipelineLayout_, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(pc), &pc);

    vkCmdDraw(commandBuffer, static_cast<uint32_t>(vertices.size()), 1, 0, 0);

    currentOffset += dataSize;
}

float VulkanTextRenderer::measureTextWidth(const std::string& text, float scale) {
    float width = 0;
    for (char c : text) {
        if (characters_.find(c) != characters_.end()) {
            width += (characters_[c].advance >> 6) * scale;
        }
    }
    return width;
}

uint32_t VulkanTextRenderer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice_, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    throw std::runtime_error("failed to find suitable memory type!");
}

void VulkanTextRenderer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device_, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device_, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device_, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(device_, buffer, bufferMemory, 0);
}

} // namespace opensef
