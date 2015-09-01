/******************************************************************************/
/*!
\file	CInventory.h
\author Princeton Chew
\par	email: 141991A@mymail.nyp.edu.sg
\brief
Inventory Class
*/
/******************************************************************************/
#pragma once
#include "GrapplingHook.h"
#include "Shuriken.h"

/******************************************************************************/
/*!
		Class CInventory
\brief
Defines the variables and functions required for the inventory.

*/
/******************************************************************************/
class CInventory
{
public:
	CInventory(void);	//Default constructor
	~CInventory(void);	//Default destructor

	CShuriken* FetchShuriken();	//Returns an unactive shuriken from the vector to be reused

	CGrapplingHook& getGrapple(void);	//Gets grappling hook
	vector<CShuriken*>& getShurikenList(void);	//Gets vector of shuriken

	void setShurikenCount(int); // Set shuriken
	int getShurikenCount(void);	//Get shuriken count
	void IncrementShuriken(void);	//Increase shuriken count by 1

private:
	int ShurikenCount;	//Number of shuriken
	vector<CShuriken*> shurikenList;	//Vector of shuriken pointers for player to use
	CGrapplingHook GrappleHook;	//Grappling hook for player

};

