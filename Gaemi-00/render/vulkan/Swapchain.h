//
// Created by gaetz on 08/10/2022.
//

#ifndef RENDER_VULKAN_SWAPCHAIN_H
#define RENDER_VULKAN_SWAPCHAIN_H

#include "Device.h"
#include "Frame.h"

using std::vector;

namespace engine::render::vulkan::vkInit {

    /**
    * Bundle possibilities of a swapchain
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
        vector<vkUtils::SwapchainFrame> frames;
        vk::Format format;
        vk::Extent2D extent;
    };

    /**
     * Get family queues we need
     * @param physicalDevice Queried physical device
     * @param device Current logical device
     * @param surface Surface we use for queues lookup
     * @return Array of queues
     */
    array<vk::Queue, 2> getQueues(vk::PhysicalDevice physicalDevice, vk::Device device, vk::SurfaceKHR surface) {
        vkUtils::QueueFamilyIndices indices = vkUtils::findQueueFamilies(physicalDevice, surface);
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
        for (auto format: formats) {
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
    vk::PresentModeKHR chooseSwapchainPresentMode(const vector<vk::PresentModeKHR>& modes) {
        for (auto presentMode: modes) {
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
    SwapchainBundle
    createSwapchain(vk::Device device, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, i32 width,
                    i32 height) {
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
        vkUtils::QueueFamilyIndices indices = vkUtils::findQueueFamilies(physicalDevice, surface);
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
        vector<vk::Image> images = device.getSwapchainImagesKHR(bundle.swapchain);
        bundle.frames.resize(images.size());
        for(size_t i = 0; i < images.size(); ++i) {
            vk::ImageViewCreateInfo createInfo {};
            createInfo.image = images[i];
            createInfo.viewType = vk::ImageViewType::e2D;
            createInfo.components.r = vk::ComponentSwizzle::eR;
            createInfo.components.g = vk::ComponentSwizzle::eG;
            createInfo.components.b = vk::ComponentSwizzle::eB;
            createInfo.components.a = vk::ComponentSwizzle::eA;
            createInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;
            createInfo.format = format.format;
            bundle.frames[i].image = images[i];
            bundle.frames[i].imageView = device.createImageView(createInfo);
        }
        bundle.format = format.format;
        bundle.extent = extent;

        LOG(LogLevel::Trace) << "Vulkan swapchain creation";
        return bundle;
    }

}

#endif //RENDER_VULKAN_SWAPCHAIN_H
