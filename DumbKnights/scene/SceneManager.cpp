//
// Created by gaetz on 24/12/2021.
//

#include <algorithm>
#include "SceneManager.h"
#include "GamePause.h"
#include "GameMap.h"

scene::SceneManager::SceneManager() {
    registerScene<GameMap>(SceneType::GameMap);
    registerScene<GamePause>(SceneType::GamePause);
    scenes.emplace_back(SceneType::GameMap, sceneFactory[SceneType::GameMap]());
}

scene::SceneManager::~SceneManager() {
    for(auto& scene : scenes) {
        scene.second->onClose();
    }
    for(auto& scene : scenes) {
        delete scene.second;
    }
}

void scene::SceneManager::update(GameTime time) {
    // Look for the last non locking scene in scenes' stack
    auto nonLockingItr = std::find_if(rbegin(scenes), rend(scenes),
                                      [](auto& scene) { return !scene.second->isNonLocking(); });
    // Convert to forward iterator on the same element
    auto forwardItr = --(nonLockingItr.base());
    // Draw non locking scenes with the right order
    for(; forwardItr != end(scenes); ++forwardItr) {
        forwardItr->second->update(time);
    }
}

void scene::SceneManager::draw() {
    // Look for the last transparent scene in scenes' stack
    auto transparentItr = std::find_if(rbegin(scenes), rend(scenes),
                                       [](auto& scene) { return !scene.second->isTransparent(); });
    // Convert to forward iterator on the same element
    auto forwardItr = --(transparentItr.base());
    // Draw transparent scenes with the right order
    for(; forwardItr != end(scenes); ++forwardItr) {
        forwardItr->second->draw();
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
