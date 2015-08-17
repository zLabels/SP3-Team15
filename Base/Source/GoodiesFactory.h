#pragma once

#include "Goodies.h"
#include "TreasureChest.h"

typedef int GoodiesID;

class CGoodiesFactory
{
public:
	enum GOODIES_TYPE
	{
		TREASURE_CHEST = 0,
		MAX_GOODIES
	};

	CGoodiesFactory(void);
	~CGoodiesFactory(void);

	CGoodies* Create(GoodiesID);
};

