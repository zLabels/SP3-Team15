#include "C3dOBB.h"


C3dOBB::C3dOBB(void)
{
	boxHeight = 0.f;
	boxWidth = 0.f;
	boxDepth = 0.f;
	halfWidth = 0.f;
	halfHeight = 0.f;
	halfDepth = 0.f;

	//Center of the box
	boxCenter.SetZero();
	//4 points of the box
	topTopLeft.SetZero();	
	topTopRight.SetZero();
	topBtmLeft.SetZero();
	topBtmRight.SetZero();

	btmTopLeft.SetZero();	
	btmTopRight.SetZero();
	btmBtmLeft.SetZero();
	btmBtmRight.SetZero();

	//Used to calculate new bounding box after rotation
	unitX.Set(1,0,0);
	unitY.Set(0,1,0);
	unitZ.Set(0,0,1);
}

C3dOBB::~C3dOBB(void)
{
}

void C3dOBB::CreateBoundingBox(Vector3 boxCenter, float height,float width,float depth)
{
	this->boxCenter = boxCenter;
	this->boxHeight = height;
	this->boxWidth = width;
	this->boxDepth = depth;

	halfWidth = boxWidth/2;	//Width from center of box
	halfHeight = boxHeight/2;	//Length from center of box
	halfDepth = boxDepth/2;		//Depth from center of box

	topTopLeft.Set(boxCenter.x - halfWidth,boxCenter.y + halfHeight,boxCenter.z - halfDepth);
	topTopRight.Set(boxCenter.x + halfWidth,boxCenter.y + halfHeight,boxCenter.z - halfDepth);
	topBtmLeft.Set(boxCenter.x - halfWidth,boxCenter.y + halfHeight,boxCenter.z + halfDepth);
	topBtmRight.Set(boxCenter.x + halfWidth,boxCenter.y + halfHeight,boxCenter.z + halfDepth);

	btmTopLeft.Set(boxCenter.x - halfWidth, boxCenter.y - halfHeight, boxCenter.z - halfDepth);
	btmTopRight.Set(boxCenter.x + halfWidth, boxCenter.y - halfHeight, boxCenter.z - halfDepth);
	btmBtmLeft.Set(boxCenter.x - halfWidth, boxCenter.y - halfHeight, boxCenter.z + halfDepth);
	btmBtmRight.Set(boxCenter.x + halfWidth, boxCenter.y - halfHeight, boxCenter.z + halfDepth);

	unitX = (btmBtmRight - btmBtmLeft).Normalized();
	unitY = (topBtmRight - btmBtmRight).Normalized();
	unitZ = (btmBtmRight - btmTopRight).Normalized();
}

void C3dOBB::CreateBoundingBox(Vector3 topTopLeft, Vector3 topTopRight,Vector3 topBtmLeft, Vector3 topBtmRight,
							Vector3 btmTopLeft, Vector3 btmTopRight, Vector3 btmBtmLeft, Vector3 btmBtmRight)
{
	this->topTopLeft = topTopLeft;
	this->topTopRight = topTopRight;
	this->topBtmLeft = topBtmLeft;
	this->topBtmRight = topBtmRight;

	this->btmTopLeft = btmTopLeft;
	this->btmTopRight = btmTopRight;
	this->btmBtmLeft = btmBtmLeft;
	this->btmBtmRight = btmBtmRight;

	this->boxWidth = (btmBtmRight - btmBtmLeft).Length();
	this->boxHeight = (topBtmRight - btmBtmRight).Length();
	this->boxDepth = (btmBtmRight - btmTopRight).Length();

	halfWidth = boxWidth/2;	//Width from center of box
	halfHeight = boxHeight/2;	//Length from center of box
	halfDepth = boxDepth/2;		//Depth from center of box

	unitX = (btmBtmRight - btmBtmLeft).Normalized();
	unitY = (topBtmRight - btmBtmRight).Normalized();
	unitZ = (btmBtmRight - btmTopRight).Normalized();
}
bool CheckIntersect(C3dOBB& box1,C3dOBB& box2)
{
	Vector3 objDiff = box2.boxCenter - box1.boxCenter;

	Vector3 unitVectors[15];

	unitVectors[0] = box1.unitX;
	unitVectors[1] = box1.unitY;
	unitVectors[2] = box1.unitZ;
	unitVectors[3] = box2.unitX;
	unitVectors[4] = box2.unitY;
	unitVectors[5] = box2.unitZ;
	unitVectors[6] = (box1.unitX).Cross(box2.unitX);
	unitVectors[7] = (box1.unitX).Cross(box2.unitY);
	unitVectors[8] = (box1.unitX).Cross(box2.unitZ);
	unitVectors[9] = (box1.unitY).Cross(box2.unitX);
	unitVectors[10] = (box1.unitY).Cross(box2.unitY);
	unitVectors[11] = (box1.unitY).Cross(box2.unitZ);
	unitVectors[12] = (box1.unitZ).Cross(box2.unitX);
	unitVectors[13] = (box1.unitZ).Cross(box2.unitY);
	unitVectors[14] = (box1.unitZ).Cross(box2.unitZ);

	for (int j = 0; j < 15; ++j)
	{
		//Checks through the 15 cases, if there is a seperating axis between the two boxes
		//intersection is false
		if (Math::FAbs(objDiff.Dot(unitVectors[j])) >
			Math::FAbs((box1.halfWidth * box1.unitX).Dot(unitVectors[j])) +
			Math::FAbs((box1.halfHeight * box1.unitX).Dot(unitVectors[j])) +
			Math::FAbs((box1.halfDepth * box1.unitX).Dot(unitVectors[j])) +
			Math::FAbs((box2.halfWidth * box2.unitX).Dot(unitVectors[j])) +
			Math::FAbs((box2.halfHeight * box2.unitY).Dot(unitVectors[j])) +
			Math::FAbs((box2.halfDepth * box2.unitZ).Dot(unitVectors[j])))
		{
			return false;
		}
	}
	//boxes intersect
	return true;
}

void C3dOBB::setCenter(Vector3 input)
{
	this->boxCenter = input;
}

float C3dOBB::getHalfDepth(void)
{
	return halfDepth;
}
float C3dOBB::getHalfHeight(void)
{
	return halfHeight;
}
float C3dOBB::getHalfWidth(void)
{
	return halfWidth;
}

Vector3 C3dOBB::getCenter(void)
{
	return boxCenter;
}
Vector3 C3dOBB::getTopTopLeft(void)
{
	return topTopLeft;
}
Vector3 C3dOBB::getTopTopRight(void)
{
	return topTopRight;
}
Vector3 C3dOBB::getTopBtmLeft(void)
{
	return topBtmLeft;
}
Vector3 C3dOBB::getTopBtmRight(void)
{
	return topBtmRight;
}
Vector3 C3dOBB::getBtmTopLeft(void)
{
	return btmTopLeft;
}
Vector3 C3dOBB::getBtmTopRight(void)
{
	return btmTopRight;
}
Vector3 C3dOBB::getBtmBtmLeft(void)
{
	return btmBtmLeft;
}
Vector3 C3dOBB::getBtmBtmRight(void)
{
	return btmBtmRight;
}