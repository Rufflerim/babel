//
// Created by gaetz on 24/12/2021.
//

#include <algorithm>
#include <Log.h>
#include "VectorUtils.h"
#include "SceneManager.h"
#include "GamePause.h"
#include "GameMap.h"

using scene::SceneType;

scene::SceneManager::SceneManager() {
    // Reserve scene storage
    scenes.reserve(MAX_SCENE_DEPTH);
    sceneTypes.reserve(MAX_SCENE_DEPTH);
    // Register scene types
    registerScene<GameMap>(SceneType::GameMap);
    registerScene<GamePause>(SceneType::GamePause);


    switchTo(SceneType::GameMap);
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
    while(begin(scenesToRemove) != end(scenesToRemove)) {
        removeScene(scenesToRemove.front());
        scenesToRemove.erase(begin(scenesToRemove));
    }
}

bool scene::SceneManager::hasScene(SceneType type) const {
    auto found = std::find(begin(sceneTypes), end(sceneTypes), type);
    if(found != end(sceneTypes)) {
        auto foundInRemoved = std::find(begin(scenesToRemove), end(scenesToRemove), type);
        if(foundInRemoved == end(scenesToRemove)) {
            return true;
        }
    }
    return false;
}

void scene::SceneManager::switchTo(SceneType type) {
    currentScene = type;
    u32 sceneIndex = VectorUtils::getIndex<SceneType>(type, sceneTypes);
    if (sceneIndex != -1) {
        // If scene already exists, put it on top of the scene stack
        scenes.back()->inactivate();
        VectorUtils::moveToBack(sceneIndex, scenes);
        VectorUtils::moveToBack(sceneIndex, sceneTypes);
        scenes.back()->activate();
    } else {
        // Else add a new scene on the stack
        if(!scenes.empty()) {
            scenes.back()->inactivate();
        }
        createScene(type);
        scenes.back()->activate();
    }
}

void scene::SceneManager::remove(SceneType type) {
    scenesToRemove.push_back(type);
}

void scene::SceneManager::createScene(SceneType type) {
    auto newSceneFactory = sceneFactories.find(type);
    if(newSceneFactory == end(sceneFactories)) {
        LOG(engine::LogLevel::Warning) << "Scene type not registered. Won't create new scene.";
        return;
    }
    scenes.push_back(std::move(newSceneFactory->second()));
    sceneTypes.push_back(type);
}

void scene::SceneManager::removeScene(SceneType type) {
    u32 sceneIndex = VectorUtils::getIndex<SceneType>(type, sceneTypes);
    if(sceneIndex == -1) return;
    // Execute scene onClose then remove scene and sceneType
    auto sceneItr = begin(scenes) + sceneIndex;
    auto typeItr = begin(sceneTypes) + sceneIndex;
    scenes[sceneIndex]->onClose();
    scenes.erase(sceneItr);
    sceneTypes.erase(typeItr);
}
