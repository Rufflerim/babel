//
// Created by gaetz on 21/12/2021.
//

#ifndef ECS_ENTITYMANAGER_H
#define ECS_ENTITYMANAGER_H

#include "../Defines.h"
#include "../Asserts.h"
#include <array>
#include <queue>

using std::queue;
using std::array;

namespace engine::ecs {
    class EntityManager {
    public:
        /// Initialize all entities. MAX_ENTITIES number is set in Defines.h.
        EntityManager();

        /// Create an entity and returns its id
        /// \return ID of the new entity
        Entity createEntity();

        /// Destroy a given entity and invalidate its signature
        /// \param entity Entity ID
        void destroyEntity(Entity entity);

        /// Set a given signature to a given entity
        /// \param entity Entity we want to set the signature on
        /// \param signature Signature to set
        void setSignature(Entity entity, Signature signature) {
            GASSERT_DEBUG(entity < MAX_ENTITIES, "Entity ID is out of range");
            signatures[entity] = signature;
        }

        /// Get the signature of a given entity
        /// \param entity Entity we want the signature from
        /// \return The signature of an entity
        Signature getSignature(Entity entity) {
            GASSERT_DEBUG(entity < MAX_ENTITIES, "Entity ID is out of range");
            return signatures[entity];
        }

    private:
        /// Queue of unused entity IDs
        queue<Entity> availableEntities{};

        /// Array of signatures where the index corresponds to the entity ID
        array<Signature, MAX_ENTITIES> signatures{};

        /// Total living entities - used to keep limits on how many exist
        u32 livingEntityCount { 0 };
    };
}

#endif //GAEMI_00_ECS_ENTITYMANAGER_H
