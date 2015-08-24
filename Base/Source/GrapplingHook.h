#pragma once
#include "Weapon.h"
#include "Map.h"
#include "TileCollision.h"

class CGrapplingHook : public CWeapon
{
public:
	CGrapplingHook(void);
	~CGrapplingHook(void);

	void setRotation(float input);
	float getRotation();

	void setHooked(bool input);
	bool getHooked();

	void Update(CMap* m_cMap,int mapOffset_X,int mapOffset_Y);

private:
	float speed;
	float rotation;
	bool hooked;

};

