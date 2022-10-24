//
// Created by gaetz on 18/10/2022.
//

#ifndef RENDER_VULKAN_TEXTUREGPU_H
#define RENDER_VULKAN_TEXTUREGPU_H

#include <SDL.h>

#include "../../Defines.h"
#include "Memory.h"

namespace engine::render::vulkan {

struct TextureGPU {
    vkUtils::AllocatedImage textureImage;
    vk::ImageView imageView;
    vk::Sampler sampler;
};

namespace vkUtils {
    void changeImageLayout(vk::Image image, vk::Format format,
                           vk::ImageLayout oldLayout, vk::ImageLayout newLayout,
                           vk::Queue queue, vk::CommandBuffer commandBuffer);

    AllocatedImage createTextureImage(vk::Device device, vk::PhysicalDevice physicalDevice, vk::Queue queue, vk::CommandBuffer commandBuffer,
                       SDL_Surface* surface, i32 width, i32 height);

    vk::Sampler createTextureSampler(vk::Device device);
}

}

#endif //RENDER_VULKAN_TEXTUREGPU_H
