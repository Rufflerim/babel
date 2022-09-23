//
// Created by gaetz on 19/09/2022.
//

#ifndef RENDER_IPIPELINE_H
#define RENDER_IPIPELINE_H

#include "Defines.h"

class IPipeline {
public:
    virtual ~IPipeline() = default;

    virtual void load();
    virtual void render();
};

#endif //RENDER_IPIPELINE_H
