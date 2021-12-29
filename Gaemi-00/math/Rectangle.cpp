//
// Created by gaetz on 25/12/2021.
//

#include <SDL_rect.h>
#include "Rectangle.h"

gmath::Rectangle gmath::Rectangle::nullRectangle { Vec2 {-1, -1}, Vec2 { -1 ,-1 }};

const SDL_Rect gmath::Rectangle::toSdlRect() const {
    return { static_cast<i32>(origin.x), static_cast<i32>(origin.y),
             static_cast<i32>(size.x), static_cast<i32>(size.y) };
}
