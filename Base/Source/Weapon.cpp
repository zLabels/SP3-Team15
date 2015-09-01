#include "Weapon.h"

CWeapon::CWeapon(void) :
	m_meshPtr(NULL),
	i_Damage(0),
	b_Active(false)
{
	v3_Pos.Set(0,0,1);
	v3_Direction.Set(0,0,1);
}

CWeapon::~CWeapon(void)
{
	if(m_meshPtr)
	{
		delete m_meshPtr;
		m_meshPtr = NULL;
	}
}

void CWeapon::setData(Mesh* ptr,float pos_x,float pos_y,int damage,float dir_x,float dir_y,bool active)
{
	this->m_meshPtr = ptr;
	this->v3_Pos.x = pos_x;
	this->v3_Pos.y = pos_y;
	this->i_Damage = damage;
	this->v3_Direction.x = dir_x;
	this->v3_Direction.y = dir_y;
	this->b_Active = active;
}

Vector3 CWeapon::getPos()
{
	return this->v3_Pos;
}
void CWeapon::setPos(Vector3 newPos)
{
	this->v3_Pos.x = newPos.x;
	this->v3_Pos.y = newPos.y;
}

Vector3 CWeapon::getDir()
{
	return this->v3_Direction;
}

Mesh* CWeapon::getMesh()
{
	return this->m_meshPtr;
}
int CWeapon::getDamage()
{
	return this->i_Damage;
}

void CWeapon::setActive(bool input)
{
	this->b_Active = input;
}
bool CWeapon::getActive()
{
	return this->b_Active;
}