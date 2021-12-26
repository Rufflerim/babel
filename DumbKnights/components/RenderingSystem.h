//
// Created by gaetz on 25/12/2021.
//

#ifndef COMPONENTS_RENDERINGSYSTEM_H
#define COMPONENTS_RENDERINGSYSTEM_H

#include <System.h>
#include <Coordinator.h>
#include <SDL_render.h>
#include "Components.h"

class RenderingSystem : public engine::ecs::System {
public:
    void draw(engine::ecs::Coordinator &coordinator, SDL_Renderer *pRenderer);
};


#endif //COMPONENTS_RENDERINGSYSTEM_H
