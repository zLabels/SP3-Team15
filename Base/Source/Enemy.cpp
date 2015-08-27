#include "Enemy.h"
#include <iostream>
CEnemy::CEnemy(void)
	: theStrategy(NULL),
	  enemyHealth(100),
	  enemyDamage(5),
	  enemyType(GUARD1),
	  enemy_face_left(false),
	  enemy_face_right(true),
	  DeathAnimation_Right(NULL),
	  WalkAnimation_Right(NULL),
	  WalkAnimation_Left(NULL),
	  mapOffset_X(0), 
	  mapOffset_Y(0),
	  mapFineOffset_X(0),
	  mapFineOffset_Y(0),
	  KNOCKBACK_RANGE(25),
	  ATTACK_RANGE(15625),
	  enemyAttacking(false),
	  enemyAttackCD(0.f),
	  MAX_ATTACK_CD(4.f),
	  enemyPatrolRange(25)
{
}

CEnemy::~CEnemy(void)
{
	if (theStrategy != NULL)
	{
		delete theStrategy;
		theStrategy = NULL;
	}
	while(enemyBulletList.size() > 0)
	{
		CEnemyBullet *bullet = enemyBulletList.back();
		delete bullet;
		enemyBulletList.pop_back();
	}
}

void CEnemy::Init(int pos_x,int pos_y,int hp,int dmg,bool active,ENEMY_TYPE type,int patrolRange)
{
	theENEMYPosition.x = pos_x;
	theENEMYPosition.y = pos_y;
	theENEMYStart.x = pos_x;
	theENEMYStart.y = pos_y;
	
	this->enemyActive = active;
	this->enemyType = type;
	this->enemyHealth = hp;
	this->enemyDamage = dmg;
	this->enemyPatrolRange = patrolRange;
}

void CEnemy::SetPos_x(int pos_x)
{
	theENEMYPosition.x = pos_x;
}
void CEnemy::SetPos_y(int pos_y)
{
	theENEMYPosition.y = pos_y;
}

void CEnemy::SetDestination(const int pos_x, const int pos_y)
{
	theDestination.x = pos_x;
	theDestination.y = pos_y;
	if(theStrategy != NULL)
	{
		theStrategy->SetDestination(theDestination.x, theDestination.y);
	}
}

int CEnemy::GetPos_x(void)
{
	return theENEMYPosition.x;
}
int CEnemy::GetPos_y(void)
{
	return theENEMYPosition.y;
}

int CEnemy::GetDestination_x(void)
{
	return theDestination.x;
}
int CEnemy::GetDestination_y(void)
{
	return theDestination.y;
}

bool CEnemy::getActive(void)
{
	return this->enemyActive;
}
int CEnemy::getHealth(void)
{
	return this->enemyHealth;
}
int CEnemy::getDamage(void)
{
	return this->enemyDamage;
}
float& CEnemy::getAttackCD(void)
{
	return this->enemyAttackCD;
}
CStrategy* CEnemy::getStrategy(void)
{
	return this->theStrategy;
}

vector<CEnemyBullet*>& CEnemy::getEnemyBullet(void)
{
	return enemyBulletList;
}

