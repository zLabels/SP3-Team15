#pragma once
#include "Vector2.h"
#include "Map.h"
#include "Strategy.h"
#include "SpriteAnimation.h"
#include "EnemyBullet.h"
#include "Mesh.h"
#include "TileCollision.h"

class CEnemy
{
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

	void Init(int pos_x,int pos_y,int hp,int dmg,bool active,ENEMY_TYPE type,int patrolRange); // Initialise this class instance

	void SetPos_x(int pos_x); // Set position x of the player
	void SetPos_y(int pos_y); // Set position y of the player

	void SetDestination(const int pos_x, const int pos_y);	// Set the destination of this enemy

	int GetPos_x(void); // Get position x of the player
	int GetPos_y(void); // Get position y of the player

	int GetDestination_x(void); // Get the destination of this enemy
	int GetDestination_y(void); // Get the destination of this enemy

	vector<CEnemyBullet*>& getEnemyBullet(void);	//Gets vector of enemy bullets

	void Update(CMap* m_cMap,int mapWidth, int mapHeight,unsigned maplevel,bool heroInvisible); 	// ENEMY Update

	CStrategy* getStrategy(void);	//Get enemy strategy
	void ChangeStrategy(CStrategy* theNewStrategy, bool bDelete=true); 	//Change/Set Enemy strategy

	CEnemyBullet* FetchBullet();	//Fetch enemy bullet from vector

	int ConstrainEnemyX(const int leftBorder, const int rightBorder, 
								  const int topBorder, const int bottomBorder, 
								  float timeDiff,int mapWidth, int mapHeight,unsigned maplevel);	//Constrain Enemy X axis

	bool CheckCollision (CMap* m_cMap, bool m_bCheckLeft,bool m_bCheckRight, bool m_bCheckUp, bool m_bCheckDown);	//Enemy CheckCollision

	bool getActive(void);	//Get enemy active state
	int getHealth(void);	//Get hp
	int getDamage(void);	//Get Damage
	float& getAttackCD(void);	//get Enemy attack CD

	void EnemyAttack(Mesh* ptr,float dir_x,float dir_y);	//Enemy attacking player
	void EnemyDamaged(int damage,CMap* m_cMap);	//Enemy is hit

	ENEMY_TYPE enemyType;	//Type of enemy

	CSpriteAnimation* DeathAnimation_Right;	//Death Animation R
	CSpriteAnimation* DeathAnimation_Left;	//Death Animation L
	CSpriteAnimation* WalkAnimation_Right;	//Walk Animation R
	CSpriteAnimation* WalkAnimation_Left;	//Walk Animation L
	CSpriteAnimation* AttackAnimation_Right;	//Attack Animation R
	CSpriteAnimation* AttackAnimation_Left;		//Attack Animation L
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

	int enemyPatrolRange;	//Enemy range of patrol
	int mapOffset_X, mapOffset_Y;	//Map offset X , Y	for sccrolling
	int mapFineOffset_X, mapFineOffset_Y;	//Map fine offset X , Y for scrolling

	vector<CEnemyBullet*> enemyBulletList;	//Container for enemy bullets
};