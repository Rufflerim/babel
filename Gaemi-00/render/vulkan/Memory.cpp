//
// Created by gaetz on 12/10/2022.
//

#include "Memory.h"

namespace engine::render::vulkan::vkUtils {

    u32 findMemoryTypeIndex(vk::PhysicalDevice physicalDevice, u32 supportedMemoryIndices,
                            vk::MemoryPropertyFlags requestedProperties) {
        vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();
        for (u32 i = 0; i < memoryProperties.memoryTypeCount; ++i) {
            // bit i of supportedMemoryIndices is set if that memory type is supported by the device
            bool supported{ static_cast<bool>(supportedMemoryIndices & (1 << i)) };

            // propertyFlags holds all the memory properties supported by this memory type
            bool sufficient{ (memoryProperties.memoryTypes[i].propertyFlags & requestedProperties) == requestedProperties };

            if (supported && sufficient) {
                return i;
            }
        }
        return 0;
    }

    void allocateBufferMemory(Buffer& buffer, const BufferInput& input) {
        vk::MemoryRequirements memoryRequirements = input.device.getBufferMemoryRequirements(buffer.buffer);
        vk::MemoryAllocateInfo allocateInfo;
        allocateInfo.allocationSize = memoryRequirements.size;
        allocateInfo.memoryTypeIndex = findMemoryTypeIndex(input.physicalDevice, memoryRequirements.memoryTypeBits,
                                                           vk::MemoryPropertyFlagBits::eHostVisible |
                                                           vk::MemoryPropertyFlagBits::eHostCoherent);
        buffer.bufferMemory = input.device.allocateMemory(allocateInfo);
        input.device.bindBufferMemory(buffer.buffer, buffer.bufferMemory, 0);
    }


    Buffer createBuffer(BufferInput input) {
        vk::BufferCreateInfo bufferInfo {};
        bufferInfo.flags = vk::BufferCreateFlags();
        bufferInfo.size = input.size;
        bufferInfo.usage = input.usageFlags;
        bufferInfo.sharingMode = vk::SharingMode::eExclusive;

        Buffer buffer;
        buffer.buffer = input.device.createBuffer(bufferInfo);
        allocateBufferMemory(buffer, input);
        return buffer;
    }

}