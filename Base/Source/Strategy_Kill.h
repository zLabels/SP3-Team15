/******************************************************************************/
/*!
\file	CStrategy_Kill.h
\author Princeton Chew
\par	email: 141991A@mymail.nyp.edu.sg
\brief
Strategy_Kill class
*/
/******************************************************************************/
#pragma once
#include "Strategy.h"
#include "Vector2.h"

/******************************************************************************/
/*!
	Class CStrategy_Kill
\brief
Derived class of CStrategy class.
Defines the functions and variables unique to this class from the base class.

*/
/******************************************************************************/
class CStrategy_Kill
	: public CStrategy
{
public:
	CStrategy_Kill(void);	//Default constructor
	~CStrategy_Kill(void);	//Default destructor
	
	/*AI Current state*/
	enum CURRENT_STATE
	{
		IDLE,
		REPEL,
		ATTACK,
		PATROL,
		MAX_ENEMY_STATE,
	};

	void Update(bool heroInvisible,int patrolRange);	//Update enemy
	void SetDestination(const float x, const float y);	//Set Enemy Destination
	void GetEnemyPosition(float& x, float& y);	//Get Enemy position
	void SetEnemyPosition(const float x, const float y);	//Set Enemy position
	void SetEnemyStart(const float x, const float y);	//Set Enemy start position

	void GetEnemyDirection(bool &left, bool &right);	//Get enemy direction
	void SetEnemyDirection(const bool left,const bool right);	//Set enemy direction
	int getState(void);	//Get enemy state

	void SetState(CStrategy_Kill::CURRENT_STATE theEnemyState);	//Set the state of the AI

private:
	float AI_ATTACK_MS;	//Enemy attack speed
	float AI_PATROL_MS;	//Enemy patrol speed

	bool AtStart;	//Enemy back at start position

	CStrategy_Kill::CURRENT_STATE CurrentState;	//Enemy AI State
};