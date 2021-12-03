#include "Entry.h"
#include "Engine.h"
#include "Log.h"

using engine::Engine;

GAPI int engine::Entry::start(engine::IGame& game)
{
    // Log initialization
    Log::restart();

    // Engine
    Engine engine;
    engine.init(game);
    engine.run();
    engine.close();

    return 0;
}
