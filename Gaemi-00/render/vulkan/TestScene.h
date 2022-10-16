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

        glm::mat4 view { glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 0.0f, 1.0f)) };
        glm::mat4 proj { glm::perspective( glm::radians(45.0f), static_cast<f32>(WINDOW_WIDTH / WINDOW_HEIGHT), 0.1f, 1000.0f) };
        //glm::mat4 proj { glm::ortho( 0.0f, (f32)WINDOW_WIDTH, 0.0f, (f32)WINDOW_HEIGHT) };

    };

}

#endif //BABEL_TESTSCENE_H
