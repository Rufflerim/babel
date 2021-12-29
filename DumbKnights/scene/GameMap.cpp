//
// Created by gaetz on 24/12/2021.
//

#include "GameMap.h"
#include <Log.h>
#include <random>

using gmath::Vec2;

scene::GameMap::GameMap(engine::ecs::Coordinator& coordinator) : IScene(coordinator) {
    coordinator.init();
    entities.reserve(500);
    for (int i = 0; i < 500; ++i) {
        entities.push_back(0);
    }
}

void scene::GameMap::onInit() {
    engine::asset::Assets& assets = Locator::instance().assets();
    assets.loadTexture("Assets/mathieu.png", "mathieu");

    // Load entities
    coordinator.registerComponent<Transform2D>();
    coordinator.registerComponent<ColorRectangle>();
    coordinator.registerComponent<Sprite>();

    rendering = coordinator.registerSystem<RenderingSystem>();

    Signature spriteSignature;
    spriteSignature.set(coordinator.getComponentType<Transform2D>());
    spriteSignature.set(coordinator.getComponentType<Sprite>());
    coordinator.setSystemSignature<RenderingSystem>(spriteSignature);

    std::default_random_engine generator;
    std::uniform_real_distribution<float> randPositionX(100.0f, 1200.0f);
    std::uniform_real_distribution<float> randPositionY(100.0f, 600.0f);
    std::uniform_real_distribution<float> randScale(50.0f, 100.0f);
    //std::uniform_int_distribution randColor(0, 255);

    for (auto& entity: entities) {
        entity = coordinator.createEntity();
        coordinator.addComponent(entity,
                                 Transform2D{Vec2{randPositionX(generator), randPositionY(generator)},
                                             0, Vec2{1.0f, 1.0f}});
        coordinator.addComponent(entity,
                                 Sprite { "mathieu", Vec2 {} });

    }
}

void scene::GameMap::onClose() {

}

void scene::GameMap::activate() {

}

void scene::GameMap::inactivate() {

}

void scene::GameMap::update(const GameTime& time, const InputState& inputState) {

}

void scene::GameMap::draw(engine::render::IRenderer& renderer) {
    rendering->draw(coordinator, renderer);
}



