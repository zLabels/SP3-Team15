#include "TileCollision.h"

bool SingleTileCollisionCheck(CMap* m_cMap, bool m_bCheckLeft,bool m_bCheckRight, bool m_bCheckUp, bool m_bCheckDown,float pos_X,float pos_Y,int mapOffset_X)
{
    int mapFineOffset_x = mapOffset_X % m_cMap->GetTileSize();
    int checkPosition_X2 = (int) ((mapOffset_X+pos_X - mapFineOffset_x) / m_cMap->GetTileSize());
    int checkPosition_X3 = (int) ((mapOffset_X+pos_X - mapFineOffset_x) / m_cMap->GetTileSize());
    int checkPosition_Y2 = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (pos_Y + m_cMap->GetTileSize()) / m_cMap->GetTileSize());

    if(m_bCheckRight)
    {
        if (//Border
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X2] == TILE_BORDER) ||
            //Grass
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X2] == TILE_GRASS) ||
            //Door
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X2] == TILE_DOOR) ||
            //laser shooter up
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X2] == TILE_LASER_SHOOTER_UP) ||
            //laser shooter down
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X2] == TILE_LASER_SHOOTER_DOWN) ||
            //laser shooter left
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X2] == TILE_LASER_SHOOTER_LEFT) ||
            //laser shooter right
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X2] == TILE_LASER_SHOOTER_RIGHT) ||
            //Tile underground
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X2] == TILE_UNDERGROUND))
        {
            return true;
        }
    }

    if(m_bCheckLeft)
    {
        if( //Border
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X3] == TILE_BORDER ) ||
            //Grass
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X3] == TILE_GRASS ) ||
            //DOOR
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X3] == TILE_DOOR ) ||
            //Laser
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X3] == TILE_LASER_SWITCH ) ||
            //Laser shooter up
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X3] == TILE_LASER_SHOOTER_UP) ||
            //Laser shooter down
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X3] == TILE_LASER_SHOOTER_DOWN) ||
            //Laser shooter left
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X3] == TILE_LASER_SHOOTER_LEFT) ||
            //Laser shooter right
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X3] == TILE_LASER_SHOOTER_RIGHT) ||
            //Tile underground
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X3] == TILE_UNDERGROUND ) )
        {
            return true;
        }
    }

    if(m_bCheckUp)
    {
        int checkPosition_X = (int) ((mapOffset_X+ pos_X) / m_cMap->GetTileSize());
        int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (pos_Y + m_cMap->GetTileSize()) / m_cMap->GetTileSize());

        if ( //Border
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_BORDER) ||
            //Grass
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_GRASS) ||
            //DOOR
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_DOOR) ||
            //laser shooter up
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_LASER_SHOOTER_UP) ||
            //laser shooter down
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_LASER_SHOOTER_DOWN) ||
            //laser shooter left
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_LASER_SHOOTER_LEFT) ||
            //laser shooter right
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_LASER_SHOOTER_RIGHT) ||
            //Tile underground
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_GRASS) )
        {
            return true;
        }
    }

    if(m_bCheckDown)
    {
        int checkPosition_X = (int) ((mapOffset_X+pos_X) / m_cMap->GetTileSize());
        int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (pos_Y) / m_cMap->GetTileSize());

        if ( (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_GRASS) || 
            //DOOR
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_DOOR) ||
            //Laser shooter up
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_LASER_SHOOTER_UP) ||
            //Laser shooter down
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_LASER_SHOOTER_DOWN) ||
            //Laser shooter left
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_LASER_SHOOTER_LEFT) ||
            //Laser shooter right
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_LASER_SHOOTER_RIGHT) ||
            //Tile underground
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_GRASS))
        {
            return true;
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
        if (//Border
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X2+2] == TILE_BORDER) ||
            (m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X2+2] == TILE_BORDER) ||
            (m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X2+2] == TILE_BORDER) ||
            //Grass
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X2+2] == TILE_GRASS) ||
            (m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X2+2] == TILE_GRASS) ||
            (m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X2+2] == TILE_GRASS) ||
            //Door
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X2+1] == TILE_DOOR) ||
            (m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X2+1] == TILE_DOOR) ||
            (m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X2+1] == TILE_DOOR) ||
            //Laser Shooter up
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X2+1] == TILE_LASER_SHOOTER_UP) ||
            (m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X2+1] == TILE_LASER_SHOOTER_UP) ||
            (m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X2+1] == TILE_LASER_SHOOTER_UP) ||
            //Laser Shooter down
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X2+1] == TILE_LASER_SHOOTER_DOWN) ||
            (m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X2+1] == TILE_LASER_SHOOTER_DOWN) ||
            (m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X2+1] == TILE_LASER_SHOOTER_DOWN) ||
            //Laser Shooter left
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X2+1] == TILE_LASER_SHOOTER_LEFT) ||
            (m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X2+1] == TILE_LASER_SHOOTER_LEFT) ||
            (m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X2+1] == TILE_LASER_SHOOTER_LEFT) ||
            //Laser Shooter right
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X2+1] == TILE_LASER_SHOOTER_RIGHT) ||
            (m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X2+1] == TILE_LASER_SHOOTER_RIGHT) ||
            (m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X2+1] == TILE_LASER_SHOOTER_RIGHT) ||
            //Tile underground
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X2+2] == TILE_UNDERGROUND) ||
            (m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X2+2] == TILE_UNDERGROUND) ||
            (m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X2+2] == TILE_UNDERGROUND))
        {
            return true;
        }
    }

    if(m_bCheckLeft)
    {
        if( //Border
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X3] == TILE_BORDER ) ||
            (m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X3] == TILE_BORDER ) ||
            (m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X3] == TILE_BORDER ) ||
            //Grass
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X3] == TILE_GRASS ) ||
            (m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X3] == TILE_GRASS ) ||
            (m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X3] == TILE_GRASS ) ||
            //Door
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X3] == TILE_DOOR ) ||
            (m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X3] == TILE_DOOR ) ||
            (m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X3] == TILE_DOOR ) ||
            //Laser Switch
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X3] == TILE_LASER_SWITCH ) ||
            (m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X3] == TILE_LASER_SWITCH ) ||
            (m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X3] == TILE_LASER_SWITCH ) ||
            //Laser Shooter up
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X3] == TILE_LASER_SHOOTER_UP) ||
            (m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X3] == TILE_LASER_SHOOTER_UP) ||
            (m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X3] == TILE_LASER_SHOOTER_UP) ||
            //Laser Shooter down
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X3] == TILE_LASER_SHOOTER_DOWN) ||
            (m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X3] == TILE_LASER_SHOOTER_DOWN) ||
            (m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X3] == TILE_LASER_SHOOTER_DOWN) ||
            //Laser Shooter left
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X3] == TILE_LASER_SHOOTER_LEFT) ||
            (m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X3] == TILE_LASER_SHOOTER_LEFT) ||
            (m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X3] == TILE_LASER_SHOOTER_LEFT) ||
            //Laser Shooter right
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X3] == TILE_LASER_SHOOTER_RIGHT) ||
            (m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X3] == TILE_LASER_SHOOTER_RIGHT) ||
            (m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X3] == TILE_LASER_SHOOTER_RIGHT) ||
            //Tile underground
            (m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X3] == TILE_UNDERGROUND ) ||
            (m_cMap->theScreenMap[checkPosition_Y3][checkPosition_X3] == TILE_UNDERGROUND ) ||
            (m_cMap->theScreenMap[checkPosition_Y4][checkPosition_X3] == TILE_UNDERGROUND ))
        {
            return true;
        }
    }

    if(m_bCheckUp)
    {
        int checkPosition_X = (int) ((mapOffset_X + pos_X) / m_cMap->GetTileSize());
        int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (pos_Y + m_cMap->GetTileSize() + jumpspeed + 39.f) / m_cMap->GetTileSize());

        if ( //Border
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_BORDER) ||
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == TILE_BORDER) ||
            //Grass
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_GRASS) ||
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == TILE_GRASS) ||
            //Door
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_DOOR) ||
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == TILE_DOOR) ||
            //Laser Switch
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_LASER_SWITCH) ||
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == TILE_LASER_SWITCH) ||
            //Laser Shooter up
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_LASER_SHOOTER_UP) ||
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == TILE_LASER_SHOOTER_UP) ||
            //Laser Shooter down
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_LASER_SHOOTER_DOWN) ||
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == TILE_LASER_SHOOTER_DOWN) ||
            //Laser Shooter left
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_LASER_SHOOTER_LEFT) ||
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == TILE_LASER_SHOOTER_LEFT) ||
            //Laser Shooter right
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_LASER_SHOOTER_RIGHT) ||
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == TILE_LASER_SHOOTER_RIGHT) ||
            //Tile underground
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_GRASS) ||
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == TILE_GRASS))
        {
            return true;
        }
    }

    if(m_bCheckDown)
    {
        int checkPosition_X = (int) ((mapOffset_X + pos_X) / m_cMap->GetTileSize());
        int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (pos_Y - jumpspeed) / m_cMap->GetTileSize());

        if ( (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_GRASS) || 
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+1] == TILE_GRASS) || 
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == TILE_GRASS) ||
            //Door
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_DOOR) || 
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+1] == TILE_DOOR) || 
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == TILE_DOOR) ||
            //Laser Switch
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_LASER_SWITCH) || 
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+1] == TILE_LASER_SWITCH) || 
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == TILE_LASER_SWITCH) ||
            //Laser Shooter UP
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_LASER_SHOOTER_UP) || 
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+1] == TILE_LASER_SHOOTER_UP) || 
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == TILE_LASER_SHOOTER_UP) ||
            //Laser Shooter DOWN
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_LASER_SHOOTER_DOWN) || 
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+1] == TILE_LASER_SHOOTER_DOWN) || 
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == TILE_LASER_SHOOTER_DOWN) ||
            //Laser Shooter LEFT
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_LASER_SHOOTER_LEFT) || 
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+1] == TILE_LASER_SHOOTER_LEFT) || 
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == TILE_LASER_SHOOTER_LEFT) ||
            //Laser Shooter RIGHT
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_LASER_SHOOTER_RIGHT) || 
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+1] == TILE_LASER_SHOOTER_RIGHT) || 
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == TILE_LASER_SHOOTER_RIGHT) ||
            //Tile underground
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_GRASS) || 
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+1] == TILE_GRASS) || 
            (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == TILE_GRASS))
        {
            return true;
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