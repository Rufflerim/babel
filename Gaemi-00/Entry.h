#pragma once
#ifndef ENTRY_H
#define ENTRY_H

#include "Defines.h"
#include "IGame.h"
#include "ILocator.h"

namespace engine {

	class Entry
	{
	public:
		GAPI static i32 start(engine::IGame* game, engine::ILocator& locator);
	};

}

#endif
