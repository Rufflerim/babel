//
// Created by gaetz on 25/12/2021.
//

#ifndef COMPONENTS_RENDERINGSYSTEM_H
#define COMPONENTS_RENDERINGSYSTEM_H

#include <System.h>
#include <Coordinator.h>
#include <SDL_render.h>
#include <render/IRenderer.h>

class RenderingSystem : public engine::ecs::System {
public:
    void draw(engine::ecs::Coordinator &coordinator, engine::render::IRenderer& renderer);
};


#endif //COMPONENTS_RENDERINGSYSTEM_H
