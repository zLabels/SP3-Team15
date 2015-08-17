#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera.h"
#include <iostream>
#include <vector>
#include "C3dOBB.h"
#include "C3dAABB.h"


using std::cout;
using std::vector;

class Camera3 : public Camera
{
public:
	//Enum for camera types
	enum CAM_TYPE
	{
		LAND_CAM = 0,
		AIR_CAM,
		NUM_CAM_TYPE
	};

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	//Storing Temp position and target
	Vector3 tempPos;
	Vector3 tempTarg;
	Vector3 cameraRotate;

	//Collision
	vector<C3dAABB> hitbox;
	float terrainHeight;
	float playerOffSet;

	void InitVariables();

	bool NoClip; //Debugging;

	//Jumping related variables
	float JumpVel;
	bool isJumping;
	float JUMPMAXSPEED;
	float JUMPACCEL;
	float GRAVITY;//Gravity
	bool touchDown;

	//Sprint related variables
	float SPRINTMAXSPEED;

	//Crouching related variables
	float currentHeight;	//Player's current height when crouching
	bool isCrouching;
	bool Crouched;
	float crouchTime;
	float CROUCH_RATE;

	//Shooting related variables
	bool Gunrecoil;
	float recoil;

	//Collision
	bool collideRight;
	bool collideLeft;
	bool collideBtm;
	bool collideFront;
	bool collideBack;
	bool collide;

	float CAMERA_SPEED;

	Camera3();
	~Camera3();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();
	//Basic Methods
	virtual void TurnLeft(const double dt);
	virtual void TurnRight(const double dt);
	virtual void LookUp(const double dt);
	virtual void LookDown(const double dt);
	virtual void SpinClockWise(const double dt);
	virtual void SpinCounterClockWise(const double dt);

	//Applied Methods
	virtual void Pitch(const double dt);
	virtual void Yaw(const double dt);
	virtual void Roll(const double dt);
	virtual void Walk(const double dt);
	virtual void Strafe(const double dt);
	virtual void Jump(const double dt);
	virtual void Crouch(const double dt);
	virtual void RecoilUp(const double dt);
	void UpdateCrouch(const double dt);
	
	void UpdateJump(const double dt);

	virtual void MoveForward(const double dt);
	virtual void MoveBackward(const double dt);
	virtual void MoveLeft(const double dt);
	virtual void MoveRight(const double dt);
	virtual void Sprint(const double dt);

	virtual void UpdateStatus(const unsigned char key);
	virtual void UpdateFalseStatus(const unsigned char key);

	virtual void SetCameraType(CAM_TYPE sCameraType);
	virtual CAM_TYPE GetCameraType(void);

private:
	bool myKeys[255];
	CAM_TYPE sCameraType;

	int viewLimit;

};

#endif