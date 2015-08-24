#pragma once
#include "Vector3.h"

class C2dAABB
{
public:
	C2dAABB(void);
	~C2dAABB(void);

	void CreateBoundingBox(Vector3 center,float height, float width);
	void CreateBoundingBox(float pos_x,float pos_y,float height, float width);
	friend bool CheckIntersect(Vector3 pos,C2dAABB* box2);

	void setObjCenter(Vector3 input);

	Vector3 getObjCenter();
	float getHeight();
	float getWidth();

private:
	float objHeight;
	float objWidth;
	Vector3 objCenter;
	Vector3 Maximum;
	Vector3 Minimum;
};

