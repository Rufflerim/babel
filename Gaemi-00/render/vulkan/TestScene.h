//
// Created by gaetz on 10/10/2022.
//

#ifndef BABEL_TESTSCENE_H
#define BABEL_TESTSCENE_H

#include <glm/glm.hpp>
#include <vector>

using std::vector;

namespace engine::render::vulkan {


    class TestScene {
    public:
        TestScene();
        vector<glm::vec3> trianglePositions;
        vector<glm::vec3> squarePositions;
        vector<glm::vec3> starPositions;
    };

}

#endif //BABEL_TESTSCENE_H
