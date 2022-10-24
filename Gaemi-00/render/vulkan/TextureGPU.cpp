//
// Created by gaetz on 23/10/2022.
//

#include "TextureGPU.h"

namespace engine::render::vulkan::vkUtils {


    void changeImageLayout(vk::Image image, vk::Format format,
                           vk::ImageLayout oldLayout, vk::ImageLayout newLayout,
                           vk::Queue queue, vk::CommandBuffer commandBuffer) {
        // Reset and begin command buffer, for one time
        commandBuffer.reset();
        vk::CommandBufferBeginInfo beginInfo {};
        beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
        commandBuffer.begin(beginInfo);

        vk::ImageMemoryBarrier barrier {};
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = image;
        barrier.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        vk::PipelineStageFlags srcStageFlags;
        vk::PipelineStageFlags dstStageFlags;
        if (oldLayout == vk::ImageLayout::eUndefined && newLayout == vk::ImageLayout::eTransferDstOptimal) {
            barrier.srcAccessMask = vk::AccessFlagBits::eNone;
            barrier.dstAccessMask = vk::AccessFlagBits::eTransferWrite;
            srcStageFlags = vk::PipelineStageFlagBits::eTopOfPipe;
            dstStageFlags = vk::PipelineStageFlagBits::eTransfer;
        } else if (oldLayout == vk::ImageLayout::eTransferDstOptimal &&
                   newLayout == vk::ImageLayout::eShaderReadOnlyOptimal) {
            barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
            barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
            srcStageFlags = vk::PipelineStageFlagBits::eTransfer;
            dstStageFlags = vk::PipelineStageFlagBits::eFragmentShader;
        }

        commandBuffer.pipelineBarrier(srcStageFlags, dstStageFlags, vk::DependencyFlags(), 0, nullptr, 0, nullptr, 1, &barrier);
        commandBuffer.end();

        // Submit command
        vk::SubmitInfo submitInfo {};
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;
        auto submitRes = queue.submit(1, &submitInfo, nullptr);
        queue.waitIdle();
    }

    AllocatedImage createTextureImage(vk::Device device, vk::PhysicalDevice physicalDevice, vk::Queue queue,
                                  vk::CommandBuffer commandBuffer, SDL_Surface* surface, i32 width, i32 height) {
        vkUtils::BufferInput input {};
        input.device = device;
        input.physicalDevice = physicalDevice;
        input.size = width * height * 4;
        input.usageFlags = vk::BufferUsageFlagBits::eTransferSrc;
        input.memoryProperties = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;

        Buffer stagingBuffer = createBuffer(input);

        // Load data on staging buffer
        void* memoryLocation = device.mapMemory(stagingBuffer.bufferMemory, 0, input.size);
        memcpy(memoryLocation, surface->pixels, input.size);
        device.unmapMemory(stagingBuffer.bufferMemory);

        AllocatedImage textureImage;
        vk::ImageUsageFlags flags = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
        vk::MemoryPropertyFlagBits memoryProperties = vk::MemoryPropertyFlagBits::eDeviceLocal;
        textureImage = createImage(device, physicalDevice, width, height, vk::Format::eR8G8B8A8Srgb,
                                   vk::ImageTiling::eOptimal, flags, memoryProperties);



        changeImageLayout(textureImage.image, vk::Format::eR8G8B8A8Srgb,
                          vk::ImageLayout::eUndefined, vk::ImageLayout::eTransferDstOptimal, queue, commandBuffer);
        copyBufferToImage(stagingBuffer, textureImage.image, width, height, queue, commandBuffer);
        changeImageLayout(textureImage.image, vk::Format::eR8G8B8A8Srgb,
                          vk::ImageLayout::eTransferDstOptimal, vk::ImageLayout::eShaderReadOnlyOptimal, queue, commandBuffer);

        device.destroyBuffer(stagingBuffer.buffer);
        device.freeMemory(stagingBuffer.bufferMemory);

        return textureImage;
    }

    vk::Sampler createTextureSampler(vk::Device device) {
        vk::SamplerCreateInfo samplerInfo {};
        samplerInfo.flags = vk::SamplerCreateFlags();
        samplerInfo.magFilter = vk::Filter::eLinear;
        samplerInfo.minFilter = vk::Filter::eLinear;
        samplerInfo.addressModeU = vk::SamplerAddressMode::eRepeat;
        samplerInfo.addressModeV = vk::SamplerAddressMode::eRepeat;
        samplerInfo.addressModeW = vk::SamplerAddressMode::eRepeat;
        samplerInfo.anisotropyEnable = true;
        samplerInfo.maxAnisotropy = 4;
        samplerInfo.borderColor = vk::BorderColor::eIntOpaqueBlack;
        samplerInfo.unnormalizedCoordinates = false;
        samplerInfo.compareEnable = false;
        samplerInfo.compareOp = vk::CompareOp::eAlways;
        samplerInfo.mipmapMode = vk::SamplerMipmapMode::eLinear;
        samplerInfo.mipLodBias = 0.0f;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = 0.0f;

        return device.createSampler(samplerInfo);
    }


}