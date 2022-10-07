//
// Created by gaetz on 06/10/2022.
//
#ifndef RENDER_VULKAN_INSTANCE_H
#define RENDER_VULKAN_INSTANCE_H

#include <vulkan/vulkan.hpp>
#include "../Defines.h"
#include "../Log.h"
#include "WindowVulkan.h"
#include "SDL_vulkan.h"

namespace engine::render::vulkan {

    namespace vkInit {
        vk::Instance makeInstance(bool debug, const str& appName, WindowVulkan& window) {
            // Vulkan version we have
            u32 vkVersion { 0 };
            vkEnumerateInstanceVersion(&vkVersion);
            LOG(LogLevel::Info) << "Making Vulkan instance, with vkVersion " << VK_API_VERSION_MAJOR(vkVersion) << "."
                                << VK_API_VERSION_MINOR(vkVersion) << "." << VK_API_VERSION_PATCH(vkVersion);
            // Vulkan version we want
            vkVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);

            // Application info
            u32 appVersion = VK_MAKE_API_VERSION(0, 0, 1, 0);
            u32 engineVersion = VK_MAKE_API_VERSION(0, 0, 1, 0);

            vk::ApplicationInfo applicationInfo { appName.c_str(), appVersion,
                                                  "Gaemi-00", engineVersion,
                                                  vkVersion };
            // Extensions
            u32 extensionCount;
            SDL_Vulkan_GetInstanceExtensions(window.get(), &extensionCount, nullptr);
            std::vector<const char*> extensions = {
                    VK_EXT_DEBUG_REPORT_EXTENSION_NAME
            };
            auto additionalExtensionCount { extensions.size() };
            extensions.resize(additionalExtensionCount + extensionCount);
            SDL_Vulkan_GetInstanceExtensions(window.get(), &extensionCount, extensions.data() + additionalExtensionCount);
            LOG(LogLevel::Info) << "Vulkan extensions:";
            for (auto name : extensions) {
                LOG(LogLevel::Info) << "    " << name;
            }
            // Instance creation
            vk::InstanceCreateInfo createInfo {
                vk::InstanceCreateFlags(),
                &applicationInfo,
                // Layers
                0, nullptr,
                // Extensions
                static_cast<u32>(extensions.size()), extensions.data()
            };

            return vk::createInstance(createInfo, nullptr);
        }
    }


}
#endif //RENDER_VULKAN_INSTANCE_H
