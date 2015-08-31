/******************************************************************************/
/*!
\file	CEnemyBullet.h
\author Princeton Chew
\par	email: 141991A@mymail.nyp.edu.sg
\brief
Enemy bullet Class
*/
/******************************************************************************/
#pragma once
#include "Weapon.h"
#include "TileCollision.h"

/******************************************************************************/
/*!
	Class CEnemyBullet
\brief
Derived class from CWeapon class.
Defines the functions and variables unique to this class from the base class.

*/
/******************************************************************************/
class CEnemyBullet : public CWeapon
{
public:
	CEnemyBullet(void);	//Default Constructor
	~CEnemyBullet(void);	//Default Destructor

	void Update(CMap* m_cMap);	//Update bullet

private:
	float f_speed;	//Speed of bullet
};

