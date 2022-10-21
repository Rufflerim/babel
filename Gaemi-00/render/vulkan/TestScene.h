//
// Created by gaetz on 10/10/2022.
//

#ifndef RENDER_VULKAN_TESTSCENE_H
#define RENDER_VULKAN_TESTSCENE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "../../Defines.h"
#include "TextureGPU.h"

using std::vector;

namespace engine::render::vulkan {


    class TestScene {
    public:
        TestScene();
        vector<glm::vec3> trianglePositions;
        vector<glm::vec3> squarePositions;
        vector<glm::vec3> squareScales;

        vector<glm::vec3> starPositions;

        glm::mat4 view;
        glm::mat4 proj;

        void clear();

    };

}

#endif //RENDER_VULKAN_TESTSCENE_H
