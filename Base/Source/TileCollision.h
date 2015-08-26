#ifndef TILE_COLLISION
#define TILE_COLLISION
#include "Map.h"

/*Enum for tiles in game*/
enum TILE_TYPE
{
    //---STUFF THAT NEEDS COLLISION---
    TILE_GRASS = 1,
    TILE_UNDERGROUND,
    TILE_BORDER,
    TILE_CAVE,
    TILE_DUMMY,
    TILE_DOOR,
    TILE_LASER_SWITCH,
    TILE_OBJECTIVE,
    TILE_LASER_SHOOTER_UP,
    TILE_LASER_SHOOTER_DOWN,
    TILE_LASER_SHOOTER_LEFT,
    TILE_LASER_SHOOTER_RIGHT,

    //---STUFF THAT DOESN'T NEED COLLISION---
    TILE_LASER = 25,
    TILE_HEALTH,
    TILE_SCORE,
    TILE_SHURIKEN,
    MAX_TILE,
};

bool SingleTileCollisionCheck(CMap* m_cMap, bool m_bCheckLeft,bool m_bCheckRight, bool m_bCheckUp, bool m_bCheckDown,float pos_X,float pos_Y,int mapOffset_X);

bool HeroCollisionCheck( CMap* m_cMap,bool m_bCheckLeft,bool m_bCheckRight, bool m_bCheckUp, bool m_bCheckDown,float pos_X,float pos_Y,int mapOffset_X,int jumpspeed );

bool HeroTileCheck(CMap* m_cMap,TILE_TYPE type,bool m_bCheckLeft,bool m_bCheckRight, bool m_bCheckUp, bool m_bCheckDown,float pos_X,float pos_Y,int mapOffset_X,int jumpspeed );

bool TileObjectCollisionCheck(CMap* m_cMap,TILE_TYPE type,bool m_bCheckLeft,bool m_bCheckRight, bool m_bCheckUp, bool m_bCheckDown,float pos_X,float pos_Y,int mapOffset_X);

#endif