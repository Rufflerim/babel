//
// Created by gaetz on 06/10/2021.
//

#ifndef LOCATOR_H
#define LOCATOR_H

#include "Events.h"

using engine::Events;
using engine::NullEvents;
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

private:
    static Events* eventsService;
    static NullEvents nullEventsService;
};

#endif //LOCATOR_H
