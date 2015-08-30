#include "Shop.h"


CShop::CShop(void)
{
}


CShop::~CShop(void)
{
}

CShop::CShop(int cash)
{
	Cash = cash;
}

void CShop::SetCash(int cash)
{
	Cash = cash;
}
int CShop::GetCash(void)
{
	return Cash; 
}