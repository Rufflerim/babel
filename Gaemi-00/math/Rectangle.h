//
// Created by gaetz on 25/12/2021.
//

#ifndef MATH_RECTANGLE_H
#define MATH_RECTANGLE_H

#include "Vec2.h"

namespace gmath {
    /// Rectangle representation
    class Rectangle {
    public:
        /// Origin, offset or position of the rectangle
        Vec2 origin {0, 0 };

        /// Size of the rectangle, horizontal then vertical
        Vec2 size { 1, 1 };

        /// Convert a rectangle to a SDL Rect
        /// \return New sdl rect with this rectangle size
        const SDL_Rect toSdlRect() const;

        /// Check rectangle equality
        /// \param other Other rectangle
        /// \return True if rectangles coordinates and size are the same
        bool operator==(const Rectangle& other) const {
            return origin == other.origin && size == other.size;
        }

        /// Null rectangle. Coordinates and sizes are -1.
        static Rectangle nullRectangle;
    };
}


#endif //MATH_RECTANGLE_H
