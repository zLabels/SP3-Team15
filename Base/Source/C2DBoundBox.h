#pragma once
#include "Vector2.h"

class C2DBoundBox
{
public:
	C2DBoundBox(void);
	~C2DBoundBox(void);

	void CreateBoundingBox(Vector2 center,float height, float width,float depth);
	friend bool CheckIntersect(Vector2 pos,C2DBoundBox& box2);

	void setObjCenter(Vector2 input);

	float getObjCenter_x();
	float getObjCenter_y();
	float getHeight();
	float getWidth();
	float getDepth();

private:
	float objHeight;
	float objWidth;
	float objDepth;
	Vector2 objCenter;
	Vector2 Maximum;
	Vector2 Minimum;

};

