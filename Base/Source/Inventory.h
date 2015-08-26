#pragma once
#include "GrapplingHook.h"
#include "Shuriken.h"

class CInventory
{
public:
	CInventory(void);
	~CInventory(void);

	CShuriken* FetchShuriken();	//Returns an unactive shuriken from the vector to be reused

	CGrapplingHook& getGrapple(void);	//Gets grappling hook
	vector<CShuriken*>& getShurikenList(void);	//Gets vector of shuriken

	int getShurikenCount(void);	//Get shuriken count
	void IncrementShuriken(void);	//Increase shuriken count by 1

private:
	int ShurikenCount;
	vector<CShuriken*> shurikenList;
	CGrapplingHook GrappleHook;

};

