//
// Created by gaetz on 09/10/2022.
//

#ifndef RENDER_VULKAN_SYNC_H
#define RENDER_VULKAN_SYNC_H

#include <vulkan/vulkan.hpp>

namespace engine::render::vulkan::vkInit {

    vk::Semaphore makeSemaphore(vk::Device device) {
        vk::SemaphoreCreateInfo semaphoreInfo {};
        semaphoreInfo.flags = vk::SemaphoreCreateFlags();
        return device.createSemaphore(semaphoreInfo);
    }

    vk::Fence makeFence(vk::Device device) {
        vk::FenceCreateInfo fenceInfo {};
        fenceInfo.flags = vk::FenceCreateFlags() | vk::FenceCreateFlagBits::eSignaled;
        return device.createFence(fenceInfo);
    }

}

#endif //RENDER_VULKAN_SYNC_H
