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
        return device.createCommandPool(poolCreateInfo);
    }

    vk::CommandBuffer makeCommandBuffer(CommandBufferInput input) {
        vk::CommandBufferAllocateInfo commandBufferAllocateInfo {};
        commandBufferAllocateInfo.commandPool = input.commandPool;
        commandBufferAllocateInfo.level = vk::CommandBufferLevel::ePrimary;
        commandBufferAllocateInfo.commandBufferCount = 1;
        for(auto i = 0; i < input.frames.size(); ++i) {
            input.frames[i].commandBuffer = input.device.allocateCommandBuffers(commandBufferAllocateInfo)[0];
        }
        vk::CommandBuffer commandBuffer = input.device.allocateCommandBuffers(commandBufferAllocateInfo)[0];
        LOG(LogLevel::Trace) << "Allocate one command buffer by frame and the main command buffer.";
        return commandBuffer;
    }

}

#endif //BABEL_COMMANDS_H
