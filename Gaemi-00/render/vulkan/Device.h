//
// Created by gaetz on 07/10/2022.
//

#ifndef RENDER_VULKAN_DEVICE_H
#define RENDER_VULKAN_DEVICE_H

#include <vulkan/vulkan.hpp>
#include "../Defines.h"
#include "../Log.h"
#include <set>
#include <optional>

using std::set;
using std::optional;

namespace engine::render::vulkan::vkInit {

        /**
         * Hold queue indices for the queue families we want
         */
        struct QueueFamilyIndices {
            optional<u32> graphicsFamily;
            optional<u32> presentFamily;

            [[nodiscard]] bool haveAll() const {
                return graphicsFamily.has_value() && presentFamily.has_value();
            }
        };

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
        bool checkDeviceExtensionsSupported(const vk::PhysicalDevice physicalDevice, const vector<const char*>& requestedExtensions) {
            set<str> uniqueExtensions { begin(requestedExtensions), end(requestedExtensions) };
            for (auto& extension : physicalDevice.enumerateDeviceExtensionProperties()) {
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

        /**
         * Find a physical device. First look for discrete GPU, then for integrated GPU.
         * @param instance Vulkan instance
         * @return A chosen physical device. nullptr if no physical device is found.
         */
        VkPhysicalDevice choosePhysicalDevice(vk::Instance& instance) {
            vector<vk::PhysicalDevice> availableDevices = instance.enumeratePhysicalDevices();
            // Look for a discrete GPU first
            for (auto physicalDevice : availableDevices) {
                if (isSuitable(physicalDevice, vk::PhysicalDeviceType::eDiscreteGpu)) {
                    LOG(LogLevel::Info) << "Chosen graphics device:";
                    logDeviceProperties(physicalDevice, LogLevel::Info);
                    return physicalDevice;
                }
            }
            // Look for integrated GPU then
            for (auto physicalDevice : availableDevices) {
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
         * Populate the queue families indices for a specific physical device.
         * @param physicalDevice The physical device we want to check
         * @return Queue families indices
         */
        QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice physicalDevice) {
            QueueFamilyIndices indices;
            vector<vk::QueueFamilyProperties> queueFamilies = physicalDevice.getQueueFamilyProperties();
            i32 i { 0 };
            for (auto queueFamily : queueFamilies) {
                if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
                    indices.graphicsFamily = i;
                    indices.presentFamily = i;
                }
                if(indices.haveAll()) break;
                ++i;
            }
            return indices;
        }

    }

#endif //RENDER_VULKAN_DEVICE_H
