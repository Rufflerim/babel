//
// Created by gaetz on 25/12/2021.
//

#include "RenderingSystem.h"
#include "../Locator.h"
#include <Texture.h>
#include "Components.h"

void RenderingSystem::draw(engine::ecs::Coordinator& coordinator, engine::render::IRenderer& renderer) {
    for (auto& entity: entities) {
        auto& transform = coordinator.getComponent<Transform2D>(entity);

        /* Draw rectangle
        auto &rectangle = coordinator.getComponent<ColorRectangle>(entity);
        gmath::Rectangle rect {Vec2{transform.position.x + rectangle.rectangle.origin.x,
                                   transform.position.y + rectangle.rectangle.origin.y},
                              Vec2{rectangle.rectangle.size.x,
                                   rectangle.rectangle.size.y}
        };
        renderer.drawRectangle(rect, rectangle.color);
         */

        auto& sprite = coordinator.getComponent<Sprite>(entity);
        auto texture = Locator::instance().assets().getTexture(sprite.textureName).get();
        gmath::Rectangle dstRect{
                Vec2{transform.position.x + sprite.origin.x, transform.position.y + sprite.origin.y},
                Vec2{static_cast<f32>(texture->width) * transform.scale.x,
                     static_cast<f32>(texture->height) * transform.scale.y}
        };
        renderer.drawSprite(texture, gmath::Rectangle::nullRectangle, dstRect, transform.rotation,
                            sprite.origin, sprite.flip);
    }
}
