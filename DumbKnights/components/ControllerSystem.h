//
// Created by gaetz on 01/01/2022.
//

#ifndef COMPONENTS_CONTROLLERSYSTEM_H
#define COMPONENTS_CONTROLLERSYSTEM_H


#include <System.h>
#include <Coordinator.h>
#include <InputManager.h>

class ControllerSystem : public engine::ecs::System {
public:
    void update(engine::ecs::Coordinator& coordinator, const engine::input::InputState& inputState);
};


#endif //COMPONENTS_CONTROLLERSYSTEM_H
