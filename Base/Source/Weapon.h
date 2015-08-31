/******************************************************************************/
/*!
\file	CWeapon.h
\author Princeton Chew
\par	email: 141991A@mymail.nyp.edu.sg
\brief
Weapon Class
*/
/******************************************************************************/
#pragma once
#include "Mesh.h"
#include "Vector3.h"
#include <vector>

using std::vector;

/******************************************************************************/
/*!
	Class CWeapon
\brief
Base class for all the weapons used.
Defines the base functions and variables all weapons should have.

*/
/******************************************************************************/
class CWeapon
{
public:
	CWeapon(void);	//Default constructor
	~CWeapon(void);	//Default destructor

	void setData(Mesh* ptr,float pos_x,float pos_y,int damage,float dir_x,float dir_y,bool active);	//Sets all data

	Vector3 getPos();	//Get weapon position 
	void setPos(Vector3 newPos);	//Set weapon position

	Vector3 CWeapon::getDir();	//Get weapon direction

	Mesh* getMesh();	//Get weapon mesh
	
	int getDamage();	//Get weapon damage

	void setActive(bool input);	//Set weapon active status
	bool getActive();	//Get weapon active status

private:
	Mesh* m_meshPtr;	//Mesh of the weapon

	Vector3 v3_Direction;	//Direction of weapon
	Vector3 v3_Pos;	//Position of weapon
	int i_Damage;	//Damage of weapon

	bool b_Active;	//Active status of weapon
};

