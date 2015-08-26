#include "TileCollision.h"

bool SingleTileCollisionCheck(CMap* m_cMap, bool m_bCheckLeft,bool m_bCheckRight, bool m_bCheckUp, bool m_bCheckDown,float pos_X,float pos_Y,int mapOffset_X)
{
    int mapFineOffset_x = mapOffset_X % m_cMap->GetTileSize();
    int checkPosition_X2 = (int) ((mapOffset_X+pos_X - mapFineOffset_x) / m_cMap->GetTileSize());
    int checkPosition_X3 = (int) ((mapOffset_X+pos_X - mapFineOffset_x) / m_cMap->GetTileSize());
    int checkPosition_Y2 = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (pos_Y + m_cMap->GetTileSize()) / m_cMap->GetTileSize());

    if(m_bCheckRight)
    {
		for(unsigned i = 1; i < TILE_MAX_COLLISION; ++i)
		{
			if ( m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X2] == i )
			{
				return true;
			}
		}
    }

    if(m_bCheckLeft)
    {
		for(unsigned i = 1; i < TILE_MAX_COLLISION; ++i)
		{
			if ( m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X3] == i )
			{
				return true;
			}
		}
    }

    if(m_bCheckUp)
    {
        int checkPosition_X = (int) ((mapOffset_X+ pos_X) / m_cMap->GetTileSize());
        int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (pos_Y + m_cMap->GetTileSize()) / m_cMap->GetTileSize());

		for(unsigned i = 1; i < TILE_MAX_COLLISION; ++i)
		{
			if ( m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == i )
			{
				return true;
			}
		}   
    }

    if(m_bCheckDown)
    {
        int checkPosition_X = (int) ((mapOffset_X+pos_X) / m_cMap->GetTileSize());
        int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (pos_Y) / m_cMap->GetTileSize());

		for(unsigned i = 1; i < TILE_MAX_COLLISION; ++i)
		{
			if ( m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == i )
			{
				return true;
			}
		}
    }

    if(pos_X < 0 )
    {
        return true;
    }

    return false;
}

bool HeroCollisionCheck( CMap* m_cMap,bool m_bCheckLeft,bool m_bCheckRight, bool m_bCheckUp, bool m_bCheckDown,float pos_X,float pos_Y,int mapOffset_X,int jumpspeed )
{
    int mapFineOffset_x = mapOffset_X % m_cMap->GetTileSize();
    int checkPosition_X2 = (int) ((mapOffset_X + pos_X + 25.f - mapFineOffset_x) / m_cMap->GetTileSize());
    int checkPosition_X3 = (int) ((mapOffset_X + pos_X - mapFineOffset_x) / m_cMap->GetTileSize());
    int checkPosition_Y2 = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (pos_Y + m_cMap->GetTileSize()) / m_cMap->GetTileSize());
    int checkPosition_Y3 = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (pos_Y + m_cMap->GetTileSize() + 25.f) / m_cMap->GetTileSize());
    int checkPosition_Y4 = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (pos_Y + m_cMap->GetTileSize() + 40.f) / m_cMap->GetTileSize());

    if(m_bCheckRight)
    {
		for(unsigned i = 1; i < TILE_MAX_COLLISION; ++i)
		{
			if( i != TILE_DOOR )
			{
				if ( (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X2+2] == i) ||
					(m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X2+2] == i) ||
					(m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X2+2] == i) )
				{
					return true;
				}
			}
			else if(i == TILE_DOOR)
			{
				if ( (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X2+1] == i) ||
					(m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X2+1] == i) ||
					(m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X2+1] == i) )
				{
					return true;
				}
			}
		}
    }

    if(m_bCheckLeft)
    {
		for(unsigned i = 1; i < TILE_MAX_COLLISION; ++i)
		{
			if (  (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X3] == i ) ||
				(m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X3] == i ) ||
				(m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X3] == i ) )
			{
				return true;
			}
		}
    }

    if(m_bCheckUp)
    {
        int checkPosition_X = (int) ((mapOffset_X + pos_X) / m_cMap->GetTileSize());
        int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (pos_Y + m_cMap->GetTileSize() + jumpspeed + 39.f) / m_cMap->GetTileSize());

		for(unsigned i = 1; i < TILE_MAX_COLLISION; ++i)
		{
			if (  (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == i) ||
				  (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == i) )
			{
				return true;
			}
		}
    }

    if(m_bCheckDown)
    {
        int checkPosition_X = (int) ((mapOffset_X + pos_X) / m_cMap->GetTileSize());
        int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (pos_Y - jumpspeed) / m_cMap->GetTileSize());

		for(unsigned i = 1; i < TILE_MAX_COLLISION; ++i)
		{
			if (  (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == i) || 
				  (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+1] == i) || 
				  (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == i) )
			{
				return true;
			}
		}
    }

    return false;
}

