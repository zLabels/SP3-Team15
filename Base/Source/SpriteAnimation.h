/******************************************************************************/
/*!
\file	CSpriteAnimation.h
\author Princeton Chew
\par	email: 141991A@mymail.nyp.edu.sg
\brief
Sprite animation class
*/
/******************************************************************************/
#pragma once
#include "Mesh.h"
#include "Math.h"
#include <vector>

using std::vector;

/******************************************************************************/
/*!
	Struct Animation
\brief
Defines the variables required for sprite animation
*/
/******************************************************************************/
struct Animation
{
	Animation() {}

	void Set(int startFrame, int endFrame, int repeat, float time,bool active,Vector3 pos,int currentFrame,float currentTime, float scale) 
	{
		this->startFrame = startFrame;
		this->endFrame = endFrame;
		this->repeatCount = repeat;
		this->animTime = time;
		this->animActive = active;
		this->animPosition = pos;
		this->animCurrentFrame = currentFrame;
		this->animeCurrentTime = currentTime;
		this->animScale = scale;
	}

	int startFrame;	//Start frame of sprite animation in sprite sheet
	int endFrame;	//End frame of sprite animation in sprite sheet
	int animNumFrame;	//number of frame 
	int repeatCount;	//Repeat count of animation
	float animTime;	//Time for animation to complete
	int animCurrentFrame;	//Current frame of animation
	float animeCurrentTime;	//Current time of animation
	float animScale;	//Scale of animation
	bool ended;	//Ended status of animation
	bool animActive;	//Active status of animation
	Vector3 animPosition;	//Position of animation
};

/******************************************************************************/
/*!
		Class CSpriteAnimation
\brief
Controls sprite animations.
Defines the variables and functions required for sprite animations.

*/
/******************************************************************************/
class CSpriteAnimation : public Mesh
{
public:
	CSpriteAnimation(const std::string &meshName, int row, int col);	//Constructor for sprite animation
	~CSpriteAnimation();	//Default destructor

	void Update(double dt);	//Updating sprite animation

	virtual void Render();	//Virtual render function

	int m_row;	//Number of row in sprite sheet
	int m_col;	//Number of column in sprite sheet

	float m_currentTime;	//Current animated time of sprite animation
	int m_currentFrame;	//Current frame in of sprite
	int m_playCount;	//Number of play

	Animation *m_anim;	//Animation struct pointer
};