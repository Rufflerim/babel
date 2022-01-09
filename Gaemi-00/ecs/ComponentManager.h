//
// Created by gaetz on 21/12/2021.
//

#ifndef ECS_COMPONENTMANAGER_H
#define ECS_COMPONENTMANAGER_H

#include <unordered_map>
#include <memory>
#include "ComponentArray.h"

using std::unordered_map;

namespace engine { namespace ecs {
    class ComponentManager {
    public:
        /// Register component for later use.
        /// \tparam T Component class
        template<typename T>
        void registerComponent() {
            const char* typeName = typeid(T).name();
            GASSERT_DEBUG(componentTypes.find(typeName) == componentTypes.end(), "Registering component type more than once.")

            // Add this component type to the component type map
            componentTypes.insert({typeName, nextComponentType});

            // Create a ComponentArray pointer and add it to the component arrays map
            componentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});

            // Increment the value so that the next component registered will be different
            ++nextComponentType;
        }

        /// Return this component's type - used for creating signatures
        /// \tparam T Component class
        /// \return Component type
        template<typename T>
        ComponentType getComponentType() {
            const char* typeName = typeid(T).name();
            GASSERT_DEBUG(componentTypes.find(typeName) != componentTypes.end(), "Component not registered before use.")

            return componentTypes[typeName];
        }

        /// Add a component to the array for a given entity
        /// \tparam T Component class
        /// \param entity Entity we want to add the component to
        /// \param component Component we want to add
        template<typename T>
        void addComponent(Entity entity, T component) {
            getComponentArray<T>()->insertComponentData(entity, component);
        }

        /// Remove a component from the array for a given entity
        /// \tparam T Component class
        /// \param entity Entity we want to remove the component from
        template<typename T>
        void removeComponent(Entity entity) {
            getComponentArray<T>()->removeComponentData(entity);
        }

        /// Get a reference to a component from the array for a given entity
        /// BEWARE: this costs more than call the component array then call getData on each component.
        /// \tparam T Component class
        /// \param entity The entity from which we want the component
        /// \return Component from the entity
        template<typename T>
        T& getComponent(Entity entity) {
            return getComponentArray<T>()->getData(entity);
        }

        /// Notify each component array that an entity has been destroyed
        /// If it has a component for that entity, it will remove it
        /// \param entity
        void onEntityDestroyed(Entity entity);

        /// Convenience function to get the statically casted pointer to the ComponentArray of type T.
        /// \tparam T Component class
        /// \return Shared pointer to a component array
        template<typename T>
        std::shared_ptr<ComponentArray<T>> getComponentArray() {
            const char* typeName = typeid(T).name();
            return std::static_pointer_cast<ComponentArray<T>>(componentArrays.at(typeName));
        }

    private:
        // Map from type string pointer to a component type
        unordered_map<const char*, ComponentType> componentTypes {};

        // Map from type string pointer to a component array
        unordered_map<const char*, std::shared_ptr<IComponentArray>> componentArrays {};

        // The component type to be assigned to the next registered component - starting at 0
        ComponentType nextComponentType {};
    };
} }

#endif //ECS_COMPONENTMANAGER_H
