//
// Created by gaetz on 06/10/2021.
//

#include "Locator.h"

Events* Locator::eventsService {nullptr};
NullEvents Locator::nullEventsService {};
Assets* Locator::assetsService {nullptr};
NullAssets Locator::nullAssetsService {};

void Locator::init() {
    eventsService = &nullEventsService;
    assetsService = &nullAssetsService;
}

void Locator::provide(Events* service) {
    if (service == nullptr) service = &nullEventsService;
    eventsService = service;
}

void Locator::provide(Assets* service) {
    if (service == nullptr) service = &nullAssetsService;
    assetsService = service;
}

