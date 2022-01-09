//
// Created by gaetz on 21/12/2021.
//

#ifndef ECS_COMPONENTARRAY_H
#define ECS_COMPONENTARRAY_H

#include "../Defines.h"
#include "../Asserts.h"
#include <array>
#include <unordered_map>

using std::array;
using std::unordered_map;

namespace engine { namespace ecs {
    class IComponentArray {
    public:
        virtual ~IComponentArray() = default;
        virtual void onEntityDestroyed(Entity entity) = 0;
    };

    template <typename T>
    class ComponentArray : public IComponentArray {
    public:
        /// Puts a new component at the end of the array and updates the indices
        /// \param entity Entity in which we want to add the component
        /// \param component Component we add to the entity
        void insertComponentData(Entity entity, T component) {
            GASSERT_DEBUG(entityToIndex.find(entity) == entityToIndex.end(), "Component added to same entity more than once.")
            size_t newIndex = size;
            entityToIndex[entity] = newIndex;
            indexToEntity[newIndex] = entity;
            componentArray[newIndex] = component;
            ++size;
        }

        /// Removes component data about gven entity
        /// \param entity Entity we want to delete the component data
        void removeComponentData(Entity entity) {
            GASSERT_DEBUG(entityToIndex.find(entity) != entityToIndex.end(), "Removing non-existent component.")

            // Copy element at end into deleted element's place to maintain density
            size_t indexOfRemovedEntity = entityToIndex[entity];
            size_t indexOfLastElement = size - 1;
            componentArray[indexOfRemovedEntity] = componentArray[indexOfLastElement];

            // Update map to point to moved spot
            Entity entityOfLastElement = indexToEntity[indexOfLastElement];
            entityToIndex[entityOfLastElement] = indexOfRemovedEntity;
            indexToEntity[indexOfRemovedEntity] = entityOfLastElement;

            entityToIndex.erase(entity);
            indexToEntity.erase(indexOfLastElement);
            --size;
        }

        /// Get the component data about given entity
        /// \param entity The entity we want its component data
        /// \return Component of given entity
        T& getData(Entity entity) {
            return componentArray.at(entityToIndex.at(entity));
        }

        /// Remove component data about an entity if it exists
        /// \param entity
        void onEntityDestroyed(Entity entity) override {
            if (entityToIndex.find(entity) != entityToIndex.end()) {
                removeComponentData(entity);
            }
        }

    private:
        /// Packed array of component of a type T, matching the maximum number of entities
        /// so that each entity has a specific component spot.
        array<T, MAX_ENTITIES> componentArray;

        /// Total size of valid entries in the component array
        size_t size { 0 };

        /// Converts an entity ID to the relative component array index
        unordered_map<Entity, size_t> entityToIndex;

        /// Converts a component array index to the relative entity ID
        unordered_map<size_t, Entity> indexToEntity;
    };
} }

#endif //ECS_COMPONENTARRAY_H
