/******************************************************************************/
/*!
\file	CHero.h
\author Princeton Chew, Tan Jun Yan
\par	email: 141991A@mymail.nyp.edu.sg
\brief
Player Class
*/
/******************************************************************************/
#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Map.h"
#include "SpriteAnimation.h"
#include "Inventory.h"
#include "Mesh.h"
#include <cmath>

/******************************************************************************/
/*!
		Class CHero
\brief
Defines the variables and functions required for the player.

*/
/******************************************************************************/
class CHero
{
public:
	/*Enum for Attacks available to Hero*/
	enum ATTACK_TYPE
	{
		ATTACK_1 = 1,
		ATTACK_2,
		SHOCKWAVE,
		MAX_ATTACKS,
	};

	void HeroInit(float posX,float posY);	//Initialize hero positions

	void Update(CMap* m_cMap,int mapWidth, int mapHeight,unsigned maplevel);	//Update hero

	void HeroJump();	//Hero jump function
	void HeroMoveUpDown(const bool mode, const float timeDiff);	//Hero Moving up / down function
	void HeroMoveLeftRight(const bool mode , const float timeDiff);	//Hero Moving left/right function
	bool HeroGrapple(CMap* m_cMap,Vector3 direction,Vector3 hookPosition);	//Hero moving towards grapple direction

	void HeroThrowShuriken(Mesh* ptr,float target_x,float target_y);	//Hero using shuriken
	void HeroUseGrapple(Mesh* ptr,float target_x,float target_y);	//Hero using grappling hook

	void HeroDamaged(int enemy_x, int enemy_y);	//Hero Damaged by colliding with enemy
	void HeroDamaged(int Damage);	//Hero Damaged by enemy
	void HeroKnockBack(CMap* m_cMap);	//Hero knocked back
	void HeroRegen(double dt);	//Regenerates hero EP

	// Constrain the position of the Hero to within the border
	int ConstrainHeroX(const int leftBorder, const int rightBorder, const int topBorder, const int bottomBorder, 
		float timeDiff,int mapWidth, int mapHeight,unsigned maplevel);	//Constraining hero's X position
	int ConstrainHeroY(const int leftBorder, const int rightBorder, const int topBorder, const int bottomBorder,
		float timeDiff, int screenWidth, int screenHeight,unsigned maplevel);	//Constraining Hero's Y position

	void CollisionResponse(bool m_bCheckLeft,bool m_bCheckRight, bool m_bCheckUp, bool m_bCheckDown);	//Collision	Response function

	CInventory& getInventory(void);	//Getting hero's inventory

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
	float& Gethero_EP(void);	//Getting hero EP

	float& Gethero_MS(void);	//Getting hero Move speed
	float& Gethero_invulframe(void);	//Getting hero invulnerable frame

	int GetMapOffset_x(void);	//Getting map offset X
	int GetMapOffset_y(void);	//Gettng map offset Y
	void setMapOffset_x(int x);	//Setting map offset x
	void setMapOffset_y(int y);	//Setting map offset y
	int GetMapFineOffset_x(void);	//Get map fine offset X
	int GetMapFineOffset_y(void);	//Get map find offset Y

	//---Physics Related---//
	void Hero_Acceleration(bool LeftOrRight,const float timeDiff);	//Function for accelerating hero
	void setHero_Velocity(float);	//Setting hero velocity
	float getHero_Velocity(void);	//Getting hero velocity
	void setHero_Deceleration(bool,float);	//Setting hero deceleration

	//---Tile Object Interaction---//
	void setHero_Health(int);	//Setting hero health
	void setHero_Invi(bool NewInvi);	//Setting hero invis status
	bool getHero_Invi();	//Getting hero invis status
	int getHero_Score();	//Getting hero score
	void setHero_Score(int);	//Setting hero score
	void setHero_EP(int);	//Setting hero energy points

	static CHero* GetInstance();	//Getting hero instance

	//Animations
	CSpriteAnimation* Hero_idle_right;
	CSpriteAnimation* Hero_idle_left;
	CSpriteAnimation* Hero_run_right;
	CSpriteAnimation* Hero_run_left;
	CSpriteAnimation* Hero_jump_right;
	CSpriteAnimation* Hero_jump_left;
private:
	CHero(void);	//Default constructor
	~CHero(void);	//Default destructor

	Vector2 HeroPos;	//Hero's current position
	Vector2 tempHeroPos;	//Hero's previous frame position
	Vector2 knockbackPos;	//Hero's knockback offset

	bool hero_inMidAir_Up;	//Bool if hero is going up in mid air
	bool hero_inMidAir_Down;	//Bool if hero is going down in mid air
	bool hero_face_left;	//Bool if hero is facing left
	bool hero_face_right;	//Bool if hero is facing right
	bool hero_damaged;	//Hero is damaged
	bool kb_direction;	//Hero knockback direction (false = left, true = right)
	bool hero_grappling;	//Hero is grappling
	bool hero_invisible; //Hero is invisible
	int jumpspeed;	//speed of jump
	int jumpcount;	//Number of jumps
	int MAX_FALLSPEED;	//Maximum speed of falling

	int hero_HP;	//Health points
	float hero_EP;	//Energy points
	int MAX_EP;	//Maximum energy points
	float Regen_speed;	//Hero ep regen speed
	int hero_SCORE; //Score Points

	float invul_frame;	//Invulnerable to damage for a set amount of time
	float MAX_INVUL;	//Maximum invulnerable time
	float kb_MS;	//Knockback speed
	float hero_MS;	//Movement speed

	static CHero* heroInstance;	//singleton hero instance

	int mapOffset_X, mapOffset_Y;	//Map offset X , Y	for sccrolling
	int mapFineOffset_X, mapFineOffset_Y;	//Map fine offset X , Y for scrolling

	CInventory hero_invent;	//Inventory class for hero inventory

	//---Physics---//
	float f_Current_Acceleration; // Acceleration of the character
	float hero_mass;              // Player's mass
	float hero_maxspeed;          // Player's Max speed that it can achieve
	float hero_velocity;          // Player's Current Velocity
	float f_force;                // Force
};

