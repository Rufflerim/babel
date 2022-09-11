#include <SDL_render.h>
#include "Game.h"
#include "SceneType.h"

void Game::load() {
    isLoaded = true;
    sceneManager.switchTo(scene::SceneType::GameMap);
}

void Game::update(const GameTime& time, const InputState& inputState) {
    if (!isLoaded) return;
    sceneManager.update(time, inputState);
}

void Game::draw(engine::render::IRenderer& rendererBackend) {
    if (!isLoaded) return;
    sceneManager.draw(rendererBackend);
}

void Game::close() {
    isLoaded = false;
    sceneManager.close();
}

void Game::lateUpdate() {
    sceneManager.processRequests();
}
