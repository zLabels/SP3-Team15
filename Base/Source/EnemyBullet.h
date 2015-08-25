#pragma once
#include "Weapon.h"
#include "TileCollision.h"

class CEnemyBullet : public CWeapon
{
public:
	CEnemyBullet(void);
	~CEnemyBullet(void);
	
	void Update(CMap* m_cMap);	//Update bullet

private:
	float f_speed;	//Speed of bullet
};

