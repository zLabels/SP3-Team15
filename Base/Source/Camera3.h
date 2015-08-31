#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera.h"
#include <iostream>
#include <vector>

using std::cout;
using std::vector;

class Camera3 : public Camera
{
public:
	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	//Storing Temp position and target
	Vector3 tempPos;
	Vector3 tempTarg;
	Vector3 cameraRotate;

	void InitVariables();

	Camera3();
	~Camera3();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();

	virtual void UpdateStatus(const unsigned char key);
	virtual void UpdateFalseStatus(const unsigned char key);

private:
	bool myKeys[255];

	int viewLimit;
};

#endif