#include "C3dAABB.h"


C3dAABB::C3dAABB(void)
{
}


C3dAABB::~C3dAABB(void)
{
}

void C3dAABB::CreateBoundingBox(Vector3 center,float height, float width,float depth)
{
	this->objCenter = center;
	this->objHeight = height;
	this->objWidth = width;
	this->objDepth = depth;

	float tempHalfWidth = objWidth * 0.5f;
	float tempHalfHeight = objHeight * 0.5f;
	float tempHalfDepth = objDepth * 0.5f;

	this->Maximum = Vector3(objCenter.x + tempHalfWidth,objCenter.y + tempHalfHeight,objCenter.z + tempHalfDepth);
	this->Minimum = Vector3(objCenter.x - tempHalfWidth,objCenter.y - tempHalfHeight,objCenter.z - tempHalfDepth);
}
bool CheckIntersect(Vector3 pos,C3dAABB& box2)
{
	if((pos.x <= box2.Maximum.x) && 
		(pos.y <= box2.Maximum.y) &&
		(pos.z <= box2.Maximum.z) && 
		(pos.x >= box2.Minimum.x) && 
		(pos.y >= box2.Minimum.y)&& 
		(pos.z >= box2.Minimum.z) )
	{
		return true;
	}
	return false;
}

void C3dAABB::setObjCenter(Vector3 input)
{
	this->objCenter = input;
}


Vector3 C3dAABB::getObjCenter()
{
	return this->objCenter;
}

float C3dAABB::getHeight()
{
	return this->objHeight;
}
float C3dAABB::getWidth()
{
	return this->objWidth;
}
float C3dAABB::getDepth()
{
	return this->objDepth;
}