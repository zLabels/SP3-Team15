/******************************************************************************/
/*!
\file	Strategy.h
\author Princeton Chew
\par	email: 141991A@mymail.nyp.edu.sg
\brief
Base Strategy Class
*/
/******************************************************************************/
#pragma once
#include "Vector2.h"
#include <iostream>

using std::cout;
using std::endl;

/******************************************************************************/
/*!
	Class CStrategy
\brief
Used as the base class for other derived strategy classes.
Defines the functions and variables for use in derived classes.

*/
/******************************************************************************/
class CStrategy
{
public:
	CStrategy(void);	//Default constructor
	~CStrategy(void);	//Default destructor

	virtual void Update(bool,int)=0;	//Pure virtual function for strategy update
	virtual void SetDestination(const float x, const float y)=0;	//Pure virtual function for setting destination of strategy
	virtual void GetEnemyPosition(float& x, float& y)=0;	//Pure virtual function for getting position of AI
	virtual void SetEnemyPosition(const float x, const float y)=0;	//Pure virtual function for setting position of AI

	virtual void SetEnemyStart(const float x, const float y)=0;	//Pure virtual function for setting start position of AI

	virtual void GetEnemyDirection(bool& left, bool& right)=0;	//Pure virtual function for getting AI direction
	virtual void SetEnemyDirection(const bool left,const bool right)=0;	//Pure virtual function for setting AI direction

	virtual int getState(void) = 0;	//Pure virtual function for getting the state of the AI

	int CalculateDistance(void);	//Function to calculate the distance between destination and position

	Vector2 theDestination;	// The Destination is the position of the target
	Vector2	theEnemyStart;	//The Start position of the enemy
	Vector2 theEnemyPosition;	// The position of the enemy

	bool faceLeft;	//Enemy facing direction
	bool faceRight;	//Enemy facing direction
};