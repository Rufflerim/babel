//
// Created by gaetz on 29/12/2021.
//

#ifndef ILOCATOR_H
#define ILOCATOR_H

#include "asset/Assets.h"
#include "Events.h"

using engine::asset::Assets;
using engine::Events;

namespace engine {

    class ILocator {
    public:
        /// Locator init
        virtual void init(Events* events, Assets* assets) = 0;

        /// Returns engine event handler
        virtual Events& events() = 0;

        /// Returns assets handler
        virtual Assets& assets() = 0;

    protected:
        /// Provides the event services to the locator
        /// \param service Event manager as a service
        virtual void provide(Events* service) = 0;

        /// Provides the assets services to the locator
        /// \param service Asset maager as a service
        virtual void provide(Assets* service) = 0;
    };

}
#endif //ILOCATOR_H
