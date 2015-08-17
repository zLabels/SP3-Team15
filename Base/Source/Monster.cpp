#include "Monster.h"


CMonster::CMonster(void):
	mob_Hp(20),
	mob_Speed(3.f),
	mob_Active(false)
{
}

CMonster::~CMonster(void)
{
}

void CMonster::setData(int Hp, Vector2 Pos, float speed,bool active)
{
	this->mob_Hp = Hp;

	this->mob_Pos.x = Pos.x;
	this->mob_Pos.y = Pos.y;

	this->mob_Speed = speed;

	this->mob_Active = active;
}

Vector2 CMonster::getMobPos(void)
{
	return this->mob_Pos;
}

int CMonster::getMobHP(void)
{
	return this->mob_Hp;
}

float CMonster::getMobSpeed(void)
{
	return this->mob_Speed;
}

bool CMonster::getMobActive(void)
{
	return this->mob_Active;
}

void CMonster::Update(CMap* m_cMap,int mapOffset_X,int mapOffset_Y,int screenWidth, int screenHeight,double dt)
{

}