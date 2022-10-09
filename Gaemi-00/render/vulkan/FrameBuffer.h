//
// Created by gaetz on 09/10/2022.
//

#ifndef RENDER_VULKAN_FRAMEBUFFER_H
#define RENDER_VULKAN_FRAMEBUFFER_H

#include <vulkan/vulkan.hpp>
#include "Frame.h"
#include "../../Defines.h"

using std::vector;

namespace engine::render::vulkan::vkInit {

    struct FramebufferInput {
        vk::Device device;
        vk::RenderPass renderPass;
        vk::Extent2D extent;
    };

    void makeFramebuffers(FramebufferInput framebufferInput, vector<vkUtils::SwapchainFrame>& frames) {
        for (size_t i = 0; i < frames.size(); ++i) {
            vector<vk::ImageView> attachments = { frames[i].imageView };
            vk::FramebufferCreateInfo framebufferInfo {};
            framebufferInfo.flags = vk::FramebufferCreateFlags();
            framebufferInfo.renderPass = framebufferInput.renderPass;
            framebufferInfo.attachmentCount = attachments.size();
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = framebufferInput.extent.width;
            framebufferInfo.height = framebufferInput.extent.height;
            framebufferInfo.layers = 1;
            LOG(LogLevel::Trace) << "Create framebuffer for image " << i;
            frames[i].framebuffer = framebufferInput.device.createFramebuffer(framebufferInfo);
        }
    }

}

#endif //RENDER_VULKAN_FRAMEBUFFER_H
