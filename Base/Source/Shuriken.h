#pragma once
#include "Weapon.h"
#include "Map.h"

class CShuriken : public CWeapon
{
	enum TILE_TYPE
	{
		TILE_GRASS = 1,
		TILE_UNDERGROUND,
		TILE_BORDER,
		TILE_CAVE,
		MAX_TILE,
	};
public:
	CShuriken(void);
	~CShuriken(void);


	void setSpeed(float input);	//Set speed of shuriken

	bool CheckCollision (CMap* m_cMap, bool m_bCheckLeft,bool m_bCheckRight, bool m_bCheckUp, bool m_bCheckDown);	//Check collision of shuriken with tilemap

	void Update(CMap* m_cMap,int mapOffset_X,int mapOffset_Y);	//Update shuriken

private:
	float speed;	//Speed of shuriken

	int mapOffset_X, mapOffset_Y;	//Map offset X , Y	for scrolling
	int mapFineOffset_X, mapFineOffset_Y;	//Map fine offset X , Y for scrolling
};

