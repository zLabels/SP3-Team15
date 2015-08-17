#include "C3DObject.h"


C3DObject::C3DObject(void)
{
	objMesh = (NULL);
	objPos.SetZero();
	isRender = false;
	affectLight = false;
}

C3DObject::~C3DObject(void)
{
}

void C3DObject::SetData(Mesh* ptr, Vector3 pos,Vector3 scale,bool Render, bool Light)
{
	this->objMesh = ptr;
	this->objPos = pos;
	this->isRender = Render;
	this->affectLight = Light;
	this->objScale = scale;
}

void C3DObject::setRender(bool input)
{
	this->isRender = input;
}

void C3DObject::setObjPos(Vector3 input)
{
	this->objPos = input;
}

Mesh* C3DObject::getMesh(void)
{
	return objMesh;
}
Vector3 C3DObject::getPos(void)
{
	return objPos;
}
bool C3DObject::getRender(void)
{
	return isRender;
}
bool C3DObject::getLight(void)
{
	return affectLight;
}

C3dAABB& C3DObject::getBoundBox(void)
{
	return BoundingBox;
}

Vector3 C3DObject::getObjScale(void)
{
	return objScale;
}
void C3DObject::UpdateBoundBox(void)
{
	BoundingBox.setObjCenter(objPos);

	BoundingBox.CreateBoundingBox(BoundingBox.getObjCenter(),BoundingBox.getHeight(),BoundingBox.getWidth(),BoundingBox.getDepth());
}