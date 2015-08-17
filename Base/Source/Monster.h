#pragma once
#include "Vector2.h"
#include "SpriteAnimation.h"
#include "Map.h"

enum MOB_STATE
{
	MOB_IDLE = 0,
	MOB_ALERT,
	MOB_DEAD,
	MOB_TOTAL
};

class CMonster
{
public:
	CMonster(void);
	~CMonster(void);

	void setData(int Hp, Vector2 Pos, float speed, bool active);

	Vector2 getMobPos(void);
	int getMobHP(void);
	float getMobSpeed(void);
	bool getMobActive(void);

	void Update(CMap* m_cMap,int mapOffset_X,int mapOffset_Y,int screenWidth, int screenHeight,double dt);

private:
	int mob_Hp;
	Vector2 mob_Pos;
	float mob_Speed;
	bool mob_Active;
};

