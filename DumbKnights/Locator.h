//
// Created by gaetz on 06/10/2021.
//

#ifndef LOCATOR_H
#define LOCATOR_H

#include <Events.h>
#include <asset/Assets.h>
#include <ILocator.h>

using engine::Events;
using engine::NullEvents;
using engine::asset::Assets;
using engine::asset::NullAssets;

/**
 * Service locator pattern, to access application-wide services. Better than a singleton.
 * Available services: platform, memory, events.
 *
 * Usage: Locator::service().expressionNeeded
 *
 * This Locator is created in the game, and filled up in the engine, so that it
 * can be use everywhere.
 */
class Locator : public engine::ILocator {
public:
    /// Create and returns a Locator instance
    /// \return Locator instance reference
    static Locator& instance() {
        static Locator locatorInstance;
        return locatorInstance;
    }

    void init(Events* events, Assets* assets) override;

    engine::Events& events() override { return *eventsService; };
    Assets& assets() override { return *assetsService; };

protected:
    Events* eventsService {nullptr};
    static engine::NullEvents nullEventsService;

    Assets* assetsService {nullptr};
    static NullAssets nullAssetsService;

    void provide(Events* service) override;
    void provide(Assets* service) override;
    Locator() = default;
};

#endif //LOCATOR_H
