#pragma once
#ifndef ENTRY_H
#define ENTRY_H

#include "Gaemi.h"
#include "IGame.h"

namespace engine {

	class Entry
	{
	public:
		GAPI static int start(engine::IGame& game);
	};

}

#endif