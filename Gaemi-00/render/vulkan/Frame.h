//
// Created by gaetz on 08/10/2022.
//

#ifndef RENDER_VULKAN_FRAME_H
#define RENDER_VULKAN_FRAME_H

#include <vulkan/vulkan.hpp>

namespace engine::render::vulkan::vkUtils {

    struct SwapchainFrame {
        vk::Image image;
        vk::ImageView imageView;
    };

}


#endif //RENDER_VULKAN_FRAME_H
