//
// Created by gaetz on 23/12/2021.
//

#ifndef MATH_VEC2_H
#define MATH_VEC2_H

#include "../Defines.h"

namespace gmath {
    class Vec2 {
    public:
        Vec2();

        Vec2(float x, float y);

        float length() const;

        Vec2 &operator*=(float factor) {
            x *= factor;
            y *= factor;
            return *this;
        }

        float x;
        float y;
    };
}

#endif //MATH_VEC2_H
