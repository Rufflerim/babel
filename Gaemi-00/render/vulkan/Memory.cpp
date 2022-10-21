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
                                                           input.memoryProperties);
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

    void copyBuffer(Buffer& srcBuffer, Buffer& dstBuffer, vk::DeviceSize size,
                    vk::Queue queue, vk::CommandBuffer commandBuffer) {

        // Reset and begin command buffer, for one time
        commandBuffer.reset();
        vk::CommandBufferBeginInfo beginInfo {};
        beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
        commandBuffer.begin(beginInfo);

        // Copy and end command
        vk::BufferCopy copyRegion;
        copyRegion.srcOffset = 0;
        copyRegion.dstOffset = 0;
        copyRegion.size = size;
        commandBuffer.copyBuffer(srcBuffer.buffer, dstBuffer.buffer, 1, &copyRegion);
        commandBuffer.end();

        // Submit command
        vk::SubmitInfo submitInfo {};
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;
        auto subbitRes = queue.submit(1, &submitInfo, nullptr);
        queue.waitIdle();
    }

    AllocatedImage createImage(vk::Device device, vk::PhysicalDevice physicalDevice, u32 width, u32 height, vk::Format format, vk::ImageTiling tiling,
                               vk::ImageUsageFlags usageFlags, vk::MemoryPropertyFlags memoryProperties) {
        vk::ImageCreateInfo imageInfo {};
        imageInfo.flags = vk::ImageCreateFlags();
        imageInfo.imageType = vk::ImageType::e2D;
        imageInfo.extent.width = width;
        imageInfo.extent.height = height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = format;
        imageInfo.tiling = tiling;
        imageInfo.initialLayout = vk::ImageLayout::eUndefined;
        imageInfo.usage = usageFlags;
        imageInfo.sharingMode = vk::SharingMode::eExclusive;
        imageInfo.samples = vk::SampleCountFlagBits::e1;

        AllocatedImage allocatedImage;
        allocatedImage.image = device.createImage(imageInfo);

        vk::MemoryRequirements memoryRequirements;
        device.getImageMemoryRequirements(allocatedImage.image, &memoryRequirements);
        vk::MemoryAllocateInfo allocateInfo {};
        allocateInfo.allocationSize = memoryRequirements.size;
        allocateInfo.memoryTypeIndex = findMemoryTypeIndex(physicalDevice, memoryRequirements.memoryTypeBits,
                                                           memoryProperties);
        allocatedImage.memory = device.allocateMemory(allocateInfo);

        device.bindImageMemory(allocatedImage.image, allocatedImage.memory, 0);
        return allocatedImage;
    }

}