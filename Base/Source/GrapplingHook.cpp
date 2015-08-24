#include "GrapplingHook.h"


CGrapplingHook::CGrapplingHook(void) :
			speed(10.f),
			rotation(0.f),
			hooked(false)
{
}

CGrapplingHook::~CGrapplingHook(void)
{
}

void CGrapplingHook::setRotation(float input)
{
	this->rotation = input;
}
float CGrapplingHook::getRotation()
{
	return this->rotation;
}

void CGrapplingHook::setHooked(bool input)
{
	this->hooked = input;
}
bool CGrapplingHook::getHooked()
{
	return this->hooked;
}

void CGrapplingHook::Update(CMap* m_cMap,int mapOffset_X,int mapOffset_Y)
{
	Vector3 dir = getDir();
	dir.Normalize();

	Vector3 newPos = getPos();
	
	newPos += Vector3(dir.x * speed,dir.y * speed,dir.z * speed);
	setPos(newPos);

	if(SingleTileCollisionCheck(m_cMap,true,true,true,true,getPos().x,getPos().y,mapOffset_X))
	{
		setActive(false);
		hooked = true;
	}

}