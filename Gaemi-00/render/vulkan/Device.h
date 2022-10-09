//
// Created by gaetz on 07/10/2022.
//

#ifndef RENDER_VULKAN_DEVICE_H
#define RENDER_VULKAN_DEVICE_H

#include <vulkan/vulkan.hpp>
#include "../Defines.h"
#include "../Log.h"
#include <set>
#include "QueueFamilies.h"

using std::set;

namespace engine::render::vulkan::vkInit {

    /**
     * Log properties of a specific physical device
     * @param physicalDevice The physical device we want to log
     * @param logLevel Level of logging
     */
    void logDeviceProperties(vk::PhysicalDevice physicalDevice, LogLevel logLevel) {
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

    /**
     * Check extensions are supported by the physical device
     * @param physicalDevice The physical device we want to check
     * @param requestedExtensions Extensions we request
     * @return true if all extensions are supported
     */
    bool checkDeviceExtensionsSupported(const vk::PhysicalDevice physicalDevice,
                                        const vector<const char*>& requestedExtensions) {
        set<str> uniqueExtensions { begin(requestedExtensions), end(requestedExtensions) };
        for (auto& extension: physicalDevice.enumerateDeviceExtensionProperties()) {
            uniqueExtensions.erase(extension.extensionName);
        }
        return uniqueExtensions.empty();
    }

    /**
     * Checks if a physical device supports requested extensions and is of a specific type (discrete, integrated, etc.). Requested extensions must be defined in this function.
     * @param physicalDevice The physical device we want to check
     * @param type The type of physical physical device we want
     * @return true when extensions are supported and physical device is of the requested type
     */
    bool isSuitable(const vk::PhysicalDevice physicalDevice, vk::PhysicalDeviceType type) {
        const vector<const char*> requestedExtensions {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
        LOG(LogLevel::Trace) << "Requested Vulkan extensions:";
        for (auto extension: requestedExtensions) {
            LOG(LogLevel::Trace) << "    " << extension;
        }
        bool areExtensionsSupported = checkDeviceExtensionsSupported(physicalDevice, requestedExtensions);
        if (areExtensionsSupported && physicalDevice.getProperties().deviceType == type) {
            LOG(LogLevel::Trace) << "Device " << physicalDevice.getProperties().deviceName
                                 << " supports requested extensions.";
            return true;
        }
        return false;
    }

    /**
     * Find a physical device. First look for discrete GPU, then for integrated GPU.
     * @param instance Vulkan instance
     * @return A chosen physical device. nullptr if no physical device is found.
     */
    VkPhysicalDevice choosePhysicalDevice(vk::Instance& instance) {
        vector<vk::PhysicalDevice> availableDevices = instance.enumeratePhysicalDevices();
        // Look for a discrete GPU first
        for (auto physicalDevice: availableDevices) {
            if (isSuitable(physicalDevice, vk::PhysicalDeviceType::eDiscreteGpu)) {
                LOG(LogLevel::Info) << "Chosen graphics device:";
                logDeviceProperties(physicalDevice, LogLevel::Info);
                return physicalDevice;
            }
        }
        // Look for integrated GPU then
        for (auto physicalDevice: availableDevices) {
            if (isSuitable(physicalDevice, vk::PhysicalDeviceType::eIntegratedGpu)) {
                LOG(LogLevel::Info) << "Chosen graphics device:";
                logDeviceProperties(physicalDevice, LogLevel::Info);
                return physicalDevice;
            }
        }
        LOG(LogLevel::Error) << "No device support requested extensions OR is discrete or integrated GPU.";
        return nullptr;
    }

    /**
     * Create a Vulkan logical device for future rendering
     * @param physicalDevice Physical device the logical device is based on
     * @param surface Surface on which we will render
     * @param debugMode True if we want debug info
     * @return Vulkan logical device
     */
    vk::Device createLogicalDevice(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, bool debugMode) {
        vkUtils::QueueFamilyIndices indices = vkUtils::findQueueFamilies(physicalDevice, surface);
        vector<u32> uniqueIndices;
        uniqueIndices.push_back(indices.graphicsFamily.value());
        if (indices.graphicsFamily.value() != indices.presentFamily.value()) {
            uniqueIndices.push_back(indices.presentFamily.value());
        }

        float queuePriority { 1.0f };
        vector<vk::DeviceQueueCreateInfo> queueCreateInfoList;
        for (auto queueFamilyIndex: uniqueIndices) {
            queueCreateInfoList.emplace_back(
                    vk::DeviceQueueCreateFlags(), queueFamilyIndex,
                    1, &queuePriority
            );
        }

        vector<const char*> deviceExtensions {
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        vk::PhysicalDeviceFeatures deviceFeatures {};

        vector<const char*> enabledLayers;
        if (debugMode) {
            enabledLayers.push_back("VK_LAYER_KHRONOS_validation");
        }

        vk::DeviceCreateInfo createInfo {
                vk::DeviceCreateFlags(),
                static_cast<u32>(queueCreateInfoList.size()), queueCreateInfoList.data(),
                static_cast<u32>(enabledLayers.size()), enabledLayers.data(),
                static_cast<u32>(deviceExtensions.size()), deviceExtensions.data(),
                &deviceFeatures
        };

        LOG(LogLevel::Trace) << "Vulkan logical device creation.";
        vk::Device device = physicalDevice.createDevice(createInfo);
        return device;
    }
}

#endif //RENDER_VULKAN_DEVICE_H