void CEnemy::EnemyDamaged(int damage,CMap* m_cMap)
{
	this->enemyHealth -= damage;
	if(enemy_face_left)
	{
		if(enemyHealth > 0)
		{
			theENEMYPosition.x += (KNOCKBACK_RANGE);
			if(theENEMYPosition.x < 0 )
			{
				theENEMYPosition.x = 0;
			}
			theStrategy->SetEnemyPosition(theENEMYPosition.x,theENEMYPosition.y);
			if( CheckCollision(m_cMap,false,false,false,true) == false)
			{
				theENEMYPosition.x = tempENEMYPosition.x;
			}
			else if( CheckCollision(m_cMap,false,true,true,false) == true)
			{
				theENEMYPosition.x = tempENEMYPosition.x;
			}
		}
			//Resets enemy attack state
		if(AttackAnimation_Left->m_anim->animActive == true || AttackAnimation_Right->m_anim->animActive == true || enemyAttacking == true)
		{
			AttackAnimation_Left->m_anim->animActive = false;
			AttackAnimation_Left->m_anim->animCurrentFrame = 0;
			AttackAnimation_Left->m_anim->animeCurrentTime = 0.f;
			AttackAnimation_Right->m_anim->animActive = false;
			AttackAnimation_Right->m_anim->animCurrentFrame = 0;
			AttackAnimation_Right->m_anim->animeCurrentTime = 0.f;
			enemyAttacking = false;
		}
	}
	else if(enemy_face_right)
	{
		if(enemyHealth > 0)
		{
			theENEMYPosition.x -= (KNOCKBACK_RANGE);
			if(theENEMYPosition.x < 0 )
			{
				theENEMYPosition.x = 0;
			}
			theStrategy->SetEnemyPosition(theENEMYPosition.x,theENEMYPosition.y);
			if( CheckCollision(m_cMap,false,false,false,true) == false)
			{
				theENEMYPosition.x = tempENEMYPosition.x;
			}
			else if( CheckCollision(m_cMap,false,true,true,false) == true)
			{
				theENEMYPosition.x = tempENEMYPosition.x;
			}
		}
			//Resets enemy attack state
		if(AttackAnimation_Left->m_anim->animActive == true || AttackAnimation_Right->m_anim->animActive == true || enemyAttacking == true)
		{
			AttackAnimation_Left->m_anim->animActive = false;
			AttackAnimation_Left->m_anim->animCurrentFrame = 0;
			AttackAnimation_Left->m_anim->animeCurrentTime = 0.f;
			AttackAnimation_Right->m_anim->animActive = false;
			AttackAnimation_Right->m_anim->animCurrentFrame = 0;
			AttackAnimation_Right->m_anim->animeCurrentTime = 0.f;
			enemyAttacking = false;
		}
	}
}

CEnemyBullet* CEnemy::FetchBullet()
{
	for(std::vector<CEnemyBullet *>::iterator it = enemyBulletList.begin(); it != enemyBulletList.end(); ++it)
	{
		CEnemyBullet *bullet = (CEnemyBullet *)*it;
		if(!bullet->getActive())
		{
			bullet->setActive(true);
			return bullet;
		}
	}
	for(unsigned i = 0; i < 5; ++i)
	{
		CEnemyBullet *bullet = new CEnemyBullet();
		enemyBulletList.push_back(bullet);
	}
	CEnemyBullet *bullet = enemyBulletList.back();
	bullet->setActive(true);
	return bullet;
}

void CEnemy::EnemyAttack(Mesh* ptr,float target_x,float target_y)
{
	if(enemyAttacking == false && enemyAttackCD == 0)
	{
		if( ((theENEMYPosition.x - (theDestination.x)) * (theENEMYPosition.x - (theDestination.x)) + 
			(theENEMYPosition.y - theDestination.y) * (theENEMYPosition.y - theDestination.y)) <= ATTACK_RANGE )
		{	
			enemyAttacking = true;
			enemyAttackCD = MAX_ATTACK_CD;
		}
	}

	if( enemyHealth > 0 && enemyAttacking == true)
	{
		if(enemy_face_right)
		{
			AttackAnimation_Right->m_anim->animActive = true;
			if(AttackAnimation_Right->m_anim->animCurrentFrame == 1)
			{
				CEnemyBullet* bullet = FetchBullet();
				bullet->setData(ptr,theENEMYPosition.x+ 25.f, theENEMYPosition.y+ 40.f,20,1,0,true);
				enemyAttacking = false;
			}
		}
		else if(enemy_face_left)
		{
			AttackAnimation_Left->m_anim->animActive = true;
			if(AttackAnimation_Left->m_anim->animCurrentFrame == 1)
			{			
				CEnemyBullet* bullet = FetchBullet();
				bullet->setData(ptr,theENEMYPosition.x+ 25.f, theENEMYPosition.y + 40.f,20,-1,0,true);
				enemyAttacking = false;
			}
		}
	}
}

