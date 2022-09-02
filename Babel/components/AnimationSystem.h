//
// Created by gaetz on 25/12/2021.
//

#ifndef COMPONENTS_ANIMATIONSYSTEM_H
#define COMPONENTS_ANIMATIONSYSTEM_H

#include <System.h>
#include <Coordinator.h>
#include <SDL_render.h>
#include <render/IRenderer.h>

class AnimationSystem : public engine::ecs::System {
public:
    void update(engine::ecs::Coordinator& coordinator, const GameTime& time);
};


#endif //COMPONENTS_ANIMATIONSYSTEM_H
