//
// Created by gaetz on 09/10/2022.
//

#ifndef RENDER_VULKAN_COMMANDS_H
#define RENDER_VULKAN_COMMANDS_H

#include "QueueFamilies.h"

namespace engine::render::vulkan::vkInit {

    struct CommandBufferInput {
        vk::Device device;
        vk::CommandPool commandPool;
        vector<vkUtils::SwapchainFrame>& frames;
    };

    vk::CommandPool makeCommandPool(vk::Device device, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface) {
        vkUtils::QueueFamilyIndices queueFamilyIndices = vkUtils::findQueueFamilies(physicalDevice, surface);

        vk::CommandPoolCreateInfo poolCreateInfo {};
        poolCreateInfo.flags = vk::CommandPoolCreateFlags() | vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
        poolCreateInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

        auto poolRes = device.createCommandPool(poolCreateInfo);
        GASSERT_MSG(poolRes.result == vk::Result::eSuccess, "Vulkan could not create command pool");
        return poolRes.value;
    }

    vk::CommandBuffer makeCommandBuffer(CommandBufferInput input) {
        vk::CommandBufferAllocateInfo commandBufferAllocateInfo {};
        commandBufferAllocateInfo.commandPool = input.commandPool;
        commandBufferAllocateInfo.level = vk::CommandBufferLevel::ePrimary;
        commandBufferAllocateInfo.commandBufferCount = 1;
        LOG(LogLevel::Trace) << "Allocate the main command buffer";
        auto commandBufferRes = input.device.allocateCommandBuffers(commandBufferAllocateInfo);
        GASSERT_MSG(commandBufferRes.result == vk::Result::eSuccess, "Vulkan could not allocate the main command buffer");
        return commandBufferRes.value[0];
    }

    void makeFrameCommandBuffers(CommandBufferInput input) {
        vk::CommandBufferAllocateInfo commandBufferAllocateInfo {};
        commandBufferAllocateInfo.commandPool = input.commandPool;
        commandBufferAllocateInfo.level = vk::CommandBufferLevel::ePrimary;
        commandBufferAllocateInfo.commandBufferCount = 1;
        for(auto i = 0; i < input.frames.size(); ++i) {
            auto commandBufferRes = input.device.allocateCommandBuffers(commandBufferAllocateInfo);
            GASSERT_MSG(commandBufferRes.result == vk::Result::eSuccess, "Vulkan could not allocate command buffer for frame " + std::to_string(i));
            input.frames[i].commandBuffer = commandBufferRes.value[0];
        }
        LOG(LogLevel::Trace) << "Allocate one command buffer by frame";
    }

}

#endif //RENDER_VULKAN_COMMANDS_H
