/******************************************************************************/
/*!
\file	CGrapplingHook.h
\author Princeton Chew
\par	email: 141991A@mymail.nyp.edu.sg
\brief
Grappling hook class
*/
/******************************************************************************/
#pragma once
#include "Weapon.h"
#include "Map.h"
#include "TileCollision.h"

/******************************************************************************/
/*!
	Class CGrapplingHook
\brief
Derived class from CWeapon class.
Defines the functions and variables unique to this class from the base class.

*/
/******************************************************************************/
class CGrapplingHook : public CWeapon
{
public:
	CGrapplingHook(void);	//Default constructor
	~CGrapplingHook(void);	//Default destructor

	void setRotation(float input);	//Setting the rotation of the grappling hook
	float getRotation();	//Getting the rotation of the grappling hook

	void setHooked(bool input);	//Setting the hooked status of the grappling hook
	bool getHooked();	//Getting the hooked status of the grappling hook

	void Update(CMap* m_cMap,int mapOffset_X,int mapOffset_Y);	//Update the grappling hook based on the map

private:
	float speed;	//float Speed of grappling hook
	float rotation;	//float Rotation of grappling
	bool hooked;	//bool Hooked status of grappling hook for use to check if it has collided with a tile

};

