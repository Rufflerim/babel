//
// Created by gaetz on 25/12/2021.
//

#include "AnimationSystem.h"
#include "Components.h"
#include <Vec2i.h>

using gmath::Vec2i;

void AnimationSystem::update(engine::ecs::Coordinator& coordinator, const GameTime& time) {


    auto animatorComps = coordinator.getComponentArray<Animator>();
    auto spriteComps = coordinator.getComponentArray<Sprite>();
    //auto& animator = coordinator.getComponent<Animator>(entity);
    //auto& sprite = coordinator.getComponent<Sprite>(entity);

    for (auto entity: entities) {
        auto& animator = animatorComps->getData(entity);
        auto& sprite = spriteComps->getData(entity);

        // PLay state, make animation loop
        auto& currentRow = animator.animations.rows[animator.currentAnimRow];
        if (animator.state == AnimatorState::Play) {
            animator.timeCounter += time.dt;
            animator.frameIndex = animator.timeCounter / animator.animations.frameTime;
            u16 frameNumber = currentRow.frameNumber;
            if (animator.frameIndex >= frameNumber) {
                animator.frameIndex = 0;
                animator.timeCounter -= currentRow.frameNumber * animator.animations.frameTime;
            }
        }

        sprite.srcRect = gmath::RectangleInt {
                Vec2i { animator.frameIndex * animator.animations.frameWidth,
                       animator.currentAnimRow * animator.animations.frameHeight },
                Vec2i { animator.animations.frameWidth,
                       animator.animations.frameHeight }};

        /*
        if (animator.)



        renderer.drawSprite(texture, gmath::Rectangle::nullRectangle, dstRect, transform.rotation,
                            sprite.origin, sprite.flip);
                            */
    }
}
