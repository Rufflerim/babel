//
// Created by gaetz on 23/12/2021.
//

#include "SystemManager.h"

void engine::ecs::SystemManager::onEntityDestroyed(Entity entity) {
    for (auto const& pair : systems)
    {
        auto const& system = pair.second;
        // entities is a set so no check needed.
        system->entities.erase(entity);
    }
}

void engine::ecs::SystemManager::onEntitySignatureChanged(Entity entity, Signature entitySignature) {
    for (auto const& pair : systems) {
        auto const& type = pair.first;
        auto const& system = pair.second;
        auto const& systemSignature = signatures[type];

        // Entity signature matches system signature - insert into set
        if ((entitySignature & systemSignature) == systemSignature) {
            system->entities.insert(entity);
        }
        // Entity signature does not match system signature - erase from set
        else {
            system->entities.erase(entity);
        }
    }
}

