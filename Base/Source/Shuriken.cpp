#include "Shuriken.h"

CShuriken::CShuriken(void) :
		f_speed(10.f),
		mapOffset_X(0), 
		mapOffset_Y(0),
		mapFineOffset_X(0),
		mapFineOffset_Y(0)
{
}

CShuriken::~CShuriken(void)
{
}

void CShuriken::setSpeed(float input)
{

}

void CShuriken::Update(CMap* m_cMap,int mapOffset_X,int mapOffset_Y)
{
	this->mapOffset_X = mapOffset_X;
	this->mapOffset_Y = mapOffset_Y;

	Vector3 dir = getDir();
	dir.Normalize();

	Vector3 newPos = getPos();
	
	newPos += Vector3(dir.x * f_speed,dir.y * f_speed,dir.z * f_speed);
	setPos(newPos);

	if(SingleTileCollisionCheck(m_cMap,true,true,true,true,getPos().x,getPos().y,this->mapOffset_X))
	{
		setActive(false);
	}
}