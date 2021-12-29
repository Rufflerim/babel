#include "Game.h"
#include "Locator.h"
#include <Entry.h>

int main(int argc, char** argv) {

	// Game instance
	Game& game = Game::instance();

    // Locator instance
    Locator& locator = Locator::instance();

	// Start engine
	return engine::Entry::start(game, locator);
}

