#pragma once
#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SDL_events.h>

namespace engine {

	struct InputState {

	};

	class InputManager
	{
	public:
		InputManager() = default;

		/// <summary>
		/// Init input manager
		/// </summary>
		/// <returns>True is properly initialized</returns>
		bool init();

		/// <summary>
		/// Close input manager and free memory
		/// </summary>
		void close();

		/// <summary>
		/// Fill the input state thanks to SDL event
		/// </summary>
		/// <param name="event">A SDL event</param>
		void processSDLEvent(SDL_Event& event, bool& engineIsRunning);

		/// <summary>
		/// Return all input states of the game
		/// </summary>
		/// <returns>Complete input state</returns>
		const InputState& getInputState() { return inputState; }


	private:
		InputState inputState {};
	};
}

#endif