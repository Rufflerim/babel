//
// Created by gaetz on 12/10/2022.
//

#ifndef RENDER_VULKAN_MESH_H
#define RENDER_VULKAN_MESH_H

#include <vulkan/vulkan.hpp>
#include <array>

using std::array;

namespace engine::render::vulkan::vkMesh {

    vk::VertexInputBindingDescription getPosColorBindingDescription() {
        vk::VertexInputBindingDescription bindingDescription;
        bindingDescription.binding = 0;
        bindingDescription.stride = 5 * sizeof(float);
        bindingDescription.inputRate = vk::VertexInputRate::eVertex;

        return bindingDescription;
    }

    array<vk::VertexInputAttributeDescription, 2> getPosColorAttributeDescriptions() {
        array<vk::VertexInputAttributeDescription, 2> attributes;
        // Position
        attributes[0].binding = 0;
        attributes[0].location = 0;
        attributes[0].format = vk::Format::eR32G32Sfloat; // X, Y
        attributes[0].offset = 0;
        // Color
        attributes[0].binding = 0;
        attributes[0].location = 1;
        attributes[0].format = vk::Format::eR32G32B32Sfloat; // R, G, B
        attributes[0].offset = 0;
        
        return attributes;
    }


}

#endif //RENDER_VULKAN_MESH_H
