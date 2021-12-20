#include "Game.h"
#include <Entry.h>

int main(int argc, char** argv) {

	// Game instance
	Game game = Game::instance();

	// Start engine
	return engine::Entry::start(game);
}
