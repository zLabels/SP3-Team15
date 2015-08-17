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

	terrainHeight = 0.f;

	//Initialising camera type
	sCameraType = LAND_CAM;

	//Jumping related variables
	isJumping = false;
	GRAVITY = -115.0f;
	JumpVel = 0.0f;
	JUMPMAXSPEED = 1500.0f;
	JUMPACCEL = 80.0f;
	CAMERA_SPEED = 200.0f;
	touchDown = false;

	//Crouching related variables
	currentHeight = position.y;
	isCrouching = false;
	Crouched = false;

	//Sprinting related variables
	SPRINTMAXSPEED = 350.f;

	Gunrecoil = false;
	recoil = 0.f;

	playerOffSet = 20.f;

	collideLeft = false;
	collideBtm = false;
	collideRight = false;
	collideFront = false;
	collideBack = false;
	collide = false;

	NoClip = false;

	crouchTime = 0.f;
	CROUCH_RATE = 0.12f;

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

void Camera3::SetCameraType(CAM_TYPE sCameraType)
{
	this->sCameraType = sCameraType;
}
Camera3::CAM_TYPE Camera3::GetCameraType(void)
{
	return sCameraType;
}

void Camera3::TurnLeft(const double dt)
{
	Vector3 view = (target - position).Normalized();
	float yaw = (float)(-CAMERA_SPEED * Application::camera_yaw * (float)dt);
	cameraRotate.y += yaw;
	Mtx44 rotation;
	rotation.SetToRotation(yaw, 0, 1, 0);
	view = rotation * view;
	target = position + view;
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
}
void Camera3::TurnRight(const double dt)
{
	Vector3 view = (target - position).Normalized();
	float yaw = (float)(-CAMERA_SPEED * Application::camera_yaw * (float)dt);
	cameraRotate.y += yaw;
	Mtx44 rotation;
	rotation.SetToRotation(yaw, 0, 1, 0);
	view = rotation * view;
	target = position + view;
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();

}
void Camera3::LookUp(const double dt)
{
	//if(viewLimit < 20)
	{
		float pitch = (float)(-CAMERA_SPEED * Application::camera_pitch * (float)dt);
		cameraRotate.x -= pitch;
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
		//viewLimit++;
	}

}
void Camera3::LookDown(const double dt)
{
	//if(viewLimit > -20)
	{
		float pitch = (float)(-CAMERA_SPEED * Application::camera_pitch * (float)dt);
		cameraRotate.x -= pitch;
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
		//viewLimit--;
	}
}
void Camera3::SpinClockWise(const double dt)
{

}
void Camera3::SpinCounterClockWise(const double dt)
{

}

