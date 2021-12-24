#pragma once
#include <IGame.h>
#include <SceneManager.h>


class Game : public engine::IGame
{
public:
	static Game& instance() {
		static Game gameInstance;
		return gameInstance;
	}

	/// <summary>
	/// Game load
	/// </summary>
	void load() override;

	/// <summary>
	/// Update state of the world
	/// </summary>
	/// <param name="time">Game time</param>
	void update(GameTime time) override;

	/// <summary>
	/// Renders the world
	/// </summary>
	void draw() override;

	/// <summary>
	/// End game
	/// </summary>
	void close() override;

private:
	Game() = default;
    scene::SceneManager sceneManager;
};

