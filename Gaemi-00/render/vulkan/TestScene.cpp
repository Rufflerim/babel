//
// Created by gaetz on 10/10/2022.
//

#include "TestScene.h"

using engine::render::vulkan::TestScene;

TestScene::TestScene() {

    //trianglePositions.emplace_back(0.0f, 0.0f, 0.0f);

    /*
    squarePositions.emplace_back(0, 0, 0.0f);
     */
    /*
    float x = 0.6f;
    for (float y = -1.0f; y < 1.0f; y += 0.2f) {
        starPositions.emplace_back(x, y, 0.0f);
    }
     */

    //view = glm::mat4(glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    view = glm::mat4(1.0f);
    proj = glm::ortho(0.0f, 1280.0f, 0.0f, 800.0f, -10.0f, 10.0f);
    //proj[1][1] *= -1;
    //proj = glm::mat4(glm::perspective( glm::radians(70.0f), static_cast<f32>(WINDOW_WIDTH / WINDOW_HEIGHT), 0.1f, 20.0f));


}

void TestScene::clear() {
    //trianglePositions.clear();
    squarePositions.clear();
    squareScales.clear();
    //starPositions.clear();
}