void Camera3::Pitch(const double dt)
{
	if(Application::camera_pitch > 0.0)
	{
		LookUp(dt);
	}
	else if(Application::camera_pitch < 0.0)
	{
		LookDown(dt);
	}
}
void Camera3::Yaw(const double dt)
{
	if(Application::camera_pitch > 0.0)
	{
		TurnLeft(dt);
	}
	else if(Application::camera_pitch < 0.0)
	{
		TurnRight(dt);
	}
}
void Camera3::Roll(const double dt)
{
}
void Camera3::Walk(const double dt)
{
	if(dt > 0)
	{
		MoveForward(dt);
	}
	else if(dt < 0)
	{
		MoveBackward(abs(dt));
	}
}
void Camera3::Strafe(const double dt)
{
	if( dt > 0)
	{
		MoveRight(dt);
	}
	else if(dt < 0)
	{
		MoveLeft(abs(dt));
	}
}
void Camera3::Jump(const double dt)
{
	if(isJumping == false)
	{
		isJumping = true;

		//Calculating jump velocity
		JumpVel = JUMPACCEL;//dt;

		//Factor in maximum speed limit
		if(JumpVel > JUMPMAXSPEED)
		{
			JumpVel = JUMPMAXSPEED;
		}
	}
}
void Camera3::Crouch(const double dt)
{
	if(Crouched == false && isCrouching == false && isJumping == false)
	{
		Crouched = true;
		isCrouching = true;
	}
	else if(Crouched == true && isCrouching == false  && isJumping == false)
	{
		Crouched = false;
		isCrouching = true;
	}
}
void Camera3::UpdateJump(const double dt)
{
	if(isJumping == true)
	{
		//Factor in gravity
		JumpVel += GRAVITY * (float)dt;

		//Update camera and target position
		position.y += JumpVel * (float)dt;
		target.y += JumpVel * (float)dt;

		//Check if the camera reach ground
		if(position.y <= terrainHeight + 20.f)
		{
			JumpVel = 0.0f;
			isJumping = false;
		}
	}
}
void Camera3::UpdateCrouch(const double dt)
{
	if(Crouched == true  && isCrouching == true)
	{
		crouchTime += (float)dt;
		if(crouchTime <= CROUCH_RATE)
		{
			position.y -= (float)(100.f * dt);
			target.y  -= (float)(100.f * dt);
			playerOffSet -= (float)(100.f * dt);	//Adjusting player offset to new height
		}
		else
		{
			crouchTime = 0.f;	//Resetting crouch timer
			isCrouching = false;
		}
	}
	else if(Crouched == false && isCrouching == true)
	{
		crouchTime += (float)dt;
		if(crouchTime <= CROUCH_RATE)
		{
			position.y += (float)(100.f * dt);
			target.y  += (float)(100.f * dt);
			playerOffSet += (float)(100.f * dt);	//Adjusting player offset to new height
		}
		else
		{
			crouchTime = 0.f;
			isCrouching = false;
		}
	}
}
void Camera3::MoveForward(const double dt)
{
	//Storing Temp position
	tempPos = position;
	tempTarg = target;

	float diff = target.y - position.y;

	Vector3 view = (target - position).Normalized();
	position += view * CAMERA_SPEED * (float)dt;
	target += view * CAMERA_SPEED * (float)dt;

	//Player height locking
	if(NoClip == false)
	{
		if(isJumping == false)
		{
			position.y = terrainHeight + playerOffSet;
			target.y = position.y + diff;
		}
		for(unsigned i = 0; i < hitbox.size(); ++i)
		{
			if(CheckIntersect(position,hitbox[i]) == true)
			{
				position.x = tempPos.x;
				position.y = tempPos.y;
				position.z = tempPos.z;

				target.x = tempTarg.x;
				target.y = tempTarg.y;
				target.z = tempTarg.z;
			}
		}
	}
}
void Camera3::MoveBackward(const double dt)
{
	//Storing Temp position
	tempPos = position;
	tempTarg = target;

	float diff = target.y - position.y;

	Vector3 view = (target - position).Normalized();
	position -= view * CAMERA_SPEED * (float)dt;
	target -= view * CAMERA_SPEED * (float)dt;

	//Player height locking
	if(NoClip == false)
	{
		if(isJumping == false)
		{
			position.y = terrainHeight + playerOffSet;
			target.y = position.y + diff;
		}
		for(unsigned i = 0; i < hitbox.size(); ++i)
		{
			if(CheckIntersect(position,hitbox[i]) == true)
			{
				position.x = tempPos.x;
				position.y = tempPos.y;
				position.z = tempPos.z;

				target.x = tempTarg.x;
				target.y = tempTarg.y;
				target.z = tempTarg.z;
			}
		}
	}
}
void Camera3::MoveLeft(const double dt)
{
	//Storing Temp position
	tempPos = position;
	tempTarg = target;

	float diff = target.y - position.y;

	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	position -= right * CAMERA_SPEED * (float)dt;
	target -= right * CAMERA_SPEED * (float)dt;

	//Player height locking
	if(NoClip == false)
	{
		if(isJumping == false)
		{
			position.y = terrainHeight + playerOffSet;
			target.y = position.y + diff;
		}
		for(unsigned i = 0; i < hitbox.size(); ++i)
		{
			if(CheckIntersect(position,hitbox[i]) == true)
			{
				position.x = tempPos.x;
				position.y = tempPos.y;
				position.z = tempPos.z;

				target.x = tempTarg.x;
				target.y = tempTarg.y;
				target.z = tempTarg.z;
			}
		}
	}
}
void Camera3::MoveRight(const double dt)
{
	//Storing Temp position
	tempPos = position;
	tempTarg = target;

	float diff = target.y - position.y;

	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	position += right * CAMERA_SPEED * (float)dt;
	target += right * CAMERA_SPEED * (float)dt;

	//Player height locking
	if(NoClip == false)
	{
		if(isJumping == false)
		{
			position.y = terrainHeight + playerOffSet;
			target.y = position.y + diff;
		}
		for(unsigned i = 0; i < hitbox.size(); ++i)
		{
			if(CheckIntersect(position,hitbox[i]) == true)
			{
				position.x = tempPos.x;
				position.y = tempPos.y;
				position.z = tempPos.z;

				target.x = tempTarg.x;
				target.y = tempTarg.y;
				target.z = tempTarg.z;
			}
		}
	}
}
void Camera3::Sprint(const double dt)
{
	if(CAMERA_SPEED <= SPRINTMAXSPEED)
	{
		CAMERA_SPEED += (float)(40*dt);
	}
	//Storing Temp position
	tempPos = position;
	tempTarg = target;

	float diff = target.y - position.y;

	Vector3 view = (target - position).Normalized();
	position += view * CAMERA_SPEED * (float)dt;
	target += view * CAMERA_SPEED * (float)dt;

	//Player height locking
	if(NoClip == false)
	{
		if(isJumping == false)
		{
			position.y = terrainHeight + playerOffSet;
			target.y = position.y + diff;
		}
		for(unsigned i = 0; i < hitbox.size(); ++i)
		{
			if(CheckIntersect(position,hitbox[i]) == true)
			{
				position.x = tempPos.x;
				position.y = tempPos.y;
				position.z = tempPos.z;

				target.x = tempTarg.x;
				target.y = tempTarg.y;
				target.z = tempTarg.z;
			}
		}
	}
}

