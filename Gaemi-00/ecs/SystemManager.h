//
// Created by gaetz on 23/12/2021.
//

#ifndef ECS_SYSTEMMANAGER_H
#define ECS_SYSTEMMANAGER_H

#include <set>
#include <memory>
#include <unordered_map>
#include "../Defines.h"
#include "../Asserts.h"
#include "System.h"

namespace engine { namespace ecs {
    class SystemManager {
    public:
        /// Create a pointer to the system and return it so it can be used externally
        /// \tparam T System type
        /// \return Shared pointer to the system
        template<typename T>
        std::shared_ptr<T> registerSystem() {
            const char* const typeName = typeid(T).name();
            GASSERT_DEBUG(systems.find(typeName) == systems.end(), "Registering system more than once.")

            auto system = std::make_shared<T>();
            systems.insert({typeName, system});
            return system;
        }

        /// Set the signature for this system
        /// \tparam T System type
        /// \param signature Signature associated with this system
        template<typename T>
        void setSignature(Signature signature) {
            const char* typeName = typeid(T).name();
            GASSERT_DEBUG(systems.find(typeName) != systems.end(), "System used before registered.")

            signatures.insert({typeName, signature});
        }

        /// Erase a destroyed entity from all system lists.
        /// \param entity Entity we want to erase from the systems
        void onEntityDestroyed(Entity entity);

        /// Notify each system that an entity's signature changed
        /// \param entity Entity which changed signature
        /// \param entitySignature New entity's signature
        void onEntitySignatureChanged(Entity entity, Signature entitySignature);

    private:
        /// Map from system type string pointer to a signature
        std::unordered_map<const char*, Signature> signatures {};

        /// Map from system type string pointer to a system pointer
        std::unordered_map<const char*, std::shared_ptr<System>> systems {};
    };
} }


#endif //ECS_SYSTEMMANAGER_H
