#pragma once
#include "goodies.h"
#include "Vector2.h"
#include "SpriteAnimation.h"
class CTreasureChest : public CGoodies
{
    enum POWERUP_TYPE
    {
        POWERUP_HEALTH = 1,
        POWERUP_SCORE,
        POWERUP_SHURIKEN,
        MAX_POWERUP
    };

public:
	CTreasureChest(void);
	~CTreasureChest(void);
    
    void ChestInit(float posX, float posY,int type,bool active);

    void setPositionX(float);
    float getPositionX();

     void setPositionY(float);
    float getPositionY();

    void SetActive(bool);
    bool getActive();


    CSpriteAnimation* Powerup_Animation;
private:
    Vector2 Position;
    bool active;
    int type;
};