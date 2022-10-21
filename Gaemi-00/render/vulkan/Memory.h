//
// Created by gaetz on 12/10/2022.
//

#include <vulkan/vulkan.hpp>
#include "../../Defines.h"

#ifndef RENDER_VULKAN_MEMORY_H
#define RENDER_VULKAN_MEMORY_H

namespace engine::render::vulkan::vkUtils {

    struct Buffer {
        vk::Buffer buffer;
        vk::DeviceMemory bufferMemory;
    };

    struct BufferInput {
        size_t size;
        vk::BufferUsageFlags usageFlags;
        vk::Device device;
        vk::PhysicalDevice physicalDevice;
        vk::MemoryPropertyFlags memoryProperties;
    };

    struct AllocatedImage {
        vk::Image image;
        vk::DeviceMemory memory;
    };

    u32 findMemoryTypeIndex(vk::PhysicalDevice physicalDevice, u32 supportedMemoryIndices,
                            vk::MemoryPropertyFlags requestedProperties);

    void allocateBufferMemory(Buffer& buffer, const BufferInput& input);

    Buffer createBuffer(BufferInput input);

    void copyBuffer(Buffer& srcBuffer, Buffer& dstBuffer, vk::DeviceSize bufferSize,
                    vk::Queue queue, vk::CommandBuffer commandBuffer);

    AllocatedImage createImage(vk::Device device, vk::PhysicalDevice physicalDevice, u32 width, u32 height, vk::Format format, vk::ImageTiling tiling,
                               vk::ImageUsageFlags usageFlags, vk::MemoryPropertyFlags memoryProperties);
}

#endif //RENDER_VULKAN_MEMORY_H
