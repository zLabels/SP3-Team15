#pragma once
#include "goodies.h"
#include "Vector2.h"
#include "SpriteAnimation.h"
class CTreasureChest : public CGoodies
{
public:
	CTreasureChest(void);
	~CTreasureChest(void);

	//Enum for type of power ups available in game
	enum POWERUP_TYPE
	{
		POWERUP_HEALTH = 1,
		POWERUP_SCORE,
		POWERUP_ENERGY,
		POWERUP_SHURIKEN,
		MAX_POWERUP
	};

	void ChestInit(float posX, float posY,int type,bool active);	//Initialize this class variables

	void setPositionX(float);	//Setting power up position x
	float getPositionX();	//Getting power up position x

	void setPositionY(float);	//Setting power up position y
	float getPositionY();	//Getting power up position y

	void SetActive(bool);	//Setting active status of power up
	bool getActive();	//Getting active status of power up

	int getType();	//Getting type of power up

	CSpriteAnimation* Powerup_Animation;	//Sprite animation pointer for power up
private:
	Vector2 Position;	//Vector2 for x and y position of power up
	bool active;	//bool to store active status of power up
	int type;	//int to store type of power up
};