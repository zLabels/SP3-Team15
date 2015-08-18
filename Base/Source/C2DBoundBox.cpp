#include "C2DBoundBox.h"

C2DBoundBox::C2DBoundBox(void)
{
}

C2DBoundBox::~C2DBoundBox(void)
{
}

void C2DBoundBox::CreateBoundingBox(Vector2 center,float height, float width,float depth)
{
	this->objCenter = center;
	this->objHeight = height;
	this->objWidth = width;
	this->objDepth = depth;

	float tempHalfWidth = objWidth * 0.5f;
	float tempHalfHeight = objHeight * 0.5f;
	float tempHalfDepth = objDepth * 0.5f;

	this->Maximum.x = ( objCenter.x + tempHalfWidth );
	this->Maximum.y = ( objCenter.y + tempHalfHeight );
	
	this->Minimum.x = ( objCenter.x - tempHalfWidth );
	this->Minimum.y = ( objCenter.y - tempHalfHeight );
}
bool CheckIntersect(Vector2 pos,C2DBoundBox& box2)
{
	if((pos.x <= box2.Maximum.x) && 
		(pos.y <= box2.Maximum.y) &&
		(pos.x >= box2.Minimum.x) && 
		(pos.y >= box2.Minimum.y) )
	{
		return true;
	}
	return false;
}

void C2DBoundBox::setObjCenter(Vector2 input)
{
	this->objCenter.x = input.x;
	this->objCenter.y = input.y;
}

float C2DBoundBox::getObjCenter_x()
{
	return this->objCenter.x;
}
float C2DBoundBox::getObjCenter_y()
{
	return this->objCenter.y;
}

float C2DBoundBox::getHeight()
{
	return this->objHeight;
}
float C2DBoundBox::getWidth()
{
	return this->objWidth;
}
float C2DBoundBox::getDepth()
{
	return this->objDepth;
}