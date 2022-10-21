//
// Created by gaetz on 18/10/2022.
//

#ifndef RENDER_VULKAN_TEXTUREGPU_H
#define RENDER_VULKAN_TEXTUREGPU_H

#include <vulkan/vulkan.hpp>
#include "../../Defines.h"
#include "Memory.h"

namespace engine::render::vulkan {

struct TextureGPU {
    vkUtils::AllocatedImage textureImage;
    vk::ImageView imageView;
    vk::Sampler sampler;
};

}

#endif //RENDER_VULKAN_TEXTUREGPU_H
