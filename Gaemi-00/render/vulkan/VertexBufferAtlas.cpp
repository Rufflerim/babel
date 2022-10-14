//
// Created by gaetz on 14/10/2022.
//

#include "VertexBufferAtlas.h"

using engine::render::vulkan::vkMesh::VertexBufferAtlas;
using engine::render::vulkan::vkMesh::GeometryType;

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

void VertexBufferAtlas::finalize(vk::Device device, vk::PhysicalDevice physicalDevice) {
    vkUtils::BufferInput input {};
    input.device = device;
    input.physicalDevice = physicalDevice;
    input.size = sizeof(float) * lump.size();
    input.usageFlags = vk::BufferUsageFlagBits::eVertexBuffer;
    vertexBuffer = vkUtils::createBuffer(input);

    void* memoryLocation = device.mapMemory(vertexBuffer.bufferMemory, 0, input.size);
    memcpy(memoryLocation, lump.data(), input.size);
    device.unmapMemory(vertexBuffer.bufferMemory);
}

void VertexBufferAtlas::close() {
    //device.destroyBuffer(vertexBuffer.buffer);
    //device.freeMemory(vertexBuffer.bufferMemory);
}
