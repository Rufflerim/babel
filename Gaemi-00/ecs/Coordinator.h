//
// Created by gaetz on 23/12/2021.
//

#ifndef ECS_COORDINATOR_H
#define ECS_COORDINATOR_H

#include <memory>
#include "../Defines.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"

/**
 * Example of the ECS coordinator use.
 *
 *

Coordinator gCoordinator;

struct Gravity {
	Vec3 force;
};

struct RigidBody {
	Vec3 velocity;
	Vec3 acceleration;
};

struct Transform {
	Vec3 position;
	Vec3 rotation;
	Vec3 scale;
};

extern Coordinator gCoordinator;

void PhysicsSystem::Update(float dt) {
	for (auto const& entity : mEntities) {
		auto& rigidBody = gCoordinator.GetComponent<RigidBody>(entity);
		auto& transform = gCoordinator.GetComponent<Transform>(entity);
		auto const& gravity = gCoordinator.GetComponent<Gravity>(entity);

		transform.position += rigidBody.velocity * dt;
		rigidBody.velocity += gravity.force * dt;
	}
}

int main() {
	gCoordinator.init();

	gCoordinator.registerComponent<Gravity>();
	gCoordinator.registerComponent<RigidBody>();
	gCoordinator.registerComponent<Transform>();

	auto physicsSystem = gCoordinator.registerSystem<PhysicsSystem>();

	Signature signature;
	signature.set(gCoordinator.getComponentType<Gravity>());
	signature.set(gCoordinator.getComponentType<RigidBody>());
	signature.set(gCoordinator.getComponentType<Transform>());
	gCoordinator.setSystemSignature<PhysicsSystem>(signature);

	std::vector<Entity> entities(MAX_ENTITIES);

	std::default_random_engine generator;
	std::uniform_real_distribution<float> randPosition(-100.0f, 100.0f);
	std::uniform_real_distribution<float> randRotation(0.0f, 3.0f);
	std::uniform_real_distribution<float> randScale(3.0f, 5.0f);
	std::uniform_real_distribution<float> randGravity(-10.0f, -1.0f);

	float scale = randScale(generator);

	for (auto& entity : entities) {
		entity = gCoordinator.createEntity();
		gCoordinator.addComponent(entity, Gravity {Vec3(0.0f, randGravity(generator), 0.0f) });
		gCoordinator.AddComponent(
			entity,
			RigidBody{
				.velocity = Vec3(0.0f, 0.0f, 0.0f),
				.acceleration = Vec3(0.0f, 0.0f, 0.0f)
			});
		gCoordinator.AddComponent(
			entity,
			Transform{
				.position = Vec3(randPosition(generator), randPosition(generator), randPosition(generator)),
				.rotation = Vec3(randRotation(generator), randRotation(generator), randRotation(generator)),
				.scale = Vec3(scale, scale, scale)
			});
	}

	float dt = 0.0f;
	while (!quit) {
		auto startTime = std::chrono::high_resolution_clock::now();
		physicsSystem->Update(dt);
		auto stopTime = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
	}
}
 *
 */


namespace engine { namespace ecs {
    class Coordinator {
    public:
        /// Initialize ECS coordination
        void init();


        // Entity methods

        /// Creates an entity
        /// \return Created entity ID
        Entity createEntity();

        /// Detroys an entity and remove its ECS system and component dependencies
        /// \param entity The entity we want to destroy
        void destroyEntity(Entity entity);


        // Component methods

        /// Register a component
        /// \tparam T Component class
        template<typename T>
        void registerComponent() {
            componentManager->registerComponent<T>();
        }

        /// Adds a component to a given entity and manage its signature
        /// \tparam T Component class
        /// \param entity Entity on which the component is added
        /// \param component Component we want to add
        template<typename T>
        void addComponent(Entity entity, T component) {
            componentManager->addComponent<T>(entity, component);

            auto signature = entityManager->getSignature(entity);
            signature.set(componentManager->getComponentType<T>(), true);
            entityManager->setSignature(entity, signature);

            systemManager->onEntitySignatureChanged(entity, signature);
        }

        /// Removes a component from a given entity and manage its signature
        /// \tparam T Component class
        /// \param entity Entity from which we want to remove a component
        template<typename T>
        void removeComponent(Entity entity) {
            componentManager->removeComponent<T>(entity);

            auto signature = entityManager->getSignature(entity);
            signature.set(componentManager->getComponentType<T>(), false);
            entityManager->setSignature(entity, signature);

            systemManager->onEntitySignatureChanged(entity, signature);
        }

        /// Get a specific component from an entity
        /// \tparam T Component class
        /// \param entity Targeted entity
        /// \return Reference to the given component
        template<typename T>
        T& getComponent(Entity entity) {
            return componentManager->getComponent<T>(entity);
        }

        /// Get a component ID from its class
        /// \tparam T Component class
        /// \return Component ID
        template<typename T>
        ComponentType getComponentType() {
            return componentManager->getComponentType<T>();
        }


        // System methods

        /// Register a system in order to use it
        /// \tparam T System type
        /// \return Shared pointer to the system
        template<typename T>
        std::shared_ptr<T> registerSystem() {
            auto system = systemManager->registerSystem<T>();
            return system;
        }

        template<typename T>
        void setSystemSignature(Signature signature) {
            systemManager->setSignature<T>(signature);
        }

    private:
        std::unique_ptr<ComponentManager> componentManager;
        std::unique_ptr<EntityManager> entityManager;
        std::unique_ptr<SystemManager> systemManager;
    };
} }

#endif //ECS_COORDINATOR_H
