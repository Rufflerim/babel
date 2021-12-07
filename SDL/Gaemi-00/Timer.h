#pragma once
#ifndef TIMER_H
#define TIMER_H

#include "GameTime.h"
#include <chrono>

constexpr f32 TARGET_FPS = 60.0f;
constexpr f32 MIN_FRAME_DURATION = 1.0f / TARGET_FPS;

namespace engine {

	class Timer
	{
	public:
		Timer() = default;
		~Timer() = default;

		/// <summary>
		/// Compute game time
		/// </summary>
		/// <param name="time">Last frame time</param>
		/// <returns>New frame time</returns>
		GameTime computeTime(const GameTime& time);

		/// <summary>
		/// Hold the engine if it is faster than MIN_FRAME_DURATION
		/// </summary>
		/// <param name="time">Current frame time</param>
		void delayTime();

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> frameStartTime { std::chrono::high_resolution_clock::now() };
		std::chrono::time_point<std::chrono::high_resolution_clock> frameEndTime { std::chrono::high_resolution_clock::now() };
	};

}

#endif