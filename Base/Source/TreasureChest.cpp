#include "TreasureChest.h"


CTreasureChest::CTreasureChest(void)
{
    Position.x = 0;
    Position.y = 0;
    active = false;
}

CTreasureChest::~CTreasureChest(void)
{
}

void CTreasureChest::setPositionX(float NewPositionX)
{
    Position.x = NewPositionX;
}
float CTreasureChest::getPositionX()
{
    return Position.x;
}

void CTreasureChest::setPositionY(float NewPositionY)
{
    Position.y = NewPositionY;
}

float CTreasureChest::getPositionY()
{
    return Position.y;
}

void CTreasureChest::SetActive(bool NewActive)
{
    active = NewActive;
}

bool CTreasureChest::getActive()
{
    return active;
}