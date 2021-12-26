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

struct Transform2D {
    Vec2 position;
    float rotation;
    Vec2 scale;
};

struct ColorRectangle {
    Color color;
    Rectangle rectangle;
};

#endif //COMPONENTS_COMPONENTS_H
