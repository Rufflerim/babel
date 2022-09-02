#include "Game.h"
#include "Locator.h"
#include <Entry.h>

using std::make_unique;

int main(int argc, char** argv) {

	// Game instance
    auto game = make_unique<Game>();

    // Locator instance
    Locator& locator = Locator::instance();

	// Start engine
	return engine::Entry::start(game.get(), locator);
}

