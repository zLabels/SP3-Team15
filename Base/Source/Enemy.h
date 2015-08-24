#pragma once
#include "Vector2.h"
#include "Map.h"
#include "Strategy.h"
#include "SpriteAnimation.h"

class CEnemy
{
	/*Enum for tiles in game*/
	enum TILE_TYPE
	{
		TILE_GRASS = 1,
		TILE_UNDERGROUND,
		TILE_BORDER,
		TILE_CAVE,
		MAX_TILE,
	};
	// friend class CStrategy;
public:
	CEnemy(void);
	~CEnemy(void);

	enum ENEMY_TYPE
	{
		GUARD1,
		GHOST,
		MAX_ENEMY
	};
	
	// Initialise this class instance
	void Init(int pos_x,int pos_y,int hp,int dmg,bool active,ENEMY_TYPE type);
	// Set position x of the player
	void SetPos_x(int pos_x);
	// Set position y of the player
	void SetPos_y(int pos_y);
	// Set the destination of this enemy
	void SetDestination(const int pos_x, const int pos_y);
	// Get position x of the player
	int GetPos_x(void);
	// Get position y of the player
	int GetPos_y(void);
	// Set the destination of this enemy
	int GetDestination_x(void);
	// Set the destination of this enemy
	int GetDestination_y(void);
	// ENEMY Update
	void Update(CMap* m_cMap,int mapWidth, int mapHeight,unsigned maplevel);
	// Strategy
	void ChangeStrategy(CStrategy* theNewStrategy, bool bDelete=true);

	int ConstrainEnemyX(const int leftBorder, const int rightBorder, 
								  const int topBorder, const int bottomBorder, 
								  float timeDiff,int mapWidth, int mapHeight,unsigned maplevel);

	bool CheckCollision (CMap* m_cMap, bool m_bCheckLeft,bool m_bCheckRight, bool m_bCheckUp, bool m_bCheckDown);

	bool getActive(void);	//Get enemy active state
	int getHealth(void);	//Get hp
	int getDamage(void);	//Get Damage
	float& getAttackCD(void);	//get Enemy attack CD

	int EnemyAttack();	//Enemy attacking player
	void EnemyDamaged(int damage,CMap* m_cMap);	//Enemy is hit

	ENEMY_TYPE enemyType;
	CSpriteAnimation* DeathAnimation_Right;
	CSpriteAnimation* WalkAnimation_Right;
	CSpriteAnimation* WalkAnimation_Left;
	CSpriteAnimation* AttackAnimation_Right;
	CSpriteAnimation* AttackAnimation_Left;
private:
	// ENEMY's information
	Vector2 tempENEMYPosition;
	Vector2 theENEMYPosition;
	Vector2 theENEMYStart;

	// The Destination is the position of the Hero
	Vector2 theDestination;
	CStrategy* theStrategy;
	
	float enemyAttackCD;	//Enemy attack cool down
	float MAX_ATTACK_CD;	//maximum attack cool down
	bool enemyAttacking;	//Enemy using an attack
	bool enemyActive;	//Enemy Active

	int enemyHealth;	//Enemy Health
	int enemyDamage;	//Enemy damage

	bool enemy_face_left;	//Enemy facing left 
	bool enemy_face_right;	//Enemy facing right

	int KNOCKBACK_RANGE;	//Enemy knockback range
	int ATTACK_RANGE;	//Enemy Attacking range

	int mapOffset_X, mapOffset_Y;	//Map offset X , Y	for sccrolling
	int mapFineOffset_X, mapFineOffset_Y;	//Map fine offset X , Y for scrolling
};