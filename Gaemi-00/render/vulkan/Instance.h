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

        bool supported(vector<const char*>& extensions, vector<const char*>& layers, bool debugMode) {
            vector<vk::ExtensionProperties> supportedExtensions = vk::enumerateInstanceExtensionProperties();
            bool found;
            for(auto extension : extensions) {
                found = false;
                for (auto supportedExtension : supportedExtensions) {
                    if (strcmp(extension, supportedExtension.extensionName) == 0) {
                        found = true;
                        if (debugMode) {
                            LOG(LogLevel::Trace) << "    Extension " << extension << " is supported.";
                        }
                    }
                }
                if (!found) {
                    if (debugMode) {
                        LOG(LogLevel::Error) << "    Extension " << extension << " is not supported.";
                    }
                    return false;
                }
            }

            vector<vk::LayerProperties> supportedLayers = vk::enumerateInstanceLayerProperties();
            for(auto layer : layers) {
                found = false;
                for (auto supportedLayer : supportedLayers) {
                    if (strcmp(layer, supportedLayer.layerName) == 0) {
                        found = true;
                        if (debugMode) {
                            LOG(LogLevel::Trace) << "    Layer " << layer << " is supported.";
                        }
                    }
                }
                if (!found) {
                    if (debugMode) {
                        LOG(LogLevel::Error) << "    Layer " << layer << " is not supported.";
                    }
                    return false;
                }
            }

            return true;
        }

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
            if (debug) {
                extensions.push_back("VK_EXT_debug_utils");
            }
            auto additionalExtensionCount { extensions.size() };
            extensions.resize(additionalExtensionCount + extensionCount);
            SDL_Vulkan_GetInstanceExtensions(window.get(), &extensionCount, extensions.data() + additionalExtensionCount);
            LOG(LogLevel::Info) << "Vulkan extensions:";
            for (auto name : extensions) {
                LOG(LogLevel::Info) << "    " << name;
            }

            // Layers
            vector<const char*> layers;
            if (debug) {
                layers.push_back("VK_LAYER_KHRONOS_validation");
            }

            if(!supported(extensions, layers, debug)) {
                LOG(LogLevel::Fatal) << "Requested Vulkan extensions and layer not supported.";
                return nullptr;
            }

            // Instance creation
            vk::InstanceCreateInfo createInfo {
                vk::InstanceCreateFlags(),
                &applicationInfo,
                // Layers
                static_cast<u32>(layers.size()), layers.data(),
                // Extensions
                static_cast<u32>(extensions.size()), extensions.data()
            };

            return vk::createInstance(createInfo, nullptr);
        }

        VKAPI_ATTR VkBool32  VKAPI_CALL debugCallback(
                VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                VkDebugUtilsMessageTypeFlagsEXT messageType,
                const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                void* pUserData
        ) {
            LogLevel level;
            switch (messageSeverity) {
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
                    level = LogLevel::Error;
                    break;
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
                    level = LogLevel::Warning;
                    break;
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
                    level = LogLevel::Info;
                    break;
                case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
                    level = LogLevel::Trace;
                    break;
                default:
                    level = LogLevel::Debug;
                    break;
            }
            str debugType;
            switch (messageType) {
                case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
                    debugType = "general";
                    break;
                case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
                    debugType = "performance";
                    break;
                case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
                    debugType = "validation";
                    break;
                default:
                    debugType = "general";
                    break;
            }

            LOG(level) << "[VULKAN][" << debugType << "] " << pCallbackData->pMessage;
            return VK_FALSE;
        }

        vk::DebugUtilsMessengerEXT makeDebugMessenger(vk::Instance instance, vk::DispatchLoaderDynamic aDynamic) {
            vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo {
                    vk::DebugUtilsMessengerCreateFlagsEXT(),
                    vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
                    vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo |
                    vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                    vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
                    vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                    vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                    vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
                    debugCallback,
                    nullptr
            };
            return instance.createDebugUtilsMessengerEXT(debugCreateInfo, nullptr, aDynamic);
        }
    }


}
#endif //RENDER_VULKAN_INSTANCE_H
