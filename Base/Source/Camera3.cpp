#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"

Camera3::Camera3()
{
}

Camera3::~Camera3()
{
}

void Camera3::InitVariables()
{
	viewLimit = 0;

	tempPos = position;
	tempTarg = target;
}
void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);

	cameraRotate.SetZero();

	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();

	InitVariables();

	//Initialise the camera movement flags
	for(int i = 0; i< 255; i++)
	{
		myKeys[i] = false;
	}
}

void Camera3::UpdateStatus(const unsigned char key)
{
	myKeys[key] = true;
}
void Camera3::UpdateFalseStatus(const unsigned char key)
{
	myKeys[key] = false;
}

void Camera3::Update(double dt)
{
}

void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}