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

    /// Contains a stack of scenes in an ordered and coherent map
    using SceneContainer = vector<pair<SceneType, IScene*>>;

    /// Store prepared functions to generate scenes
    using SceneFactory = unordered_map<SceneType, std::function<IScene*(void)>>;

    class SceneManager {
    public:
        SceneManager();
        ~SceneManager();
        void update(GameTime time);
        void draw();

        void processRequests();
        bool hasScene(SceneType type) const;
        void switchTo(SceneType type);
        void remove(SceneType type);

    private:
        template<class T>
        void registerScene(SceneType type) {
            sceneFactory[type] = []() {
                return new T();
            };
        }

        void createScene(SceneType type);
        void removeScene(SceneType type);

        SceneContainer scenes {};
        vector<SceneType> scenesToRemove {};
        SceneFactory sceneFactory {};
    };

}
#endif //SCENE_SCENEMANAGER_H
