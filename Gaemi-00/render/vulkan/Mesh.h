//
// Created by gaetz on 12/10/2022.
//

#ifndef RENDER_VULKAN_MESH_H
#define RENDER_VULKAN_MESH_H

#include <vulkan/vulkan.hpp>
#include <array>
#include "Memory.h"

using std::array;

namespace engine::render::vulkan::vkMesh {

    enum class GeometryType {
        Triangle,
        Square,
        Star
    };

    vk::VertexInputBindingDescription getPosColorBindingDescription() ;

    array<vk::VertexInputAttributeDescription, 3> getPosColorUVAttributeDescriptions();

    class TriangleMesh {
    public:
        TriangleMesh(vk::Device deviceP, vk::PhysicalDevice physicalDevice);
        ~TriangleMesh();
        vkUtils::Buffer vertexBuffer;
    private:
        vk::Device device;
    };

    class SquareMesh {
    public:
        SquareMesh(vk::Device deviceP, vk::PhysicalDevice physicalDevice);
        ~SquareMesh();
        vkUtils::Buffer vertexBuffer;
    private:
        vk::Device device;
    };

    class StarMesh {
    public:
        StarMesh(vk::Device deviceP, vk::PhysicalDevice physicalDevice);
        ~StarMesh();
        vkUtils::Buffer vertexBuffer;
    private:
        vk::Device device;
    };

}

#endif //RENDER_VULKAN_MESH_H
