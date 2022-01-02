//
// Created by gaetz on 23/12/2021.
//

#include "Vec2i.h"
#include "Functions.h"
#include "Vec2.h"

using gmath::Vec2i;

Vec2i::Vec2i(i32 x, i32 y) : x{x}, y{y} {

}

Vec2i::Vec2i() : x{0}, y{0} {

}

f32 Vec2i::length() const {
    return gmath::sqrt(x * x + y * y);
}

u32 gmath::Vec2i::squareLength() const {
    return x * x + y * y;
}

gmath::Vec2 gmath::Vec2i::toVec2() {
    return gmath::Vec2 { static_cast<f32>(x), static_cast<f32>(y) };
}
