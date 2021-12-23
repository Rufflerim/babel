//
// Created by gaetz on 23/12/2021.
//

#include "Coordinator.h"

void engine::ecs::Coordinator::init() {
    // Create pointers to each manager
    componentManager = std::make_unique<ComponentManager>();
    entityManager = std::make_unique<EntityManager>();
    systemManager = std::make_unique<SystemManager>();
}

Entity engine::ecs::Coordinator::createEntity() {
    return entityManager->createEntity();
}

void engine::ecs::Coordinator::destroyEntity(Entity entity) {
    entityManager->onDestroyEntity(entity);
    componentManager->onEntityDestroyed(entity);
    systemManager->onEntityDestroyed(entity);
}

