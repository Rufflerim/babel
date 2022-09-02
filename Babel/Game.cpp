#include <SDL_render.h>
#include "Game.h"
#include "SceneType.h"

void Game::load() {
    sceneManager.switchTo(scene::SceneType::GameMap);
}

void Game::update(const GameTime& time, const InputState& inputState) {
    sceneManager.update(time, inputState);
}

void Game::draw(engine::render::IRenderer& rendererBackend) {
    sceneManager.draw(rendererBackend);
}

void Game::close() {
    sceneManager.close();
}

void Game::lateUpdate() {
    sceneManager.processRequests();
}
