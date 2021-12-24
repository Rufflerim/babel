//
// Created by gaetz on 24/12/2021.
//

#include <GameTime.h>

#ifndef SCENE_ISCENE_H
#define SCENE_ISCENE_H

namespace scene {
    class IScene {
    public:
        virtual ~IScene() {}

        virtual void onInit() = 0;

        virtual void onClose() = 0;

        virtual void activate() = 0;

        virtual void inactivate() = 0;

        virtual void update(GameTime time) = 0;

        virtual void draw() = 0;

        bool isTransparent() const { return transparent; }

        void setTransparent(bool value) { transparent = value; }

        bool isNonLocking() const { return nonLocking; }

        void setNonLocking(bool value) { nonLocking = value; }

    protected:
        /// True if the scene under this scene in the stack must draw
        bool transparent{false};

        /// True if the scene under this scene in the stack must update
        bool nonLocking{false};
    };
}
#endif //SCENE_ISCENE_H
