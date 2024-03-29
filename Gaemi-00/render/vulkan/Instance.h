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
#include "../Asserts.h"

namespace engine::render::vulkan::vkInit {

        bool supported(vector<const char*>& extensions, vector<const char*>& layers) {
            auto extensionsRes = vk::enumerateInstanceExtensionProperties();
            GASSERT_MSG(extensionsRes.result == vk::Result::eSuccess, "Could not get vulkan extension properties");
            vector<vk::ExtensionProperties> supportedExtensions = extensionsRes.value;
            bool found;
            LOG(LogLevel::Trace) << "Vulkan extensions and layers support status:";
            for (auto extension: extensions) {
                found = false;
                for (auto supportedExtension: supportedExtensions) {
                    if (strcmp(extension, supportedExtension.extensionName) == 0) {
                        found = true;
                        LOG(LogLevel::Trace) << "    Extension " << extension << " is supported.";
                    }
                }
                if (!found) {
                    LOG(LogLevel::Error) << "    Extension " << extension << " is not supported.";
                    return false;
                }
            }

            auto layersRes = vk::enumerateInstanceLayerProperties();
            GASSERT_MSG(layersRes.result == vk::Result::eSuccess, "Could not get vulkan layers properties");
            vector<vk::LayerProperties> supportedLayers = layersRes.value;
            for (auto layer: layers) {
                found = false;
                for (auto supportedLayer: supportedLayers) {
                    if (strcmp(layer, supportedLayer.layerName) == 0) {
                        found = true;
                        LOG(LogLevel::Trace) << "    Layer " << layer << " is supported.";
                    }
                }
                if (!found) {
                    LOG(LogLevel::Error) << "    Layer " << layer << " is not supported.";
                    return false;
                }
            }

            return true;
        }

        vk::Instance makeInstance(const str& appName, WindowVulkan& window) {
            // Vulkan version we have
            u32 vkVersion { 0 };
            vkEnumerateInstanceVersion(&vkVersion);
            // Vulkan version we want
            u32 vkTargetVersion = VK_MAKE_API_VERSION(0, 1, 3, 0);
            LOG(LogLevel::Info) << "Making Vulkan instance, with Vulkan system version "
                                << VK_API_VERSION_MAJOR(vkVersion) << "."
                                << VK_API_VERSION_MINOR(vkVersion) << "."
                                << VK_API_VERSION_PATCH(vkVersion)
                                << ". Target version: "
                                << VK_API_VERSION_MAJOR(vkTargetVersion) << "."
                                << VK_API_VERSION_MINOR(vkTargetVersion) << "."
                                << VK_API_VERSION_PATCH(vkTargetVersion);

            // Application info
            u32 appVersion = VK_MAKE_API_VERSION(0, 0, 1, 0);
            u32 engineVersion = VK_MAKE_API_VERSION(0, 0, 1, 0);

            vk::ApplicationInfo applicationInfo { appName.c_str(), appVersion,
                                                  "Gaemi-00", engineVersion,
                                                  vkTargetVersion };

            // Extensions
            u32 extensionCount;
            SDL_Vulkan_GetInstanceExtensions(window.get(), &extensionCount, nullptr);
            std::vector<const char*> extensions = {
                    VK_EXT_DEBUG_REPORT_EXTENSION_NAME
            };
#ifdef GDEBUG
            extensions.push_back("VK_EXT_debug_utils");
#endif
            auto additionalExtensionCount { extensions.size() };
            extensions.resize(additionalExtensionCount + extensionCount);
            SDL_Vulkan_GetInstanceExtensions(window.get(), &extensionCount,
                                             extensions.data() + additionalExtensionCount);
            LOG(LogLevel::Trace) << "Vulkan extensions:";
            for (auto name: extensions) {
                LOG(LogLevel::Trace) << "    " << name;
            }

            // Layers
            vector<const char*> layers;
#ifdef GDEBUG
            layers.push_back("VK_LAYER_KHRONOS_validation");
#endif
            LOG(LogLevel::Trace) << "Vulkan Layers:";
            for (auto name: layers) {
                LOG(LogLevel::Trace) << "    " << name;
            }

            if (!supported(extensions, layers)) {
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

            auto instanceRes = vk::createInstance(createInfo, nullptr);
            GASSERT_MSG(instanceRes.result == vk::Result::eSuccess, "Could not create vulkan instance");
            return vk::createInstance(createInfo, nullptr).value;
        }

        VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
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
                    //vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
                    //vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo |
                    vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                    vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
                    vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                    vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                    vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
                    debugCallback,
                    nullptr
            };
            auto debugMessenger = instance.createDebugUtilsMessengerEXT(debugCreateInfo, nullptr, aDynamic);
            GASSERT_DEBUG(debugMessenger.result == vk::Result::eSuccess, "Could not create vulkan debug messenger");
            return debugMessenger.value;
        }
    }
#endif //RENDER_VULKAN_INSTANCE_H
