//
// Created by gaetz on 24/12/2021.
//

#ifndef SCENE_GAMEMAP_H
#define SCENE_GAMEMAP_H

#include "IScene.h"

namespace scene {
    class GameMap : public IScene {
    public:
        void onInit() override;

        void onClose() override;

        void activate() override;

        void inactivate() override;

        void update(GameTime time) override;

        void draw() override;

        u32 count;
    };
}

#endif //SCENE_GAMEMAP_H
