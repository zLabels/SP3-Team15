/******************************************************************************/
/*!
\file	CShuriken.h
\author Princeton Chew
\par	email: 141991A@mymail.nyp.edu.sg
\brief
Shuriken Class
*/
/******************************************************************************/
#pragma once
#include "Weapon.h"
#include "TileCollision.h"

/******************************************************************************/
/*!
		Class CShuriken
\brief
Derived class from CWeapon class.
Defines the functions and variables unique to this class from the base class.

*/
/******************************************************************************/
class CShuriken : public CWeapon
{
public:
	CShuriken(void);	//Default constructor
	~CShuriken(void);	//Default destructor

	void setSpeed(float input);	//Set speed of shuriken

	void Update(CMap* m_cMap,int mapOffset_X,int mapOffset_Y);	//Update shuriken

private:
	float f_speed;	//Speed of shuriken

	int mapOffset_X, mapOffset_Y;	//Map offset X , Y	for scrolling
	int mapFineOffset_X, mapFineOffset_Y;	//Map fine offset X , Y for scrolling
};

