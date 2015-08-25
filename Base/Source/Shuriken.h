#pragma once
#include "Weapon.h"
#include "TileCollision.h"

class CShuriken : public CWeapon
{
public:
	CShuriken(void);
	~CShuriken(void);

	void setSpeed(float input);	//Set speed of shuriken

	void Update(CMap* m_cMap,int mapOffset_X,int mapOffset_Y);	//Update shuriken

private:
	float f_speed;	//Speed of shuriken

	int mapOffset_X, mapOffset_Y;	//Map offset X , Y	for scrolling
	int mapFineOffset_X, mapFineOffset_Y;	//Map fine offset X , Y for scrolling
};

