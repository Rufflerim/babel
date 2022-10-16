//
// Created by gaetz on 10/10/2022.
//

#ifndef BABEL_TESTSCENE_H
#define BABEL_TESTSCENE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "../../Defines.h"

using std::vector;

namespace engine::render::vulkan {


    class TestScene {
    public:
        TestScene();
        vector<glm::vec3> trianglePositions;
        vector<glm::vec3> squarePositions;
        vector<glm::vec3> starPositions;

        glm::mat4 view;
        glm::mat4 proj;
        //glm::mat4 proj { glm::ortho( 0.0f, (f32)WINDOW_WIDTH, 0.0f, (f32)WINDOW_HEIGHT) };

    };

}

#endif //BABEL_TESTSCENE_H
