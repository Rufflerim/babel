//
// Created by gaetz on 10/10/2022.
//

#include "TestScene.h"
#include "../../Defines.h"

using engine::render::vulkan::TestScene;

TestScene::TestScene() {

    trianglePositions.emplace_back(-1.0, 0, -5.0f);

    squarePositions.emplace_back(0, 0, -5.0f);

    float x = 0.6f;
    for (float y = -1.0f; y < 1.0f; y += 0.2f) {
        starPositions.emplace_back(x, y, -5.0f);
    }
}