#include "Strategy.h"

CStrategy::CStrategy()
{
}
CStrategy::~CStrategy()
{
}
void CStrategy::Update(bool,int)
{
	cout << "void CStrategy::Update()" << endl;
}

int CStrategy::CalculateDistance(void)
{
	return ((theDestination.x - theEnemyPosition.x) * (theDestination.x - theEnemyPosition.x) +
			(theDestination.y - theEnemyPosition.y) * (theDestination.y - theEnemyPosition.y));
}