//
// Created by gaetz on 25/12/2021.
//

#include <SDL_rect.h>
#include "RectangleInt.h"

gmath::RectangleInt gmath::RectangleInt::nullRectangle { Vec2i {-1, -1}, Vec2i { -1 ,-1 }};

const SDL_Rect gmath::RectangleInt::toSdlRect() const {
    return { origin.x, origin.y, size.x, size.y };
}