void Camera3::RecoilUp(const double dt)
{
	float pitch = (float)(-CAMERA_SPEED * Application::camera_pitch * (float)dt) + 60.f * float(dt);
	recoil += pitch;
	cameraRotate.x -= pitch;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(view).Normalized();
	Mtx44 rotation;
	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	view = rotation * view;
	target = position + view;

	Gunrecoil = false;
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
	//Sprint
	if(myKeys[VK_SHIFT] == true && myKeys['w'] == true)
	{
		Sprint(dt);
		myKeys[VK_SHIFT] = false;
		myKeys['w'] = false;
	}
	else
	{
		if(CAMERA_SPEED > SPRINTMAXSPEED)	//Returning to normal speed
		{
			CAMERA_SPEED -= (float)(10*dt);
			if(CAMERA_SPEED <= 200.f)
			{
				CAMERA_SPEED = 200.f;
			}
		}
	}
	//Walk
	if(myKeys['w'] == true)
	{
		Walk(dt);
		myKeys['w'] = false;
	}
	if(myKeys['a'] == true)
	{
		Strafe(-dt);
		myKeys['a'] = false;
	}
	if(myKeys['d'] == true)
	{
		Strafe(dt);
		myKeys['d'] = false;
	}
	if(myKeys['s'] == true)
	{
		Walk(-dt);
		myKeys['s'] = false;
	}
	//Jumping
	if(myKeys[32] == true)
	{
		Jump(dt);
		myKeys[32] = false;
	}

	//Crouching
	if(myKeys[VK_LCONTROL] == true)
	{
		Crouch(dt);
		myKeys[VK_LCONTROL] = false;
	}

	UpdateCrouch(dt);
	UpdateJump(dt);
	//Falling
	/*touchDown = false;
	if(isJumping == false && position.y != 0.f && NoClip == false)
	{
	for(unsigned i = 0; i < groundMax.size();i++)
	{
	if(groundCollide(groundMax[i], groundMin[i]) == true)
	{
	touchDown = true;
	break;
	}
	if( i == groundMax.size() - 1 && touchDown == false)
	{
	position.y -= CAMERA_SPEED * (float)dt;
	target.y -= CAMERA_SPEED * (float)dt;
	}
	}
	}*/

	/*if(collideFront == true)
	{
	position += Vector3(0,0,1) * CAMERA_SPEED * (float)dt ;
	target += Vector3(0,0,1) * CAMERA_SPEED * (float)dt ;
	}
	else if(collideBack == true)
	{
	position += Vector3(0,0,-1) * CAMERA_SPEED * (float)dt ;
	target += Vector3(0,0,-1) * CAMERA_SPEED * (float)dt ;
	}
	else if(collideRight == true)
	{
	position += Vector3(-1,0,0) * CAMERA_SPEED * (float)dt ;
	target += Vector3(-1,0,0) * CAMERA_SPEED * (float)dt ;
	}
	else if(collideLeft == true)
	{
	position += Vector3(1,0,0) * CAMERA_SPEED * (float)dt ;
	target += Vector3(1,0,0) * CAMERA_SPEED * (float)dt ;
	}*/


	//Recoil
	if(Gunrecoil == true)
	{
		RecoilUp(dt);
	}

	if(Application::camera_yaw != 0)
	{
		Yaw(dt);
	}
	if(Application::camera_pitch != 0)
	{
		Pitch(dt);
	}
}

void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}