#pragma once
#include "Vector2.h"
#include <iostream>

using std::cout;
using std::endl;

class CStrategy
{
public:
	CStrategy(void);
	~CStrategy(void);
	virtual void Update(bool)=0;
	virtual void SetDestination(const float x, const float y)=0;
	virtual void GetEnemyPosition(float& x, float& y)=0;
	virtual void SetEnemyPosition(const float x, const float y)=0;

	virtual void SetEnemyStart(const float x, const float y)=0;

	virtual void GetEnemyDirection(bool& left, bool& right)=0;
	virtual void SetEnemyDirection(const bool left,const bool right)=0;

	virtual int getState(void) = 0;

	int CalculateDistance(void);

	// The Destination is the position of the target
	Vector2 theDestination;
	//The Start position of the enemy
	Vector2	theEnemyStart;
	// The position of the enemy
	Vector2 theEnemyPosition;

	bool faceLeft;	//Enemy facing direction
	bool faceRight;	//Enemy facing direction
};