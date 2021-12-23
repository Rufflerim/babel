//
// Created by gaetz on 06/10/2021.
//

#include "Locator.h"

Events* Locator::eventsService {nullptr};
NullEvents Locator::nullEventsService {};

void Locator::init() {
    eventsService = &nullEventsService;
}

void Locator::provide(Events* service) {
    if (service == nullptr) service = &nullEventsService;
    eventsService = service;
}

