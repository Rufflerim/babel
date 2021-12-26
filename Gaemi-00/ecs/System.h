//
// Created by gaetz on 26/12/2021.
//

#ifndef ECS_SYSTEM_H
#define ECS_SYSTEM_H

#include <set>
#include "../Defines.h"

namespace engine { namespace ecs {
    class System {
    public:
        std::set<Entity> entities {};
    };
} }
#endif //ECS_SYSTEM_H
