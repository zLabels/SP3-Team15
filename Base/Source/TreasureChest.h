#pragma once
#include "goodies.h"
#include "Vector2.h"
class CTreasureChest : public CGoodies
{
public:
	CTreasureChest(void);
	~CTreasureChest(void);
    
    void ChestInit(float posX, float posY,bool active);

    void setPositionX(float);
    float getPositionX();

     void setPositionY(float);
    float getPositionY();

    void SetActive(bool);
    bool getActive();
private:
    Vector2 Position;
    bool active;
};