//
// Created by gaetz on 24/12/2021.
//

#ifndef SCENE_GAMEPAUSE_H
#define SCENE_GAMEPAUSE_H

#include "IScene.h"

namespace scene {
    class GamePause : public IScene {
    public:
        explicit GamePause(engine::ecs::Coordinator& coordinator);

        void onInit() override;

        void onClose() override;

        void activate() override;

        void inactivate() override;

        void update(GameTime time) override;

        void draw(SDL_Renderer *pRenderer) override;
    };
}

#endif //SCENE_GAMEPAUSE_H
