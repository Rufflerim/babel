//
// Created by gaetz on 23/12/2021.
//

#include "Vec2.h"
#include "Functions.h"
#include "Vec2i.h"

using gmath::Vec2;

Vec2::Vec2(f32 x, f32 y) : x{x}, y{y} {

}

Vec2::Vec2() : x{0}, y{0} {

}

f32 Vec2::length() const {
    return gmath::sqrt(x * x + y * y);
}

f32 gmath::Vec2::squareLength() const {
    return x * x + y * y;
}

Vec2& gmath::Vec2::normalize() {
    if (*this == Vec2::zero()) return *this;
    f32 len = length();
    x = x / len;
    y = y / len;
    return *this;
}

gmath::Vec2i gmath::Vec2::toVec2i() {
    return gmath::Vec2i { gmath::round(x), gmath::round(y) };
}
