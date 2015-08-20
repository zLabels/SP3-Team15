#ifndef TILE_COLLISION
#define TILE_COLLISION
#include "Map.h"

/*Enum for tiles in game*/
enum TILE_TYPE
{
	TILE_GRASS = 1,
	TILE_UNDERGROUND,
	TILE_BORDER,
	TILE_CAVE,
    TILE_DOOR = 6,
	MAX_TILE,
};

bool SingleTileCollisionCheck(CMap* m_cMap, bool m_bCheckLeft,bool m_bCheckRight, bool m_bCheckUp, bool m_bCheckDown,float pos_X,float pos_Y,int mapOffset_X);

bool HeroCollisionCheck( CMap* m_cMap,bool m_bCheckLeft,bool m_bCheckRight, bool m_bCheckUp, bool m_bCheckDown,float pos_X,float pos_Y,int mapOffset_X,int jumpspeed );

#endif