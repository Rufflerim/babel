//
// Created by gaetz on 23/12/2021.
//

#include "Vec2.h"
#include "Functions.h"

Vec2::Vec2(float x, float y) : x { x }, y { y } {

}

Vec2::Vec2() : x { 0 }, y { 0 } {

}

float Vec2::length() {
    return gmath::sqrt(x * x + y * y);
}