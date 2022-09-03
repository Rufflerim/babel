//
// Created by gaetz on 25/12/2021.
//

#ifndef COMPONENTS_COMPONENTS_H
#define COMPONENTS_COMPONENTS_H

#include <Vec2.h>
#include <Color.h>
#include <Rectangle.h>
#include <render/IRenderer.h>

#include <utility>
#include "../data/AnimationData.h"
#include "../Locator.h"

using gmath::Vec2;
using gmath::Color;
using gmath::Rectangle;

/// Transform for a 2D entity
struct Transform2D {
    Vec2 position { Vec2::zero() };
    f64 rotation { 0.0 };
    Vec2 scale { Vec2::zero() };
};

/// Rendering a color rectangle
struct ColorRectangle {
    Color color;
    Rectangle rectangle;
};

/// Rendering an image or a part of it
struct Sprite {
    Vec2 origin { Vec2::zero() };
    gmath::RectangleInt srcRect { gmath::RectangleInt::nullRectangle };
    gmath::Vec2 dstSize { Vec2::zero() };
    engine::render::Flip flip { engine::render::Flip::None };
    str textureName;

    Sprite() = default;

    Sprite(const str& textureNameP, const Vec2& originP, const gmath::RectangleInt& srcRectP,
           const Vec2& dstSizeP = Vec2::zero(), engine::render::Flip flipP = engine::render::Flip::None)
            : textureName { textureNameP }, origin { originP }, srcRect { srcRectP }, dstSize { dstSizeP },
              flip { flipP } {
        // If destination size is zero, load it from texture size
        if (dstSizeP == gmath::Vec2::zero()) {
            auto tex = Locator::instance().assets().getTexture(textureNameP);
            dstSize = { static_cast<f32>(tex->width), static_cast<f32>(tex->height) };
        }
    }
};

enum class AnimatorState {
    Play, Stop, Pause
};

/// Flipbook animation manager
struct Animator {
    Vec2 origin;
    u16 frameIndex { 0 };
    u16 currentAnimRow { 0 };
    f32 timeCounter { 0 };
    AnimatorState state { AnimatorState::Play };
    data::AnimationData animations;
    engine::render::Flip flip { engine::render::Flip::None };

    Animator() = default;

    Animator(data::AnimationData animationsP, const Vec2& originP, engine::render::Flip flipP)
            : animations { std::move(animationsP) }, origin { originP }, flip { flipP } {

    }

    void setCurrentAnim(const str& str) {
        auto newAnimItr = std::find_if(begin(animations.rows), end(animations.rows),
                                       [&](data::AnimationRow& row) { return row.name == str; });
        currentAnimRow = newAnimItr->row;
    }
};

/// 2D movement
struct Move2D {
    f32 maxSpeed { 0 };
    f32 decelerationFactor { 0 };
    Vec2 acceleration { Vec2::zero() };
    Vec2 speed { Vec2::zero() };

    Move2D() = default;

    Move2D(f32 maxSpeedP, f32 decelerationFactorP) : maxSpeed { maxSpeedP },
                                                     decelerationFactor { decelerationFactorP } {}
};

/// Control an entity orders
struct Controller {
    Vec2 inputAxis { Vec2::zero() };
    bool isPlayer { false };

    Controller() = default;

    explicit Controller(bool isPlayerP) : isPlayer { isPlayerP } {}
};

#endif //COMPONENTS_COMPONENTS_H
