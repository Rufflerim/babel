//
// Created by gaetz on 25/12/2021.
//

#ifndef MATH_RECTANGLE_H
#define MATH_RECTANGLE_H

#include "Vec2.h"

namespace gmath {
    class Rectangle {
    public:
        Vec2 offset { 0, 0 };
        Vec2 size { 1, 1 };
    };
}


#endif //MATH_RECTANGLE_H
