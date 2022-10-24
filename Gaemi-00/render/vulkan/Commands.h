//
// Created by gaetz on 09/10/2022.
//

#ifndef RENDER_VULKAN_COMMANDS_H
#define RENDER_VULKAN_COMMANDS_H

#include "QueueFamilies.h"
#include "../../Asserts.h"

namespace engine::render::vulkan::vkInit {

    struct SingleCommandBufferInput {
        vk::Device device;
        vk::CommandPool commandPool;
        vk::Queue queue;
    };

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

    vk::CommandBuffer beginSingleCommand(SingleCommandBufferInput input) {
        vk::CommandBufferAllocateInfo commandBufferAllocateInfo {};
        commandBufferAllocateInfo.commandPool = input.commandPool;
        commandBufferAllocateInfo.level = vk::CommandBufferLevel::ePrimary;
        commandBufferAllocateInfo.commandBufferCount = 1;

        auto commandBufferRes = input.device.allocateCommandBuffers(commandBufferAllocateInfo);
        //GASSERT_MSG(commandBufferRes.result == vk::Result::eSuccess, "Vulkan could not allocate a single command buffer");

        auto commandBuffer = commandBufferRes.value[0];
        vk::CommandBufferBeginInfo beginInfo {};
        beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
        auto beginRes = commandBuffer.begin(beginInfo);
        return commandBuffer;
    }

    void endSingleCommand(vk::CommandBuffer& commandBuffer, SingleCommandBufferInput input) {
        auto endRes = commandBuffer.end();

        // Submit command
        vk::SubmitInfo submitInfo {};
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;
        auto submitRes = input.queue.submit(1, &submitInfo, nullptr);
        auto waitRes = input.queue.waitIdle();
        input.device.freeCommandBuffers(input.commandPool, 1, &commandBuffer);
    }
}

#endif //RENDER_VULKAN_COMMANDS_H
