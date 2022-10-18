#include "Entry.h"
#include "Engine.h"

using engine::Engine;

GAPI i32 engine::Entry::start(engine::IGame* game, engine::ILocator& locator)
{
    // Log initialization
    Log::restart();

    // Engine
    i32 errorCode { 0 };
    auto engine { std::make_unique<Engine>()};
    engine->init(game, locator);
    errorCode = static_cast<i32>(engine->run());
    engine->close();

    return errorCode;
}
