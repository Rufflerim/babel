#include <iostream>
#include "Game.h"
#include <Entry.h>

int main() {

	// Game instance
	Game game = Game::instance();

	// Start engine
	return engine::Entry::start(game);
}
