//
// Created by gaetz on 25/12/2021.
//

#ifndef COMPONENTS_COMPONENTS_H
#define COMPONENTS_COMPONENTS_H

#include <Vec2.h>
#include <Color.h>
#include <Rectangle.h>
#include <render/IRenderer.h>

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

struct SpriteAnimation {
    u16 frameNumber { 0 };
    u16 frameWidth { 0 };
    u16 frameHeight { 0 };
};

struct AnimatedSprite {
    Vec2 origin;
    u16 frameIndex { 0 };
    f32 timeCounter { 0 };
    u16 currentAnimRow { 0 };
    std::vector<SpriteAnimation> animations;
    std::unordered_map<str, u16> animNameToRow;
    engine::render::Flip flip { engine::render::Flip::None };
    str textureName { "" };

    AnimatedSprite(const str& texNameP, const Vec2& originP, u16  ) {

    }

    void setCurrentAnim(const str& str) {
        currentAnimRow = animNameToRow[str];
    }
};

struct Move2D {
    f32 maxSpeed {0};
    f32 decelerationFactor {0};
    Vec2 acceleration;
    Vec2 speed;

    Move2D() = default;
    Move2D(f32 maxSpeedP, f32 decelerationFactorP) : maxSpeed{maxSpeedP}, decelerationFactor{decelerationFactorP} {}
};

#endif //COMPONENTS_COMPONENTS_H
