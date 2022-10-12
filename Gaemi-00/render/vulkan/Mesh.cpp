//
// Created by gaetz on 12/10/2022.
//

#include "Mesh.h"
#include <vector>

using std::vector;

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
        attributes[1].binding = 0;
        attributes[1].location = 1;
        attributes[1].format = vk::Format::eR32G32B32Sfloat; // R, G, B
        attributes[1].offset = 2 * sizeof(float);

        return attributes;
    }

    TriangleMesh::TriangleMesh(vk::Device deviceP, vk::PhysicalDevice physicalDevice)
            : device { deviceP } {
        vector<float> vertices {
            // Position         // Color
            0.0f, -0.05f, 0.0f, 1.0f, 0.0f,
            0.05f, 0.05f, 0.0f, 1.0f, 0.0f,
            -0.05f, 0.05f, 0.0f, 1.0f, 0.0f
        };

        vkUtils::BufferInput input;
        input.device = device;
        input.physicalDevice = physicalDevice;
        input.size = sizeof(float) * vertices.size();
        input.usageFlags = vk::BufferUsageFlagBits::eVertexBuffer;
        vertexBuffer = vkUtils::createBuffer(input);

        // Upload vertices to GPU
        void* memoryLocation = device.mapMemory(vertexBuffer.bufferMemory, 0, input.size);
        memcpy(memoryLocation, vertices.data(), input.size);
        device.unmapMemory(vertexBuffer.bufferMemory);
    }

    TriangleMesh::~TriangleMesh() {
        device.destroyBuffer(vertexBuffer.buffer);
        device.freeMemory(vertexBuffer.bufferMemory);
    }
}