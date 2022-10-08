//
// Created by gaetz on 07/10/2022.
//

#ifndef RENDER_VULKAN_DEVICE_H
#define RENDER_VULKAN_DEVICE_H

#include <vulkan/vulkan.hpp>
#include "../Defines.h"
#include "../Log.h"
#include <set>

using std::set;

namespace engine::render::vulkan {
    namespace vkInit {

        struct QueueFamilyIndices {
            u32 graphicsFamily;
            u32 presentFamily;
        };

        void logDeviceProperties(vk::PhysicalDevice& physicalDevice, LogLevel logLevel) {
            vk::PhysicalDeviceProperties properties = physicalDevice.getProperties();
            str type;
            switch (properties.deviceType) {
                case (vk::PhysicalDeviceType::eCpu):
                    type = "CPU";
                    break;
                case (vk::PhysicalDeviceType::eDiscreteGpu):
                    type = "Discrete GPU";
                    break;
                case (vk::PhysicalDeviceType::eIntegratedGpu):
                    type = "Integrated GPU";
                    break;
                case (vk::PhysicalDeviceType::eVirtualGpu):
                    type = "Virtual GPU";
                    break;
                default:
                    type = "Other";
            }
            LOG(logLevel) << "    Name: " << properties.deviceName;
            LOG(logLevel) << "    Type: " << type;
        }

        bool checkDeviceExtensionsSupported(const vk::PhysicalDevice& physicalDevice, const vector<const char*> requestedExtensions) {
            set<str> uniqueExtensions { begin(requestedExtensions), end(requestedExtensions) };
            for (auto& extension : physicalDevice.enumerateDeviceExtensionProperties()) {
                uniqueExtensions.erase(extension.extensionName);
            }
            return uniqueExtensions.empty();
        }

        bool isSuitable(const vk::PhysicalDevice& physicalDevice, vk::PhysicalDeviceType type) {
            const vector<const char*> requestedExtensions {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
            };
            LOG(LogLevel::Trace) << "Requested Vulkan extensions:";
            for (auto extension : requestedExtensions) {
                LOG(LogLevel::Trace) << "    " << extension;
            }
            bool areExtensionsSupported = checkDeviceExtensionsSupported(physicalDevice, requestedExtensions);
            if (areExtensionsSupported && physicalDevice.getProperties().deviceType == type) {
                LOG(LogLevel::Trace) << "Device " << physicalDevice.getProperties().deviceName << " supports requested extensions.";
                return true;
            }
            return false;
        }

        VkPhysicalDevice choosePhysicalDevice(vk::Instance& instance) {
            vector<vk::PhysicalDevice> availableDevices = instance.enumeratePhysicalDevices();
            // Look for a discrete GPU first
            for (auto& physicalDevice : availableDevices) {
                if (isSuitable(physicalDevice, vk::PhysicalDeviceType::eDiscreteGpu)) {
                    LOG(LogLevel::Info) << "Chosen graphics device:";
                    logDeviceProperties(physicalDevice, LogLevel::Info);
                    return physicalDevice;
                }
            }
            // Look for a integrated GPU then
            for (auto& physicalDevice : availableDevices) {
                if (isSuitable(physicalDevice, vk::PhysicalDeviceType::eIntegratedGpu)) {
                    LOG(LogLevel::Info) << "Chosen graphics device:";
                    logDeviceProperties(physicalDevice, LogLevel::Info);
                    return physicalDevice;
                }
            }
            LOG(LogLevel::Error) << "No device support requested extensions OR is discrete or integrated GPU.";
            return nullptr;
        }

    }
}

#endif //RENDER_VULKAN_DEVICE_H
