//
// Created by gaetz on 21/12/2021.
//

#include "EntityManager.h"

using engine::ecs::EntityManager;

EntityManager::EntityManager() {
    for (Entity entity = 0; entity < MAX_ENTITIES; ++entity) {
        availableEntities.push(entity);
    }
}

Entity EntityManager::createEntity() {
    GASSERT_DEBUG(livingEntityCount < MAX_ENTITIES, "Too many entities exist")
    Entity id = availableEntities.front();
    availableEntities.pop();
    ++livingEntityCount;
    return id;
}

void EntityManager::destroyEntity(Entity entity) {
    GASSERT_DEBUG(entity < MAX_ENTITIES, "Entity out of range")
    signatures[entity].reset();
    availableEntities.push(entity);
    --livingEntityCount;
}

