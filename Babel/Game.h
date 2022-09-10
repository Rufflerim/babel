#pragma once
#include <IGame.h>
#include <SceneManager.h>
#include <Coordinator.h>
#include <SDL_render.h>


class Game : public engine::IGame
{
public:
    Game() = default;
    ~Game() override = default;

    /// <summary>
	/// Game load
	/// </summary>
	void load() override;

	/// <summary>
	/// Update state of the world
	/// </summary>
	/// <param name="time">Game time</param>
	void update(const GameTime& time, const InputState& inputState) override;

	/// <summary>
	/// Renders the world
	/// </summary>
    void draw(engine::render::IRenderer& rendererBackend) override;

    /// <summary>
    /// Updates what need to be updated after drawing
    /// </summary>
    void lateUpdate() override;

	/// <summary>
	/// End game
	/// </summary>
	void close() override;

private:
    engine::ecs::Coordinator coordinator {};
    scene::SceneManager sceneManager { coordinator };
};

