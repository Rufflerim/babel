#include <SDL_render.h>
#include "Game.h"
#include "SceneType.h"

void Game::load() {
    sceneManager.switchTo(scene::SceneType::GameMap);
    isLoaded = true;
    LOG(LogLevel::Info) << "Game started.";
}

void Game::update(const GameTime& time, const InputState& inputState) {
#ifdef GPLATFORM_WEB
    if (!isLoaded) return;
#endif
    sceneManager.update(time, inputState);
}

void Game::draw(engine::render::IRenderer& rendererBackend) {
#ifdef GPLATFORM_WEB
    if (!isLoaded) return;
#endif
    sceneManager.draw(rendererBackend);
}

void Game::close() {
    sceneManager.close();
    isLoaded = false;
    LOG(LogLevel::Info) << "Game closed.";
}

void Game::lateUpdate() {
    sceneManager.processRequests();
}
