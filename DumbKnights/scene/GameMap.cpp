//
// Created by gaetz on 24/12/2021.
//

#include "GameMap.h"
#include <Log.h>
#include <random>

using gmath::Vec2;

scene::GameMap::GameMap(engine::ecs::Coordinator &coordinator) : IScene(coordinator) {
    coordinator.init();
    entities.reserve(5000);
    for(int i = 0; i < 5000; ++i) {
        entities.push_back(0);
    }
}

void scene::GameMap::onInit() {
    coordinator.registerComponent<Transform2D>();
    coordinator.registerComponent<ColorRectangle>();

    rendering = coordinator.registerSystem<RenderingSystem>();

    Signature rectangleSignature;
    rectangleSignature.set(coordinator.getComponentType<Transform2D>());
    rectangleSignature.set(coordinator.getComponentType<ColorRectangle>());
    coordinator.setSystemSignature<RenderingSystem>(rectangleSignature);

    std::default_random_engine generator;
    std::uniform_real_distribution<float> randPositionX(100.0f, 1200.0f);
    std::uniform_real_distribution<float> randPositionY(100.0f, 600.0f);
    std::uniform_real_distribution<float> randScale(10.0f, 50.0f);
    std::uniform_int_distribution randColor(0, 255);

    for(auto& entity : entities) {
        entity = coordinator.createEntity();
        coordinator.addComponent(entity, Transform2D { Vec2 { randPositionX(generator), randPositionY(generator) },
                                                       0, Vec2 { 1.0f, 1.0f } });
        coordinator.addComponent(entity, ColorRectangle { gmath::Color { (u8)randColor(generator), (u8)randColor(generator), (u8)randColor(generator), 255 },
                                                          gmath::Rectangle { Vec2{}, Vec2 { randScale(generator), randScale(generator) } }});

    }
}

void scene::GameMap::onClose() {

}

void scene::GameMap::activate() {

}

void scene::GameMap::inactivate() {

}

void scene::GameMap::update(GameTime time) {

}

void scene::GameMap::draw(engine::render::IRenderer& renderer) {
    rendering->draw(coordinator, renderer);
}



