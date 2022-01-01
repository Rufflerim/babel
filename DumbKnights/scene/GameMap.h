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
        std::vector<Entity> entities;

    };
}

#endif //SCENE_GAMEMAP_H
