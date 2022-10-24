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
        bindingDescription.stride = 7 * sizeof(float);
        bindingDescription.inputRate = vk::VertexInputRate::eVertex;

        return bindingDescription;
    }

    array<vk::VertexInputAttributeDescription, 3> getPosColorUVAttributeDescriptions() {
        array<vk::VertexInputAttributeDescription, 3> attributes;
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
        // Texture coords
        attributes[2].binding = 0;
        attributes[2].location = 2;
        attributes[2].format = vk::Format::eR32G32Sfloat; // U, V
        attributes[2].offset = 5 * sizeof(float);

        return attributes;
    }

    TriangleMesh::TriangleMesh(vk::Device deviceP, vk::PhysicalDevice physicalDevice)
            : device { deviceP } {
        vector<float> vertices {{
            // Position     // Color            // UV
            0.0f, -0.5f,    0.0f, 1.0f, 0.0f,   0.5f, 0.0f,
            0.5f, 0.5f,     0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
            -0.5f, 0.5f,    0.0f, 1.0f, 0.0f,   0.0f, 1.0f
        }};

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

    SquareMesh::SquareMesh(vk::Device deviceP, vk::PhysicalDevice physicalDevice) : device {deviceP} {
        std::vector<float> vertices = { {
                                                -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
                                                -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
                                                0.5f, -0.5f,    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
                                                0.5f, -0.5f,    1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
                                                0.5f,  0.5f,    1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
                                                -0.5f, 0.5f,    1.0f, 0.0f, 0.0f,   0.0f, 1.0f
                                        } };

        vkUtils::BufferInput input;
        input.device = device;
        input.physicalDevice = physicalDevice;
        input.size = sizeof(float) * vertices.size();
        input.usageFlags = vk::BufferUsageFlagBits::eVertexBuffer;

        vertexBuffer = vkUtils::createBuffer(input);

        void* memoryLocation = device.mapMemory(vertexBuffer.bufferMemory, 0, input.size);
        memcpy(memoryLocation, vertices.data(), input.size);
        device.unmapMemory(vertexBuffer.bufferMemory);
    }

    SquareMesh::~SquareMesh() {
        device.destroyBuffer(vertexBuffer.buffer);
        device.freeMemory(vertexBuffer.bufferMemory);
    }

    StarMesh::StarMesh(vk::Device deviceP, vk::PhysicalDevice physicalDevice) : device {deviceP} {
        std::vector<float> vertices = { {
                                                -0.05f, -0.025f, 0.0f, 0.0f, 1.0f,
                                                -0.02f, -0.025f, 0.0f, 0.0f, 1.0f,
                                                -0.03f,    0.0f, 0.0f, 0.0f, 1.0f,
                                                -0.02f, -0.025f, 0.0f, 0.0f, 1.0f,
                                                0.0f,  -0.05f, 0.0f, 0.0f, 1.0f,
                                                0.02f, -0.025f, 0.0f, 0.0f, 1.0f,
                                                -0.03f,    0.0f, 0.0f, 0.0f, 1.0f,
                                                -0.02f, -0.025f, 0.0f, 0.0f, 1.0f,
                                                0.02f, -0.025f, 0.0f, 0.0f, 1.0f,
                                                0.02f, -0.025f, 0.0f, 0.0f, 1.0f,
                                                0.05f, -0.025f, 0.0f, 0.0f, 1.0f,
                                                0.03f,    0.0f, 0.0f, 0.0f, 1.0f,
                                                -0.03f,    0.0f, 0.0f, 0.0f, 1.0f,
                                                0.02f, -0.025f, 0.0f, 0.0f, 1.0f,
                                                0.03f,    0.0f, 0.0f, 0.0f, 1.0f,
                                                0.03f,    0.0f, 0.0f, 0.0f, 1.0f,
                                                0.04f,   0.05f, 0.0f, 0.0f, 1.0f,
                                                0.0f,   0.01f, 0.0f, 0.0f, 1.0f,
                                                -0.03f,    0.0f, 0.0f, 0.0f, 1.0f,
                                                0.03f,    0.0f, 0.0f, 0.0f, 1.0f,
                                                0.0f,   0.01f, 0.0f, 0.0f, 1.0f,
                                                -0.03f,    0.0f, 0.0f, 0.0f, 1.0f,
                                                0.0f,   0.01f, 0.0f, 0.0f, 1.0f,
                                                -0.04f,   0.05f, 0.0f, 0.0f, 1.0f
                                        } };

        vkUtils::BufferInput inputChunk;
        inputChunk.device = device;
        inputChunk.physicalDevice = physicalDevice;
        inputChunk.size = sizeof(float) * vertices.size();
        inputChunk.usageFlags = vk::BufferUsageFlagBits::eVertexBuffer;

        vertexBuffer = vkUtils::createBuffer(inputChunk);

        void* memoryLocation = device.mapMemory(vertexBuffer.bufferMemory, 0, inputChunk.size);
        memcpy(memoryLocation, vertices.data(), inputChunk.size);
        device.unmapMemory(vertexBuffer.bufferMemory);
    }

    StarMesh::~StarMesh() {
        device.destroyBuffer(vertexBuffer.buffer);
        device.freeMemory(vertexBuffer.bufferMemory);
    }
}