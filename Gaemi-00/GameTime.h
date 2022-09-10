#pragma once
#ifndef GAMETIME_H
#define GAMETIME_H

#include "Defines.h"

struct GameTime {
	/// <summary>
	/// Delta time in seconds
	/// </summary>
	f64 dt { 0.0 };

	/// <summary>
	/// Time since beginning of the game
	/// </summary>
	f64 totalTime { 0.0 };
};

#endif
