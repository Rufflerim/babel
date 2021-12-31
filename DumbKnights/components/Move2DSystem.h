//
// Created by gaetz on 31/12/2021.
//

#ifndef MOVE2DSYSTEM_H
#define MOVE2DSYSTEM_H

#include <GameTime.h>
#include <InputManager.h>

class Move2DSystem {
    void update(const GameTime& time, const engine::input::InputState& inputState);
};


#endif //MOVE2DSYSTEM_H
