//
// Created by gaetz on 24/12/2021.
//

#ifndef SCENE_GAMEMAP_H
#define SCENE_GAMEMAP_H

#include <SDL_render.h>
#include <render/IRenderer.h>
#include "IScene.h"
#include "../components/RenderingSystem.h"
#include "../Locator.h"
#include "../components/Move2DSystem.h"
#include "../components/ControllerSystem.h"
#include "../components/AnimationSystem.h"
#include "../world/TileMap.h"

namespace scene {
    class GameMap : public IScene {
    public:
        explicit GameMap(engine::ecs::Coordinator& coordinator);

        void onInit() override;

        void onClose() override;

        void activate() override;

        void inactivate() override;

        void update(const GameTime& time, const InputState& inputState) override;

        void draw(engine::render::IRenderer& renderer) override;

    private:
        std::shared_ptr<RenderingSystem> renderingSystem;
        std::shared_ptr<Move2DSystem> moveSystem;
        std::shared_ptr<ControllerSystem> controllerSystem;
        std::shared_ptr<AnimationSystem> animationSystem;

        std::vector<Entity> entities;
        world::TileMap tileMap { "map1"};
    };
}

#endif //SCENE_GAMEMAP_H
