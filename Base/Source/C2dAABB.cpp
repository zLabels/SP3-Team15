#include "C2dAABB.h"


C2dAABB::C2dAABB(void) :
		objHeight(0),
		objWidth(0)
{
}

C2dAABB::~C2dAABB(void)
{
}

void C2dAABB::CreateBoundingBox(Vector3 center,float height, float width)
{
	this->objCenter = center;
	this->objHeight = height;
	this->objWidth = width;

	float tempHalfWidth = objWidth * 0.5f;
	float tempHalfHeight = objHeight * 0.5f;

	this->Maximum = Vector3(objCenter.x + tempHalfWidth,objCenter.y + tempHalfHeight,1);
	this->Minimum = Vector3(objCenter.x - tempHalfWidth,objCenter.y - tempHalfHeight,1);
}
void C2dAABB::CreateBoundingBox(float pos_x,float pos_y,float height, float width)
{
	this->objHeight = height;
	this->objWidth = width;

	this->Maximum = Vector3(pos_x + width,pos_y + height,1);
	this->Minimum = Vector3(pos_x, pos_y,1);
}
bool CheckIntersect(Vector3 pos,C2dAABB* box2)
{
	if((pos.x <= box2->Maximum.x) && 
		(pos.y <= box2->Maximum.y) &&
		(pos.z <= box2->Maximum.z) && 
		(pos.x >= box2->Minimum.x) && 
		(pos.y >= box2->Minimum.y)&& 
		(pos.z >= box2->Minimum.z) )
	{
		return true;
	}
	return false;
}

void C2dAABB::setObjCenter(Vector3 input)
{
	this->objCenter = input;
}

Vector3 C2dAABB::getObjCenter()
{
	return this->objCenter;
}

float C2dAABB::getHeight()
{
	return this->objHeight;
}

float C2dAABB::getWidth()
{
	return this->objWidth;
}
