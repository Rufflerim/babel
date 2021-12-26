#include <SDL_render.h>
#include "Game.h"

void Game::load() {

}

void Game::update(GameTime time) {
    sceneManager.update(time);
}

void Game::draw(SDL_Renderer *pRenderer) {
    sceneManager.draw(pRenderer);
}

void Game::close() {
    sceneManager.close();
}

void Game::lateUpdate() {
    sceneManager.processRequests();
}
