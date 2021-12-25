//
// Created by gaetz on 24/12/2021.
//

#ifndef SCENE_SCENEMANAGER_H
#define SCENE_SCENEMANAGER_H

#include "SceneType.h"
#include <IScene.h>
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>

using std::vector;
using std::pair;
using std::unordered_map;
using std::unique_ptr;

namespace scene {

    /// Store prepared functions to generate scenes
    using SceneFactory = unordered_map<SceneType, std::function<unique_ptr<IScene>(void)>>;

    /// Max depth in the scene stack
    constexpr i32 MAX_SCENE_DEPTH = 10;

    /// Manage scene execution and transition.
    ///
    class SceneManager {
    public:
        SceneManager();
        ~SceneManager() = default;

        void update(GameTime time);
        void draw();
        void close();

        void processRequests();
        bool hasScene(SceneType type) const;
        void switchTo(SceneType type);
        void remove(SceneType type);

        SceneType currentScene { SceneType::GameMap };

    private:
        template<class T>
        void registerScene(SceneType type) {
            sceneFactories[type] = []() {
                return std::make_unique<T>();
            };
        }

        void createScene(SceneType type);
        void removeScene(SceneType type);

        vector<unique_ptr<IScene>> scenes;
        vector<SceneType> sceneTypes;
        vector<SceneType> scenesToRemove;
        SceneFactory sceneFactories;
    };

}
#endif //SCENE_SCENEMANAGER_H
