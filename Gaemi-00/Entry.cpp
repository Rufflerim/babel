#include "Entry.h"
#include "Engine.h"
#include "Log.h"

using engine::Engine;

GAPI i32 engine::Entry::start(engine::IGame* game, engine::ILocator& locator)
{
    // Log initialization
    Log::restart();

    // Engine
    u32 errorCode { 0 };
    Engine engine;
    engine.init(game, locator);
    errorCode = static_cast<i32>(engine.run());
    engine.close();

    return errorCode;
}
