//
// Created by gaetz on 25/12/2021.
//

#ifndef MATH_RECTANGLEINT_H
#define MATH_RECTANGLEINT_H

#include "Vec2i.h"

namespace gmath {
    /// Rectangle representation
    class RectangleInt {
    public:
        // Default constructor
        RectangleInt() = default;

        // 2-parameters constructor
        RectangleInt(Vec2i originP, Vec2i sizeP)
        : origin {  originP }, size { sizeP }
        {}

        // 4-parameters constructor
        RectangleInt(i32 x, i32 y, i32 width, i32 height)
        : origin { x, y }, size { width, height }
        {}

        /// Origin, offset or position of the rectangle
        Vec2i origin {0, 0 };

        /// Size of the rectangle, horizontal then vertical
        Vec2i size { 1, 1 };

        /// Convert a rectangle to a SDL Rect
        /// \return New sdl rect with this rectangle size
        const SDL_Rect toSdlRect() const;

        /// Check rectangle equality
        /// \param other Other rectangle
        /// \return True if rectangles coordinates and size are the same
        bool operator==(const RectangleInt& other) const {
            return origin == other.origin && size == other.size;
        }

        /// Null rectangle. Coordinates and sizes are -1.
        static RectangleInt nullRectangle;
    };
}


#endif //MATH_RECTANGLEINT_H
