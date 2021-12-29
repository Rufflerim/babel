//
// Created by gaetz on 25/12/2021.
//

#ifndef COMPONENTS_COMPONENTS_H
#define COMPONENTS_COMPONENTS_H

#include <Vec2.h>
#include <Color.h>
#include <Rectangle.h>

using gmath::Vec2;
using gmath::Color;
using gmath::Rectangle;

/// Transform for a 2D entity
struct Transform2D {
    Vec2 position;
    f64 rotation { 0.0 };
    Vec2 scale;
};

/// Rendering a color rectangle
struct ColorRectangle {
    Color color;
    Rectangle rectangle;
};

struct Sprite {
    str textureName;
    Vec2 origin;
    engine::render::Flip flip { engine::render::Flip::None };
};

#endif //COMPONENTS_COMPONENTS_H
