//
// Created by gaetz on 31/12/2021.
//

#include <Coordinator.h>
#include <Functions.h>
#include "Move2DSystem.h"
#include "Components.h"

void Move2DSystem::update(engine::ecs::Coordinator& coordinator, const GameTime& time) {

    auto transformComps = coordinator.getComponentArray<Transform2D>();
    auto moveComps = coordinator.getComponentArray<Move2D>();

    for(auto entity : entities) {
        auto& transform = transformComps->getData(entity);
        auto& move = moveComps->getData(entity);

        if(move.acceleration == gmath::Vec2::zero() && move.speed == gmath::Vec2::zero()) return;
        move.speed += move.acceleration * time.dt;
        // Clamp speed to max speed
        f32 maxSpeedInverseProportion = move.maxSpeed / move.speed.length();
        f32 speedMultiplier = gmath::min(1.0f, maxSpeedInverseProportion);
        move.speed *= speedMultiplier;
        // Translation
        transform.position += move.speed * time.dt;
        // Deceleration
        move.speed *= move.decelerationFactor;
    }
}
