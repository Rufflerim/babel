//
// Created by gaetz on 08/10/2022.
//

#ifndef RENDER_VULKAN_SWAPCHAIN_H
#define RENDER_VULKAN_SWAPCHAIN_H

#include "Device.h"
#include "Frame.h"
#include "RenderData.h"

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
        supportDetails.capabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface).value;
        supportDetails.formats = physicalDevice.getSurfaceFormatsKHR(surface).value;
        supportDetails.presentModes = physicalDevice.getSurfacePresentModesKHR(surface).value;
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
                    i32 height, vk::DescriptorPool descriptorPool, vk::DescriptorSetLayout descriptorSetLayout) {
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

        // Create swapchain
        auto swapchainRes = device.createSwapchainKHR(swapchainCreateInfo);
        GASSERT_MSG(swapchainRes.result == vk::Result::eSuccess, "Vulkan could not create swapchain")
        bundle.swapchain = swapchainRes.value;

        // Gather images
        auto imagesRes = device.getSwapchainImagesKHR(bundle.swapchain);
        GASSERT_MSG(imagesRes.result == vk::Result::eSuccess, "Vulkan could not get swapchain images")
        vector<vk::Image> images = imagesRes.value;

        // Create image related storage
        bundle.frames.resize(images.size());

        for(size_t i = 0; i < images.size(); ++i) {
            bundle.frames[i].image = images[i];
            bundle.frames[i].imageView = vkUtils::createImageView(device, images[i], format.format);

            // Create buffers for UBO
            vkUtils::BufferInput uboBufferInput {};
            uboBufferInput.device = device;
            uboBufferInput.physicalDevice = physicalDevice;
            uboBufferInput.size = sizeof(vkUtils::UniformBufferObject);
            uboBufferInput.usageFlags = vk::BufferUsageFlagBits::eUniformBuffer;
            uboBufferInput.memoryProperties = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;
            bundle.frames[i].uniformBuffer = vkUtils::createBuffer(uboBufferInput);
        }

        bundle.format = format.format;
        bundle.extent = extent;

        LOG(LogLevel::Trace) << "Vulkan swapchain creation";
        return bundle;
    }



    void createFrameDescriptors(vector<vkUtils::SwapchainFrame>& frames, vk::Device device,
                                vk::DescriptorSetLayout descriptorSetLayout, vk::DescriptorPool descriptorPool,
                                const TextureGPU& texture) {

        // Allocate descriptor sets to the descriptor pool
        vector<vk::DescriptorSetLayout> descriptorSetLayouts { frames.size(), descriptorSetLayout };
        vk::DescriptorSetAllocateInfo allocateInfo {};
        allocateInfo.descriptorPool = descriptorPool;
        allocateInfo.descriptorSetCount = frames.size();
        allocateInfo.pSetLayouts = descriptorSetLayouts.data();

        for(auto& frame : frames) {
            // Create descriptors in order to put UBO in the shader
            auto allocateDescriptorSetsRes = device.allocateDescriptorSets(&allocateInfo, &frame.descriptorSet);
            GASSERT_MSG(allocateDescriptorSetsRes == vk::Result::eSuccess, "Vulkan could allocate descriptor sets");

            vk::DescriptorBufferInfo bufferInfo {};
            bufferInfo.buffer = frame.uniformBuffer.buffer;
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(vkUtils::UniformBufferObject);

            vk::DescriptorImageInfo imageInfo {};
            imageInfo.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
            imageInfo.imageView = texture.imageView;
            imageInfo.sampler = texture.sampler;

            array<vk::WriteDescriptorSet, 2> descriptorWrites {};
            // UBO write
            descriptorWrites[0].dstSet = frame.descriptorSet;
            descriptorWrites[0].dstBinding = 0;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].descriptorType = vk::DescriptorType::eUniformBuffer;
            descriptorWrites[0].pBufferInfo = &bufferInfo;
            // Sampler write
            descriptorWrites[1].dstSet = frame.descriptorSet;
            descriptorWrites[1].dstBinding = 1;
            descriptorWrites[1].dstArrayElement = 0;
            descriptorWrites[1].descriptorCount = 1;
            descriptorWrites[1].descriptorType = vk::DescriptorType::eCombinedImageSampler;
            descriptorWrites[1].pImageInfo = &imageInfo;

            device.updateDescriptorSets(descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
        }
    }
}

#endif //RENDER_VULKAN_SWAPCHAIN_H