int CEnemy::ConstrainEnemyX(const int leftBorder, const int rightBorder, 
								  const int topBorder, const int bottomBorder, 
								  float timeDiff,int mapWidth, int mapHeight,unsigned maplevel)
{
	if(maplevel != 1)
	{
		if (theENEMYPosition.x < leftBorder)
		{
			mapOffset_X =  mapOffset_X - (int) (3.f * timeDiff);
			if (mapOffset_X < 0)
			{
				mapOffset_X = 0;
			}
			else
			{
				theENEMYPosition.x = leftBorder;
			}
		}
		else if (theENEMYPosition.x > rightBorder)
		{
			mapOffset_X =  mapOffset_X + (int) (3.f * timeDiff);
			if (mapOffset_X > mapWidth)	// This must be changed to soft-coded
			{
				mapOffset_X = mapWidth;
			}
			else
			{
				theENEMYPosition.x = rightBorder;
			}
		}
	}

	if (theENEMYPosition.x < 0)
	{
		theENEMYPosition.x = tempENEMYPosition.x;
	}
	else if (mapOffset_X > mapWidth)
	{
		theENEMYPosition.x = tempENEMYPosition.x;
	}
	return mapOffset_X;
}

bool CEnemy::CheckCollision (CMap* m_cMap, bool m_bCheckLeft,bool m_bCheckRight, bool m_bCheckUp, bool m_bCheckDown)
{
	int mapFineOffset_x = mapOffset_X % m_cMap->GetTileSize();
	int checkPosition_X2 = (int) ((mapOffset_X+theENEMYPosition.x + 25.f - mapFineOffset_x) / m_cMap->GetTileSize());
	int checkPosition_X3 = (int) ((mapOffset_X+theENEMYPosition.x - mapFineOffset_x) / m_cMap->GetTileSize());
	int checkPosition_Y2 = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (theENEMYPosition.y + m_cMap->GetTileSize()) / m_cMap->GetTileSize());
	int checkPosition_Y3 = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (theENEMYPosition.y + m_cMap->GetTileSize() + 25.f) / m_cMap->GetTileSize());
	int checkPosition_Y4 = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (theENEMYPosition.y + m_cMap->GetTileSize() + 40.f) / m_cMap->GetTileSize());

	if(m_bCheckRight)
	{
		if (//Border
			(m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X2+2] == TILE_BORDER) ||
			(m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X2+2] == TILE_BORDER) ||
			(m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X2+2] == TILE_BORDER) ||
			//Grass
			(m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X2+2] == TILE_GRASS) ||
			(m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X2+2] == TILE_GRASS) ||
			(m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X2+2] == TILE_GRASS) ||
			//Tile underground
			(m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X2+2] == TILE_UNDERGROUND) ||
			(m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X2+2] == TILE_UNDERGROUND) ||
			(m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X2+2] == TILE_UNDERGROUND))
		{
			return true;
		}
	}

	if(m_bCheckLeft)
	{
		if( //Border
			(m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X3] == TILE_BORDER ) ||
			(m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X3] == TILE_BORDER ) ||
			(m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X3] == TILE_BORDER ) ||
			//Grass
			(m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X3] == TILE_GRASS ) ||
			(m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X3] == TILE_GRASS ) ||
			(m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X3] == TILE_GRASS ) ||
			//Tile underground
			(m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X3] == TILE_UNDERGROUND ) ||
			(m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X3] == TILE_UNDERGROUND ) ||
			(m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X3] == TILE_UNDERGROUND ))
		{
			return true;
		}
	}

	if(m_bCheckUp)
	{
		int checkPosition_X = (int) ((mapOffset_X+theENEMYPosition.x) / m_cMap->GetTileSize());
		int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (theENEMYPosition.y + m_cMap->GetTileSize() + 39.f) / m_cMap->GetTileSize());

		if ( //Border
			 (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_BORDER) ||
		     (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == TILE_BORDER) ||
			 //Grass
			 (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_GRASS) ||
		     (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == TILE_GRASS) ||
			 //Tile underground
			 (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_GRASS) ||
		     (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == TILE_GRASS))
		{
			return true;
		}
	}

	if(m_bCheckDown)
	{
		int checkPosition_X = (int) ((mapOffset_X+theENEMYPosition.x) / m_cMap->GetTileSize());
		int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (theENEMYPosition.y) / m_cMap->GetTileSize());

		if ( (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_GRASS) || 
			(m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+1] == TILE_GRASS) || 
			(m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == TILE_GRASS) ||
			//Tile underground
			(m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_GRASS) || 
			(m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+1] == TILE_GRASS) || 
			(m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == TILE_GRASS))
		{
			return true;
		}

	}

	return false;
}

