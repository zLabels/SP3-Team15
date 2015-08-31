#pragma once
#include "Goodies.h"
#include "TreasureChest.h"

typedef int GoodiesID;

class CGoodiesFactory
{
public:
	//Enum for type of goodies available
	enum GOODIES_TYPE
	{
		TREASURE_CHEST = 0,
		MAX_GOODIES
	};

	CGoodiesFactory(void);	//Default constructor
	~CGoodiesFactory(void);	//Default destructor

	CGoodies* Create(GoodiesID);	//Function to create a new goodie pointer
};

