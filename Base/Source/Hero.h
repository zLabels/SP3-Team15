#pragma once
#include "Vector2.h"
#include "Map.h"
#include "SpriteAnimation.h"
#include <cmath>

class CHero
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
public:
	/*Enum for Attacks available to Hero*/
	enum ATTACK_TYPE
	{
		ATTACK_1 = 1,
		ATTACK_2,
		SHOCKWAVE,
		MAX_ATTACKS,
	};
	//Hero related functions
	void HeroInit(float posX,float posY);	//Initialize hero positions
	void Update(CMap* m_cMap,int mapWidth, int mapHeight,unsigned maplevel);	//Update hero
	void HeroJump();	//Hero jump function
	void HeroMoveUpDown(const bool mode, const float timeDiff);	//Hero Moving up / down function
	void HeroMoveLeftRight(const bool mode , const float timeDiff);	//Hero Moving left/right function
	int HeroAttack(int enemy_x,int enemy_y, ATTACK_TYPE type);	//Hero attacking
	void HeroDamaged(int enemy_x, int enemy_y);	//Hero Damaged by colliding with enemy
	void HeroDamaged(int Damage);	//Hero Damaged by enemy
	void HeroKnockBack(CMap* m_cMap);	//Hero knocked back

	// Constrain the position of the Hero to within the border
	int ConstrainHeroX(const int leftBorder, const int rightBorder, const int topBorder, const int bottomBorder, 
						float timeDiff,int mapWidth, int mapHeight,unsigned maplevel);	//Constraining hero's X position
	int ConstrainHeroY(const int leftBorder, const int rightBorder, const int topBorder, const int bottomBorder,
						float timeDiff, int screenWidth, int screenHeight,unsigned maplevel);	//Constraining Hero's Y position

	bool CheckCollision (CMap* m_cMap,bool m_bCheckLeft,bool m_bCheckRight, bool m_bCheckUp, bool m_bCheckDown);	//Collision Detection function
	void CollisionResponse(bool m_bCheckLeft,bool m_bCheckRight, bool m_bCheckUp, bool m_bCheckDown);	//Collision	Response function

	float& GetHeroPos_x(void);	//Getting hero X
	float& GetHeroPos_y(void);	//Getting hero Y
	void SetHeroPos_x(float input);	//Setting hero X
	void SetHeroPos_y(float input);	//Setting hero Y
	
	bool& Gethero_inMidAir_Up(void);	//Getting hero in mid air up bool
	bool& Gethero_inMidAir_Down(void);	//Getting hero in mid air down bool

	bool& Gethero_face_left(void);	//Getting herofaceleft bool
	bool& Gethero_face_right(void);	//Getting herofaceright bool

	int& Getjumpspeed(void);	//Getting jumpspeed
	int& Getjumpcount(void);	//Getting jumpcounter

	int& Gethero_HP(void);	//Getting hero hp
	int& Gethero_EP(void);	//Getting hero EP

	float& Gethero_MS(void);	//Getting hero Move speed
	float& Gethero_invulframe(void);	//Getting hero invulnerable frame

	int GetMapOffset_x(void);	//Getting map offset X
	int GetMapOffset_y(void);	//Gettng map offset Y
	void setMapOffset_x(int x);	//Setting map offset x
	void setMapOffset_y(int y);	//Setting map offset y
	int GetMapFineOffset_x(void);	//Get map fine offset X
	int GetMapFineOffset_y(void);	//Get map find offset Y

	static CHero* GetInstance();	//Getting hero instance
	
		//Animations
	CSpriteAnimation* Hero_idle_right;
	CSpriteAnimation* Hero_idle_left;
	CSpriteAnimation* Hero_run_right;
	CSpriteAnimation* Hero_run_left;
	CSpriteAnimation* Hero_attack_1_right;
	CSpriteAnimation* Hero_attack_1_left;
	CSpriteAnimation* Hero_attack_2_right;
	CSpriteAnimation* Hero_attack_2_left;
	CSpriteAnimation* Hero_shockwave_right;
	CSpriteAnimation* Hero_shockwave_left;
	CSpriteAnimation* Hero_jump_right;
	CSpriteAnimation* Hero_jump_left;
private:
	CHero(void);
	~CHero(void);
	//Hero's Info
	Vector2 HeroPos;	//Hero's current position
	Vector2 tempHeroPos;	//Hero's previous frame position
	Vector2 knockbackPos;	//Hero's knockback offset

	bool hero_inMidAir_Up;	//Bool if hero is going up in mid air
	bool hero_inMidAir_Down;	//Bool if hero is going down in mid air
	bool hero_face_left;	//Bool if hero is facing left
	bool hero_face_right;	//Bool if hero is facing right
	bool hero_damaged;	//Hero is damaged
	bool kb_direction;	//Hero knockback direction (false = left, true = right)
	int jumpspeed;	//speed of jump
	int jumpcount;	//Number of jumps
	int MAX_FALLSPEED;	//Maximum speed of falling

	int hero_HP;	//Health points
	int hero_EP;	//Energy points

	float invul_frame;	//Invulnerable to damage for a set amount of time
	float MAX_INVUL;	//Maximum invulnerable time
	float kb_MS;	//Knockback speed
	float hero_MS;	//Movement speed
	int ATTACK_RANGE;	//Hero's attack range
	int ATTACK_DAMAGE;	//Hero's attack damage
	int SHOCKWAVE_DAMAGE;	//Hero's shockwave damage

	static CHero* heroInstance;	//singleton hero instance

	int mapOffset_X, mapOffset_Y;	//Map offset X , Y	for sccrolling
	int mapFineOffset_X, mapFineOffset_Y;	//Map fine offset X , Y for scrolling
};
