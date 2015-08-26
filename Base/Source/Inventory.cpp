#include "Inventory.h"

CInventory::CInventory(void) :
		ShurikenCount(50)
{
}

CInventory::~CInventory(void)
{
}

CGrapplingHook& CInventory::getGrapple(void)
{
	return this->GrappleHook;
}

CShuriken* CInventory::FetchShuriken()
{
	for(std::vector<CShuriken *>::iterator it = shurikenList.begin(); it != shurikenList.end(); ++it)
	{
		CShuriken *star = (CShuriken *)*it;
		if(!star->getActive())
		{
			this->ShurikenCount--;
			star->setActive(true);
			return star;
		}
	}
	for(unsigned i = 0; i < 5; ++i)
	{
		CShuriken *star = new CShuriken();
		shurikenList.push_back(star);
	}
	this->ShurikenCount--;
	CShuriken *star = shurikenList.back();
	star->setActive(true);
	return star;
}

vector<CShuriken*>& CInventory::getShurikenList(void)
{
	return this->shurikenList;
}

int CInventory::getShurikenCount(void)
{
	return this->ShurikenCount;
}
void CInventory::IncrementShuriken(void)
{
	this->ShurikenCount++;
}