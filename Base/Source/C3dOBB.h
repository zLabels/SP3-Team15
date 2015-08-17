#pragma once
#include "Vector3.h"
#include "MyMath.h"
#include "Mtx44.h"

class C3dOBB
{
public:
	C3dOBB(void);
	~C3dOBB(void);

	void CreateBoundingBox(Vector3 boxCenter, float height,float width,float depth);
	void CreateBoundingBox(Vector3 topTopLeft, Vector3 topTopRight,Vector3 topBtmLeft, Vector3 topBtmRight,
							Vector3 btmTopLeft, Vector3 btmTopRight, Vector3 btmBtmLeft, Vector3 btmBtmRight);
	friend bool CheckIntersect(C3dOBB& box1,C3dOBB& box2);

	void setCenter(Vector3 input);

	float getHalfDepth(void);
	float getHalfHeight(void);
	float getHalfWidth(void);
	Vector3 getCenter(void);
	Vector3 getTopTopLeft(void);
	Vector3 getTopTopRight(void);
	Vector3 getTopBtmLeft(void);
	Vector3 getTopBtmRight(void);
	Vector3 getBtmTopLeft(void);
	Vector3 getBtmTopRight(void);
	Vector3 getBtmBtmLeft(void);
	Vector3 getBtmBtmRight(void);
	
private:
	float boxHeight;	//Length of whole object
	float boxWidth;		//Width of whole object
	float boxDepth;		//Depth of whole object
	float halfDepth;	//Depth from center of box
	float halfWidth;	//Width from center of box
	float halfHeight;	//Length from center of box

	//Center of the box
	Vector3 boxCenter;
	//8 points of the box
	Vector3 topTopLeft;	
	Vector3 topTopRight;
	Vector3 topBtmLeft;
	Vector3 topBtmRight;

	Vector3 btmTopLeft;	
	Vector3 btmTopRight;
	Vector3 btmBtmLeft;
	Vector3 btmBtmRight;

	//Used to calculate new bounding box after rotation
	Vector3 unitX;
	Vector3 unitY;
	Vector3 unitZ;

};

