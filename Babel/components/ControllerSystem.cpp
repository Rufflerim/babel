//
// Created by gaetz on 01/01/2022.
//

#include "ControllerSystem.h"
#include "Components.h"

void ControllerSystem::update(engine::ecs::Coordinator& coordinator, const engine::input::InputState& inputState) {

    auto controllerComps = coordinator.getComponentArray<Controller>();
    auto moveComps = coordinator.getComponentArray<Move2D>();

    for (auto entity: entities) {
        auto& controller = controllerComps->getData(entity);
        if (!controller.isPlayer) return;

        auto& move = moveComps->getData(entity);
        controller.inputAxis = Vec2::zero();
        if (inputState.keyboard.getKeyState(SDL_SCANCODE_D) == engine::input::ButtonState::Held) {
            controller.inputAxis.x = 1;
        } else if (inputState.keyboard.getKeyState(SDL_SCANCODE_A) == engine::input::ButtonState::Held) {
            controller.inputAxis.x = -1;
        }
        if (inputState.keyboard.getKeyState(SDL_SCANCODE_W) == engine::input::ButtonState::Held) {
            controller.inputAxis.y = 1;
        } else if (inputState.keyboard.getKeyState(SDL_SCANCODE_S) == engine::input::ButtonState::Held) {
            controller.inputAxis.y = -1;
        }
        // Vertical should be inversed in 2d
        controller.inputAxis.y *= -1;

        controller.inputAxis.normalize();
        move.acceleration = controller.inputAxis * 500.f;
    }


}
