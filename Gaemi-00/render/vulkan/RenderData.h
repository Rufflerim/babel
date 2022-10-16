//
// Created by gaetz on 10/10/2022.
//

#ifndef RENDER_VULKAN_RENDERDATA_H
#define RENDER_VULKAN_RENDERDATA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace engine::render::vulkan::vkUtils {

    struct ObjectData {
        glm::mat4 model;
    };

    struct UniformBufferObject {
        glm::mat4 view;
        glm::mat4 proj;
    };

}

#endif //RENDER_VULKAN_RENDERDATA_H
