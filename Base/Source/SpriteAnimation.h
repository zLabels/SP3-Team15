#pragma once

#include "Mesh.h"
#include "Math.h"
#include <vector>

using std::vector;

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

	int startFrame;
	int endFrame;
	int animNumFrame;
	int repeatCount;
	float animTime;
	int animCurrentFrame;
	float animeCurrentTime;
	float animScale;
	bool ended;
	bool animActive;
	Vector3 animPosition;
};

class CSpriteAnimation : public Mesh
{
public:
	CSpriteAnimation(const std::string &meshName, int row, int col);
	~CSpriteAnimation();
	void Update(double dt);
	virtual void Render();

	int m_row;
	int m_col;
	//int numFrame;

	float m_currentTime;
	//float frameTime;
	int m_currentFrame;
	int m_playCount;

	Animation *m_anim;
	
	////Sprite Animations
	//vector<Animation*> collidesprite;
	//Animation* Saber_idle;
	//Animation* Saber_run;
};