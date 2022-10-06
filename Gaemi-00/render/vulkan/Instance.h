//
// Created by gaetz on 06/10/2022.
//
#ifndef RENDER_VULKAN_INSTANCE_H
#define RENDER_VULKAN_INSTANCE_H

#include <vulkan/vulkan.hpp>
#include "../Defines.h"
#include "../Log.h"

namespace engine::render::vulkan {

    namespace vkInit {
        vk::Instance makeInstance(bool debug, const str& appName) {

            u32 version { 0 };
            vkEnumerateInstanceVersion(&version);
            LOG(LogLevel::Info) << "Making Vulkan instance, version " << version;

            return nullptr;
        }
    }


}
#endif //RENDER_VULKAN_INSTANCE_H
