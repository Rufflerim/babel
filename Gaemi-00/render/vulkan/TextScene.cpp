//
// Created by gaetz on 10/10/2022.
//

#include "TestScene.h"
#include "../../Defines.h"

using engine::render::vulkan::TestScene;

TestScene::TestScene() {
    for (f32 x = -1.0f; x < 1.0f; x += 0.2f) {
        for (f32 y = -1.0f; y < 1.0f; y += 0.2f) {
            trianglePositions.emplace_back(x, y, 0.0f);
        }
    }
}