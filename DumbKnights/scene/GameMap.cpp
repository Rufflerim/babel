//
// Created by gaetz on 24/12/2021.
//

#include "GameMap.h"
#include "../components/Components.h"
#include <Log.h>
#include <random>

using gmath::Vec2;

scene::GameMap::GameMap(engine::ecs::Coordinator& coordinator) : IScene(coordinator) {
    coordinator.init();
    entities.reserve(1000);
    for (int i = 0; i < 1000; ++i) {
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
    //coordinator.registerComponent<AnimatedSprite>();
    coordinator.registerComponent<Move2D>();
    coordinator.registerComponent<Controller>();

    renderingSystem = coordinator.registerSystem<RenderingSystem>();
    moveSystem = coordinator.registerSystem<Move2DSystem>();
    controllerSystem = coordinator.registerSystem<ControllerSystem>();

    Signature animatedSpriteSignature;
    animatedSpriteSignature.set(coordinator.getComponentType<Transform2D>());
    animatedSpriteSignature.set(coordinator.getComponentType<Sprite>());
    //animatedSpriteSignature.set(coordinator.getComponentType<AnimatedSprite>());
    //animatedSpriteSignature.set(coordinator.getComponentType<Move2D>());
    coordinator.setSystemSignature<RenderingSystem>(animatedSpriteSignature);

    Signature moveSignature;
    moveSignature.set(coordinator.getComponentType<Transform2D>());
    moveSignature.set(coordinator.getComponentType<Move2D>());
    coordinator.setSystemSignature<Move2DSystem>(moveSignature);

    Signature controllerSignature;
    controllerSignature.set(coordinator.getComponentType<Controller>());
    controllerSignature.set(coordinator.getComponentType<Move2D>());
    coordinator.setSystemSignature<ControllerSystem>(controllerSignature);


    std::default_random_engine generator;
    std::uniform_real_distribution<float> randPositionX(50.0f, 1200.0f);
    std::uniform_real_distribution<float> randPositionY(50.0f, 600.0f);
    //std::uniform_real_distribution<float> randScale(0.06f, 0.25f);
    //std::uniform_int_distribution randColor(0, 255);

    Entity furior = coordinator.createEntity();
    coordinator.addComponent(furior,
                             Transform2D{Vec2{randPositionX(generator), randPositionY(generator)},
                                         0, Vec2{1.0f, 1.0f}});
    coordinator.addComponent(furior,
                             Sprite { "mathieu", Vec2::zero(), engine::render::Flip::None });
    coordinator.addComponent(furior, Move2D { 100, 0.99 } );
    coordinator.addComponent(furior, Controller{true});

    entities.push_back(furior);
}

void scene::GameMap::onClose() {

}

void scene::GameMap::activate() {

}

void scene::GameMap::inactivate() {

}

void scene::GameMap::update(const GameTime& time, const InputState& inputState) {
    controllerSystem->update(coordinator, inputState);
    moveSystem->update(coordinator, time);
}

void scene::GameMap::draw(engine::render::IRenderer& renderer) {
    renderingSystem->draw(coordinator, renderer);
}