bool HeroTileCheck(CMap* m_cMap,TILE_TYPE type,bool m_bCheckLeft,bool m_bCheckRight, bool m_bCheckUp, bool m_bCheckDown,float pos_X,float pos_Y,int mapOffset_X,int jumpspeed )
{
    int mapFineOffset_x = mapOffset_X % m_cMap->GetTileSize();
    int checkPosition_X2 = (int) ((mapOffset_X + pos_X + 25.f - mapFineOffset_x) / m_cMap->GetTileSize());
    int checkPosition_X3 = (int) ((mapOffset_X + pos_X - mapFineOffset_x) / m_cMap->GetTileSize());
    int checkPosition_Y2 = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (pos_Y + m_cMap->GetTileSize()) / m_cMap->GetTileSize());
    int checkPosition_Y3 = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (pos_Y + m_cMap->GetTileSize() + 25.f) / m_cMap->GetTileSize());
    int checkPosition_Y4 = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (pos_Y + m_cMap->GetTileSize() + 40.f) / m_cMap->GetTileSize());

    if(m_bCheckRight)
    {
        if (//Border
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X2+2] == type) ||
            (m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X2+2] == type) ||
            (m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X2+2] == type) ||
			(m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X2+1] == type) ||
            (m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X2+1] == type) ||
            (m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X2+1] == type))
        {
            return true;
        }
    }

    if(m_bCheckLeft)
    {
        if( //Border
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X3] == type ) ||
            (m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X3] == type ) ||
            (m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X3] == type ))
        {
            return true;
        }
    }

    if(m_bCheckUp)
    {
        int checkPosition_X = (int) ((mapOffset_X + pos_X) / m_cMap->GetTileSize());
        int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (pos_Y + m_cMap->GetTileSize() + jumpspeed + 39.f) / m_cMap->GetTileSize());

        if ( //Border
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == type) ||
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == type))
        {
            return true;
        }
    }

    if(m_bCheckDown)
    {
        int checkPosition_X = (int) ((mapOffset_X + pos_X) / m_cMap->GetTileSize());
        int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (pos_Y - jumpspeed) / m_cMap->GetTileSize());

        if ( (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == type)   || 
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+1] == type) || 
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == type))
        {
            return true;
        }

    }

    return false;
}

bool TileObjectCollisionCheck(CMap* m_cMap,TILE_TYPE type,bool m_bCheckLeft,bool m_bCheckRight, bool m_bCheckUp, bool m_bCheckDown,float pos_X,float pos_Y,int mapOffset_X)
{
    int mapFineOffset_x = mapOffset_X % m_cMap->GetTileSize();
    int checkPosition_X2 = (int) ((mapOffset_X + pos_X + 25.f - mapFineOffset_x) / m_cMap->GetTileSize());
    int checkPosition_X3 = (int) ((mapOffset_X + pos_X - mapFineOffset_x) / m_cMap->GetTileSize());
    int checkPosition_Y2 = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (pos_Y + m_cMap->GetTileSize()) / m_cMap->GetTileSize());
    int checkPosition_Y3 = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (pos_Y + m_cMap->GetTileSize() + 25.f) / m_cMap->GetTileSize());
    int checkPosition_Y4 = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (pos_Y + m_cMap->GetTileSize() + 40.f) / m_cMap->GetTileSize());

    if(m_bCheckRight)
    {
        if (//Border
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X2+2] == type) ||
            (m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X2+2] == type) ||
            (m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X2+2] == type))
        {
            return true;
        }
    }

    if(m_bCheckLeft)
    {
        if( //Border
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X3] == type ) ||
            (m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X3] == type ) ||
            (m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X3] == type ))
        {
            return true;
        }
    }

    if(m_bCheckUp)
    {
        int checkPosition_X = (int) ((mapOffset_X + pos_X) / m_cMap->GetTileSize());
        int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (pos_Y + m_cMap->GetTileSize() + 39.f) / m_cMap->GetTileSize());

        if ( //Border
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == type) ||
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == type))
        {
            return true;
        }
    }

    if(m_bCheckDown)
    {
        int checkPosition_X = (int) ((mapOffset_X + pos_X) / m_cMap->GetTileSize());
        int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (pos_Y ) / m_cMap->GetTileSize());

        if ( (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == type)   || 
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+1] == type) || 
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == type))
        {
            return true;
        }

    }

    return false;
}