#pragma once
#include "goodies.h"
#include "Vector2.h"
#include "SpriteAnimation.h"
class CTreasureChest : public CGoodies
{
    

public:
	CTreasureChest(void);
	~CTreasureChest(void);
    
    enum POWERUP_TYPE
    {
        POWERUP_HEALTH = 1,
        POWERUP_SCORE,
        POWERUP_ENERGY,
        POWERUP_SHURIKEN,
        MAX_POWERUP
    };

    void ChestInit(float posX, float posY,int type,bool active);

    void setPositionX(float);
    float getPositionX();

     void setPositionY(float);
    float getPositionY();

    void SetActive(bool);
    bool getActive();

    int getType();

    CSpriteAnimation* Powerup_Animation;
private:
    Vector2 Position;
    bool active;
    int type;
};