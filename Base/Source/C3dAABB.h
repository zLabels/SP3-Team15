#pragma once
#include "Vector3.h"

class C3dAABB
{
public:
	C3dAABB(void);
	~C3dAABB(void);

	void CreateBoundingBox(Vector3 center,float height, float width,float depth);
	friend bool CheckIntersect(Vector3 pos,C3dAABB& box2);

	void setObjCenter(Vector3 input);

	Vector3 getObjCenter();
	float getHeight();
	float getWidth();
	float getDepth();

private:
	float objHeight;
	float objWidth;
	float objDepth;
	Vector3 objCenter;
	Vector3 Maximum;
	Vector3 Minimum;

};

