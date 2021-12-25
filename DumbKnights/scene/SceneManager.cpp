//
// Created by gaetz on 24/12/2021.
//

#include <algorithm>
#include "SceneManager.h"
#include "GamePause.h"
#include "GameMap.h"

scene::SceneManager::SceneManager() {
    // Reserve scene storage
    scenes.reserve(MAX_SCENE_DEPTH);
    sceneTypes.reserve(MAX_SCENE_DEPTH);
    // Register scene types
    registerScene<GameMap>(SceneType::GameMap);
    registerScene<GamePause>(SceneType::GamePause);



    scenes.push_back(std::move(sceneFactory[SceneType::GameMap]()));
    sceneTypes.push_back(SceneType::GameMap);
}

scene::SceneManager::~SceneManager() {

}

void scene::SceneManager::update(GameTime time) {
    // Look for the last non locking scene in scenes' stack
    auto nonLockingItr = std::find_if(rbegin(scenes), rend(scenes),
                                      [](auto& scene) { return !scene->isNonLocking(); });
    // Convert to forward iterator on the same element
    auto forwardItr = --(nonLockingItr.base());
    // Draw non locking scenes with the right order
    for(; forwardItr != end(scenes); ++forwardItr) {
        (*forwardItr)->update(time);
    }
}

void scene::SceneManager::draw() {
    // Look for the last transparent scene in scenes' stack
    auto transparentItr = std::find_if(rbegin(scenes), rend(scenes),
                                       [](auto& scene) { return !scene->isTransparent(); });
    // Convert to forward iterator on the same element
    auto forwardItr = --(transparentItr.base());
    // Draw transparent scenes with the right order
    for(; forwardItr != end(scenes); ++forwardItr) {
        (*forwardItr)->draw();
    }
}

void scene::SceneManager::close() {
    for(auto& scene : scenes) {
        scene->onClose();
    }
}

void scene::SceneManager::processRequests() {

}

bool scene::SceneManager::hasScene(scene::SceneType type) const {
    return false;
}

void scene::SceneManager::switchTo(scene::SceneType type) {

}

void scene::SceneManager::remove(scene::SceneType type) {

}

void scene::SceneManager::createScene(scene::SceneType type) {

}

void scene::SceneManager::removeScene(scene::SceneType type) {

}

