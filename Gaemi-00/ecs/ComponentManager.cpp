//
// Created by gaetz on 21/12/2021.
//

#include "ComponentManager.h"

void engine::ecs::ComponentManager::onEntityDestroyed(Entity entity) {
    for (auto const& pair : componentArrays) {
        auto const& component = pair.second;
        component->onEntityDestroyed(entity);
    }
}
