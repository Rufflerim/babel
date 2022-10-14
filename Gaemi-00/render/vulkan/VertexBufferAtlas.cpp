//
// Created by gaetz on 14/10/2022.
//

#include "VertexBufferAtlas.h"

using engine::render::vulkan::vkMesh::VertexBufferAtlas;
using engine::render::vulkan::vkMesh::GeometryType;
using engine::render::vulkan::vkMesh::FinalizationInput;

VertexBufferAtlas::~VertexBufferAtlas() {
    device.destroyBuffer(vertexBuffer.buffer);
    device.freeMemory(vertexBuffer.bufferMemory);
}

void VertexBufferAtlas::consume(GeometryType geometryType, vector<float>& vertexData) {
    lump.reserve(vertexData.size());
    for (float attribute : vertexData) {
        lump.push_back(attribute);
    }
    i32 vertexCount = vertexData.size() / 5; // 5 numbers per vertex : X, Y, R, G, B
    offsets.insert(std::make_pair(geometryType, offset));
    sizes.insert(std::make_pair(geometryType, vertexCount));
    offset += vertexCount;
}

void VertexBufferAtlas::finalize(FinalizationInput finalizationInput) {
    device = finalizationInput.device;

    vkUtils::BufferInput input {};
    input.device = device;
    input.physicalDevice = finalizationInput.physicalDevice;
    input.size = sizeof(float) * lump.size();
    input.usageFlags = vk::BufferUsageFlagBits::eTransferSrc;
    input.memoryProperties = vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent;

    vkUtils::Buffer stagingBuffer = vkUtils::createBuffer(input);

    // Load data on staging buffer
    void* memoryLocation = device.mapMemory(stagingBuffer.bufferMemory, 0, input.size);
    memcpy(memoryLocation, lump.data(), input.size);
    device.unmapMemory(stagingBuffer.bufferMemory);

    // Copy from staging to vertex buffer
    input.usageFlags = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer;
    input.memoryProperties = vk::MemoryPropertyFlagBits::eDeviceLocal;
    vertexBuffer = vkUtils::createBuffer(input);
    vkUtils::copyBuffer(stagingBuffer, vertexBuffer,input.size,
                        finalizationInput.queue, finalizationInput.commandBuffer);

    // Clear staging buffer
    device.destroyBuffer(stagingBuffer.buffer);
    device.freeMemory(stagingBuffer.bufferMemory);
}
