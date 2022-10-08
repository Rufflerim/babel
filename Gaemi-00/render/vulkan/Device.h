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
        optional <u32> graphicsFamily;
        optional <u32> presentFamily;

        [[nodiscard]] bool haveAll() const {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    /**
     * Bundle possibilities of q swapchain
     */
    struct SwapchainSupportDetails {
        vk::SurfaceCapabilitiesKHR capabilities;
        vector<vk::SurfaceFormatKHR> formats;
        vector<vk::PresentModeKHR> presentModes;
    };

    /**
     * Bundle swapchain related data
     */
    struct SwapchainBundle {
        vk::SwapchainKHR swapchain;
        vector<vk::Image> images;
        vk::Format format;
        vk::Extent2D extent;
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
     * Populate the queue families indices for a specific physical device.
     * @param physicalDevice The physical device we want to check
     * @return Queue families indices
     */
    QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface) {
        QueueFamilyIndices indices;
        vector<vk::QueueFamilyProperties> queueFamilies = physicalDevice.getQueueFamilyProperties();
        i32 i { 0 };
        for (auto queueFamily: queueFamilies) {
            if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
                indices.graphicsFamily = i;
            }
            if (physicalDevice.getSurfaceSupportKHR(i, surface)) {
                indices.presentFamily = i;
            }
            if (indices.haveAll()) break;
            ++i;
        }
        return indices;
    }

    /**
     * Create a Vulkan logical device for future rendering
     * @param physicalDevice Physical device the logical device is based on
     * @param surface Surface on which we will render
     * @param debugMode True if we want debug info
     * @return Vulkan logical device
     */
    vk::Device createLogicalDevice(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, bool debugMode) {
        QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);
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

    /**
     * Get family queues we need
     * @param physicalDevice Queried physical device
     * @param device Current logical device
     * @param surface Surface we use for queues lookup
     * @return Array of queues
     */
    array<vk::Queue, 2> getQueues(vk::PhysicalDevice physicalDevice, vk::Device device, vk::SurfaceKHR surface) {
        QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);
        return {{
            device.getQueue(indices.graphicsFamily.value(), 0),
            device.getQueue(indices.presentFamily.value(), 0)
        }};
    }

    /**
     * Get all swapchain possibilities info
     * @param physicalDevice Queried physical device
     * @param surface Queried surface
     * @return A bundle of swapchain info
     */
    SwapchainSupportDetails querySwapchainSupport(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface) {
        SwapchainSupportDetails supportDetails;
        supportDetails.capabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
        supportDetails.formats = physicalDevice.getSurfaceFormatsKHR(surface);
        supportDetails.presentModes = physicalDevice.getSurfacePresentModesKHR(surface);
        return supportDetails;
    }

    /**
     * Choose a specific format for our swapchain if available
     * @param formats Available formats
     * @return The format we hardcoded details, or the first format available if looked up format cannot be found
     */
    vk::SurfaceFormatKHR chooseSwapchainSurfaceFormat(vector<vk::SurfaceFormatKHR> formats) {
        for (auto format : formats) {
            if (format.format == vk::Format::eB8G8R8A8Unorm && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
                return format;
            }
        }
        return formats[0];
    }

    /**
     * Choose a specific present mode for our swapchain if available
     * @param modes Available present modes
     * @return Hardcoded present mode if available, else return FIFO
     */
    vk::PresentModeKHR chooseSwapchainPresentMode(vector<vk::PresentModeKHR> modes) {
        for (auto presentMode : modes) {
            if (presentMode == vk::PresentModeKHR::eMailbox) {
                return presentMode;
            }
        }
        return vk::PresentModeKHR::eFifo;
    }

    /**
     * Choose swapchain extent, handling specific cases
     * @param width Requested width
     * @param height Requested height
     * @param surfaceCapabilities Available surface capabilities
     * @return Swapchain extent
     */
    vk::Extent2D chooseSwapchainExtent(u32 width, u32 height, vk::SurfaceCapabilitiesKHR surfaceCapabilities) {
        if (surfaceCapabilities.currentExtent.width != UINT32_MAX) {
            return surfaceCapabilities.currentExtent;
        }
        vk::Extent2D extent { width, height };
        extent.width = std::min(surfaceCapabilities.maxImageExtent.width,
                                std::max(surfaceCapabilities.minImageExtent.width, width));
        extent.height = std::min(surfaceCapabilities.maxImageExtent.height,
                                std::max(surfaceCapabilities.minImageExtent.height, height));
        return extent;
    }

    /**
     * Create a swapchain bundle that contains : swapchain, swapchain images, format and extent
     * @param device Used logical device
     * @param physicalDevice Used physical device
     * @param surface Target surface
     * @param width Targeted width
     * @param height Targeted height
     * @return A swapchain bundle
     */
    SwapchainBundle createSwapchain(vk::Device device, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, i32 width, i32 height) {
        // Create info
        SwapchainSupportDetails support = querySwapchainSupport(physicalDevice, surface);
        vk::SurfaceFormatKHR format = chooseSwapchainSurfaceFormat(support.formats);
        vk::PresentModeKHR presentMode = chooseSwapchainPresentMode(support.presentModes);
        vk::Extent2D extent = chooseSwapchainExtent(width, height, support.capabilities);
        u32 imageCount = std::min(support.capabilities.maxImageCount, support.capabilities.minImageCount + 1);
        vk::SwapchainCreateInfoKHR swapchainCreateInfo {
            vk::SwapchainCreateFlagsKHR(), surface, imageCount, format.format,
            format.colorSpace, extent, 1, vk::ImageUsageFlagBits::eColorAttachment
        };

        // In case graphics queue index is different from present queue index
        QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);
        array<u32, 2> queueFamilyIndices { indices.graphicsFamily.value(), indices.presentFamily.value() };
        if (queueFamilyIndices.at(0) != queueFamilyIndices.at(1)) {
            swapchainCreateInfo.imageSharingMode = vk::SharingMode::eConcurrent;
            swapchainCreateInfo.queueFamilyIndexCount = 2;
            swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices.data();
        }

        // Other info
        swapchainCreateInfo.preTransform = support.capabilities.currentTransform;
        swapchainCreateInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        swapchainCreateInfo.presentMode = presentMode;
        swapchainCreateInfo.clipped = VK_TRUE;

        SwapchainBundle bundle {};
        bundle.swapchain = device.createSwapchainKHR(swapchainCreateInfo);
        bundle.images = device.getSwapchainImagesKHR(bundle.swapchain);
        bundle.format = format.format;
        bundle.extent = extent;

        LOG(LogLevel::Trace) << "Vulkan swapchain creation";
        return bundle;
    }

}

#endif //RENDER_VULKAN_DEVICE_H
