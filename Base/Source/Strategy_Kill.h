#pragma once
#include "Strategy.h"
#include "Vector2.h"

class CStrategy_Kill
	: public CStrategy
{
public:
	CStrategy_Kill(void);
	~CStrategy_Kill(void);

	void Update(void);	//Update enemy
	void SetDestination(const float x, const float y);	//Set Enemy Destination
	void GetEnemyPosition(float& x, float& y);	//Get Enemy position
	void SetEnemyPosition(const float x, const float y);	//Set Enemy position
	void SetEnemyStart(const float x, const float y);	//Set Enemy start position

	void GetEnemyDirection(bool &left, bool &right);	//Get enemy direction
	void SetEnemyDirection(const bool left,const bool right);	//Set enemy direction

	/*AI Current state*/
	enum CURRENT_STATE
	{
		IDLE,
		REPEL,
		ATTACK,
		PATROL,
		MAX_ENEMY_STATE,
	};

	CStrategy_Kill::CURRENT_STATE GetState(void);
	void SetState(CStrategy_Kill::CURRENT_STATE theEnemyState);

private:
	float AI_ATTACK_MS;	//Enemy attack speed
	float AI_PATROL_MS;	//Enemy patrol speed

	int PATROL_OFFSET;	//Patrol offset point from start
	bool AtStart;	//Enemy back at start position

	//Enemy AI State
	CStrategy_Kill::CURRENT_STATE CurrentState;
};