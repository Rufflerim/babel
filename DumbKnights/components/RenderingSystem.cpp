//
// Created by gaetz on 25/12/2021.
//

#include "RenderingSystem.h"

void RenderingSystem::draw(engine::ecs::Coordinator &coordinator, engine::render::IRenderer &renderer) {
    for (auto &entity: entities) {
        auto &transform = coordinator.getComponent<Transform2D>(entity);
        auto &rectangle = coordinator.getComponent<ColorRectangle>(entity);


        gmath::Rectangle rect {Vec2{transform.position.x + rectangle.rectangle.origin.x,
                                   transform.position.y + rectangle.rectangle.origin.y},
                              Vec2{rectangle.rectangle.size.x,
                                   rectangle.rectangle.size.y}
        };
        renderer.drawRectangle(rect, rectangle.color);
    }
}
