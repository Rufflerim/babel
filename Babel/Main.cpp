#include "Game.h"
#include "Locator.h"
#include <Entry.h>

using std::make_unique;

int main(int argc, char** argv) {
    for (int i = 0; i < argc; ++i) {
        LOG(LogLevel::Info) << argv[i];
    }

	// Game instance
    auto game = make_unique<Game>();

    // Locator instance
    Locator& locator = Locator::instance();

	// Start engine
	return engine::Entry::start(game.get(), locator);
}

