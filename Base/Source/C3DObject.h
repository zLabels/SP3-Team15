#pragma once
#include "Mesh.h"
#include "Vector3.h"
#include "C3dAABB.h"

class C3DObject
{
public:
	C3DObject(void);
	~C3DObject(void);

	void SetData(Mesh* ptr, Vector3 pos,Vector3 scale,bool Render, bool Light);

	void setRender(bool input);
	void setObjPos(Vector3 input);

	Mesh* getMesh(void);
	Vector3 getPos(void);
	bool getRender(void);
	bool getLight(void);
	C3dAABB& getBoundBox(void);
	void UpdateBoundBox(void);
	Vector3 getObjScale(void);

private:
	Mesh* objMesh;
	Vector3 objPos;
	Vector3 objScale;
	bool isRender;
	bool affectLight;
	C3dAABB BoundingBox;
	//C3dOBB BoundingBox;
	
};

