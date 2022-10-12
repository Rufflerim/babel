//
// Created by gaetz on 10/10/2022.
//

#include "TestScene.h"
#include "../../Defines.h"

using engine::render::vulkan::TestScene;

TestScene::TestScene() {
    float x = -0.6f;
    for (float y = -1.0f; y < 1.0f; y += 0.2f) {
        trianglePositions.emplace_back(x, y, 0.0f);
    }

    x = 0.0f;
    for (float y = -1.0f; y < 1.0f; y += 0.2f) {
        squarePositions.emplace_back(x, y, 0.0f);
    }

    x = 0.6f;
    for (float y = -1.0f; y < 1.0f; y += 0.2f) {
        starPositions.emplace_back(x, y, 0.0f);
    }
}