/********************************************************************************
Enemy Update
********************************************************************************/
void CEnemy::Update(CMap* m_cMap,int mapWidth, int mapHeight,unsigned maplevel,bool heroInvisible)
{
	tempENEMYPosition.x = theENEMYPosition.x;
	tempENEMYPosition.y = theENEMYPosition.y;
	/*if (theStrategy != NULL)
	{
		theStrategy->SetDestination(theDestination.x,theDestination.y);
		theStrategy->Update();
		theStrategy->GetEnemyPosition( (theENEMYPosition.x),
			(theENEMYPosition.y) );
	}*/
	
	if(AttackAnimation_Left->m_anim->animActive == false && AttackAnimation_Right->m_anim->animActive == false)
	{
		theStrategy->SetDestination(theDestination.x,theDestination.y);
		theStrategy->Update(heroInvisible,enemyPatrolRange);
		theStrategy->GetEnemyPosition( (theENEMYPosition.x),(theENEMYPosition.y) );
		theStrategy->GetEnemyDirection( enemy_face_left,enemy_face_right );
	}

	if( CheckCollision(m_cMap,false,false,false,true) == false)
	{
		theENEMYPosition.x = tempENEMYPosition.x;
	}

	//Setting animation positions
	WalkAnimation_Right->m_anim->animPosition.x = theENEMYPosition.x;
	WalkAnimation_Right->m_anim->animPosition.y = theENEMYPosition.y;
	WalkAnimation_Left->m_anim->animPosition.x = theENEMYPosition.x;
	WalkAnimation_Left->m_anim->animPosition.y = theENEMYPosition.y;
	if(DeathAnimation_Right->m_anim->animActive == false)
	{
		DeathAnimation_Right->m_anim->animPosition.x = theENEMYPosition.x;
		DeathAnimation_Right->m_anim->animPosition.y = theENEMYPosition.y;
	}
	if(AttackAnimation_Right->m_anim->animActive == false)
	{
		AttackAnimation_Right->m_anim->animPosition.x = theENEMYPosition.x;
		AttackAnimation_Right->m_anim->animPosition.y = theENEMYPosition.y;
	}
	if(AttackAnimation_Left->m_anim->animActive == false)
	{
		AttackAnimation_Left->m_anim->animPosition.x = theENEMYPosition.x;
		AttackAnimation_Left->m_anim->animPosition.y = theENEMYPosition.y;
	}

	//Death Animation
	if(enemyActive == true && enemyHealth <= 0)
	{
		DeathAnimation_Right->m_anim->animActive = true;
		if(DeathAnimation_Right->m_anim->animCurrentFrame == 5)
		{
			enemyActive = false;
		}
	}
	//Walk animation for right
	if(enemy_face_right == true && DeathAnimation_Right->m_anim->animActive == false
		&& AttackAnimation_Right->m_anim->animActive == false
		&& AttackAnimation_Left->m_anim->animActive == false)
	{
		WalkAnimation_Right->m_anim->animActive = true;
	}
	else
	{
		WalkAnimation_Right->m_anim->animActive = false;
	}

	//Walk animation for left
	if(enemy_face_left == true && DeathAnimation_Right->m_anim->animActive == false  
		&& AttackAnimation_Right->m_anim->animActive == false
		&& AttackAnimation_Left->m_anim->animActive == false)
	{
		WalkAnimation_Left->m_anim->animActive = true;
	}
	else
	{
		WalkAnimation_Left->m_anim->animActive = false;
	}
}
/********************************************************************************
Strategy
********************************************************************************/
void CEnemy::ChangeStrategy(CStrategy* theNewStrategy, bool bDelete)
{
	if (bDelete == true)
	{
		if (theStrategy != NULL)
		{
			delete theStrategy;
			theStrategy = NULL;
		}
	}
	theStrategy = theNewStrategy;
	if (theStrategy != NULL)
	{
		theStrategy->SetEnemyPosition(theENEMYPosition.x, theENEMYPosition.y);

		theStrategy->SetEnemyStart(theENEMYPosition.x,theENEMYPosition.y);
	}
}