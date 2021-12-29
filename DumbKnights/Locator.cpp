//
// Created by gaetz on 06/10/2021.
//

#include "Locator.h"

engine::NullEvents Locator::nullEventsService {};
NullAssets Locator::nullAssetsService {};

void Locator::init(Events* events, Assets* assets) {
    eventsService = events;
    assetsService = assets;
}

void Locator::provide(Events* service) {
    if (service == nullptr) service = &nullEventsService;
    eventsService = service;
}

void Locator::provide(Assets* service) {
    if (service == nullptr) service = &nullAssetsService;
    assetsService = service;
}

