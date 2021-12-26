//
// Created by gaetz on 25/12/2021.
//

#include <SDL_rect.h>
#include "RenderingSystem.h"

void RenderingSystem::draw(engine::ecs::Coordinator &coordinator, SDL_Renderer *pRenderer) {
    for(auto& entity : entities) {
        auto& transform = coordinator.getComponent<Transform2D>(entity);
        auto& rectangle = coordinator.getComponent<ColorRectangle>(entity);

        SDL_Rect fillRect = { (i32)transform.position.x + (i32)rectangle.rectangle.offset.x,
                              (i32)transform.position.y + (i32)rectangle.rectangle.offset.y,
                              (i32)rectangle.rectangle.size.x,
                              (i32)rectangle.rectangle.size.y
        };
        SDL_SetRenderDrawColor( pRenderer, rectangle.color.r, rectangle.color.g, rectangle.color.b, rectangle.color.a );
        SDL_RenderFillRect( pRenderer, &fillRect );
    }
}
