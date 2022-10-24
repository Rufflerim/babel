//
// Created by gaetz on 09/10/2022.
//

#ifndef RENDER_VULKAN_QUEUEFAMILIES_H
#define RENDER_VULKAN_QUEUEFAMILIES_H

#include <optional>
using std::optional;
using std::vector;

namespace engine::render::vulkan::vkUtils {

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
            if (physicalDevice.getSurfaceSupportKHR(i, surface).value) {
                indices.presentFamily = i;
            }
            if (indices.haveAll()) break;
            ++i;
        }
        return indices;
    }
}

#endif //RENDER_VULKAN_QUEUEFAMILIES_H
