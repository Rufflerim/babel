//
// Created by gaetz on 06/10/2021.
//

#ifndef LOCATOR_H
#define LOCATOR_H

#include "Events.h"
#include "asset/Assets.h"

using engine::Events;
using engine::NullEvents;
using engine::asset::Assets;
using engine::asset::NullAssets;

/**
 * Service locator pattern, to access application-wide services. Better than a singleton.
 * Available services: platform, memory, events.
 *
 * Usage: Locator::service().expressionNeeded
 */
class Locator {
public:
    static void init();
    GAPI static Events& events() { return *eventsService; };
    static void provide(Events* service);
    GAPI static Assets& assets() { return *assetsService; };
    static void provide(Assets* service);

private:
    static Events* eventsService;
    static NullEvents nullEventsService;

    static Assets* assetsService;
    static NullAssets nullAssetsService;
};

#endif //LOCATOR_H
