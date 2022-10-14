//
// Created by gaetz on 14/10/2022.
//

#ifndef RENDER_VULKAN_VERTEXBUFFERATLAS_H
#define RENDER_VULKAN_VERTEXBUFFERATLAS_H

#include <vector>
#include <unordered_map>

#include "Mesh.h"
#include "Memory.h"

using std::vector;
using std::unordered_map;

namespace engine::render::vulkan::vkMesh {

    struct FinalizationInput {
        vk::Device device;
        vk::PhysicalDevice physicalDevice;
        vk::Queue queue;
        vk::CommandBuffer commandBuffer;
    };

    class VertexBufferAtlas {
    public:
        VertexBufferAtlas() = default;
        ~VertexBufferAtlas();

        i32 getFirstVertex(GeometryType geometryType) const {
            return offsets.find(geometryType)->second;
        }

        i32 getVertexCount(GeometryType geometryType) const {
            return sizes.find(geometryType)->second;
        }

        void consume(GeometryType geometryType, vector<float>& vertexData);
        void finalize(FinalizationInput finalizationInput);
        void close();

        vkUtils::Buffer vertexBuffer;
        unordered_map<GeometryType, int> offsets;
        unordered_map<GeometryType, int> sizes;

    private:

        /** How many points were sent */
        i32 offset { 0 };
        vk::Device device;

        vector<float> lump;
    };

}

#endif //RENDER_VULKAN_VERTEXBUFFERATLAS_H
