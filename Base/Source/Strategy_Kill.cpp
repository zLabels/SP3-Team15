#include "Strategy_Kill.h"
#include <iostream>

CStrategy_Kill::CStrategy_Kill() :
		AI_ATTACK_MS(2.f),
		AI_PATROL_MS(1.f),
		PATROL_OFFSET(100),
		AtStart(true)
{
}
CStrategy_Kill::~CStrategy_Kill()
{
}

void CStrategy_Kill::Update(void)
{
	int distanceDestinationToEnemy = CalculateDistance();
	
	if( distanceDestinationToEnemy < 40000.f)
	{
		if( distanceDestinationToEnemy < 2500.f)
		{
			CurrentState = REPEL;
		}
		else
		{
			CurrentState = ATTACK;
			AtStart = false;
		}
	}
	else if(theEnemyPosition.x != theEnemyStart.x)
	{
		CurrentState = IDLE;
	}
	if(AtStart == true)
	{
		AtStart = true;
		CurrentState = PATROL;
	}

	switch(CurrentState)
	{
	case ATTACK:
		if( (theDestination.x - theEnemyPosition.x) > 50)
		{
			theEnemyPosition.x += AI_ATTACK_MS;
			if(faceRight != true)
			{
				faceRight = true;
				faceLeft = false;
			}
		}
		else if( (theDestination.x - theEnemyPosition.x) < -50)
		{
			theEnemyPosition.x += -AI_ATTACK_MS;
			if(faceLeft != true)
			{
				faceRight = false;
				faceLeft = true;
			}
		}
		//theEnemyPosition.x = theEnemyPosition.x + (theDestination.x - theEnemyPosition.x > 0 ? AI_ATTACK_MS : -AI_ATTACK_MS);
		break;
	case REPEL:
		if( theEnemyPosition.x > theDestination.x )
		{
			if( (theEnemyPosition.x - theDestination.x) < 50)
			{
				theEnemyPosition.x += AI_ATTACK_MS;
				if(theEnemyPosition.x > theDestination.x + 50)
				{
					theEnemyPosition.x = theDestination.x + 50;
				}
				if(faceLeft != true)
				{
					faceLeft = true;
					faceRight = false;
				}
			}
		}
		else if(theEnemyPosition.x < theDestination.x)
		{
			if( (theDestination.x - theEnemyPosition.x) < 50)
			{
				theEnemyPosition.x -= AI_ATTACK_MS;
				if(theEnemyPosition.x < theDestination.x - 50)
				{
					theEnemyPosition.x = theDestination.x - 50;
				}
				if(faceRight != true)
				{
					faceRight = true;
					faceLeft = false;
				}
			}
		}
		/*if( (theDestination.x - theEnemyPosition.x) > 50)
		{
			if(theEnemyPosition.x < theDestination.x + 50)
			{
				theEnemyPosition.x += AI_ATTACK_MS;
				if(faceRight != true)
				{
					faceRight = true;
					faceLeft = false;
				}
			}
			if(theEnemyPosition.x > theDestination.x + 50)
			{
				theEnemyPosition.x = theDestination.x - 50;
			}
		}
		else if( (theDestination.x - theEnemyPosition.x) < 50)
		{
			if(theEnemyPosition.x > theDestination.x + 50)
			{
				theEnemyPosition.x += -AI_ATTACK_MS;
				if(faceLeft != true)
				{
					faceRight = false;
					faceLeft = true;
				}
			}
			if(theEnemyPosition.x < theDestination.x + 50)
			{
				theEnemyPosition.x = theDestination.x + 50;
			}
		}*/
		break;
	case IDLE:
		if( (theEnemyStart.x - theEnemyPosition.x) < 0 )
		{
			theEnemyPosition.x += -AI_PATROL_MS;
			if(theEnemyPosition.x < theEnemyStart.x)
			{
				theEnemyPosition.x = theEnemyStart.x;
			}
			if(faceLeft != true)
			{
				faceRight = false;
				faceLeft = true;
			}
		}
		else if( (theEnemyStart.x - theEnemyPosition.x) > 0 )
		{
			theEnemyPosition.x += AI_PATROL_MS;
			if(theEnemyPosition.x > theEnemyStart.x)
			{
				theEnemyPosition.x = theEnemyStart.x;
			}
			if(faceRight != true)
			{
				faceRight = true;
				faceLeft = false;
			}
		}
		else if( (theEnemyStart.x == theEnemyPosition.x ) )
		{
			AtStart = true;	//Back at start
		}
		break;
	case PATROL:
		if(AtStart == true)	//If back at start
		{
			if( ((theEnemyStart.x + PATROL_OFFSET) - theEnemyPosition.x) > 0 )
			{
				theEnemyPosition.x += AI_PATROL_MS;	//Patrol
				if(theEnemyPosition.x > theEnemyStart.x + PATROL_OFFSET)
				{
					theEnemyPosition.x = (theEnemyStart.x + PATROL_OFFSET);
				}
				if(faceRight != true)
				{
					faceRight = true;
					faceLeft = false;
				}
			}
			else if( theEnemyPosition.x == (theEnemyStart.x + PATROL_OFFSET) )
			{
				AtStart = false;
				CurrentState = IDLE;
			}
		}
		break;
	default:
		break;
	}

	//theEnemyPosition.x = theEnemyPosition.x + (theDestination.x - theEnemyPosition.x > 0 ? 5 : -5 );
	//theEnemyPosition.y = theEnemyPosition.y + (theDestination.y - theEnemyPosition.y > 0 ? 5 : -5 );
}
void CStrategy_Kill::SetDestination(const float x, const float y)
{
	theDestination.x = x;
	theDestination.y = y;
}
void CStrategy_Kill::GetEnemyPosition(float& x, float& y)
{
	x = theEnemyPosition.x;
	y = theEnemyPosition.y;
}

void CStrategy_Kill::GetEnemyDirection(bool &left, bool &right)
{
	left = faceLeft;
	right = faceRight;
}
void CStrategy_Kill::SetEnemyDirection(const bool left,const bool right)
{
	faceLeft = left;
	faceRight = right;
}

void CStrategy_Kill::SetEnemyPosition(const float x, const float y)
{
	theEnemyPosition.x = x;
	theEnemyPosition.y = y;
}
void CStrategy_Kill::SetEnemyStart(const float x, const float y)
{
	theEnemyStart.x = x;
	theEnemyStart.y = y;
}

CStrategy_Kill::CURRENT_STATE CStrategy_Kill::GetState(void)
{
	return CurrentState;
}
void CStrategy_Kill::SetState(CStrategy_Kill::CURRENT_STATE theEnemyState)
{
	CurrentState = theEnemyState;
}