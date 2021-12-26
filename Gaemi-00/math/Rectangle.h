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
    };
}


#endif //MATH_RECTANGLE_H
