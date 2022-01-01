//
// Created by gaetz on 31/12/2021.
//

#ifndef MOVE2DSYSTEM_H
#define MOVE2DSYSTEM_H

#include <GameTime.h>
#include <InputManager.h>
#include <System.h>

class Move2DSystem : public engine::ecs::System {
public:
    void update(engine::ecs::Coordinator& coordinator, const GameTime& time);
};


#endif //MOVE2DSYSTEM_H
