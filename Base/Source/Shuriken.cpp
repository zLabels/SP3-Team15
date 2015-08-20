#include "Shuriken.h"

CShuriken::CShuriken(void) :
		speed(10.f),
		mapOffset_X(0), 
		mapOffset_Y(0),
		mapFineOffset_X(0),
		mapFineOffset_Y(0)
{
}

CShuriken::~CShuriken(void)
{
}

void CShuriken::setSpeed(float input)
{

}

bool CShuriken::CheckCollision (CMap* m_cMap,bool m_bCheckLeft,bool m_bCheckRight, bool m_bCheckUp, bool m_bCheckDown)
{
	int mapFineOffset_x = mapOffset_X % m_cMap->GetTileSize();
	int checkPosition_X2 = (int) ((mapOffset_X+getPos().x - mapFineOffset_x) / m_cMap->GetTileSize());
	int checkPosition_X3 = (int) ((mapOffset_X+getPos().x - mapFineOffset_x) / m_cMap->GetTileSize());
	int checkPosition_Y2 = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (getPos().y + m_cMap->GetTileSize()) / m_cMap->GetTileSize());

	if(m_bCheckRight)
	{
		if (//Border
			(m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X2+2] == TILE_BORDER) ||
			//Grass
			(m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X2+2] == TILE_GRASS) ||
			//Tile underground
			(m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X2+2] == TILE_UNDERGROUND))
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
			//Tile underground
			(m_cMap->theScreenMap[checkPosition_Y2][checkPosition_X3] == TILE_UNDERGROUND ) )
		{
			return true;
		}
	}

	if(m_bCheckUp)
	{
		int checkPosition_X = (int) ((mapOffset_X+ getPos().x) / m_cMap->GetTileSize());
		int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (getPos().y + m_cMap->GetTileSize()) / m_cMap->GetTileSize());

		if ( //Border
			 (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_BORDER) ||
		     (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == TILE_BORDER) ||
			 //Grass
			 (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_GRASS) ||
		     (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == TILE_GRASS) ||
			 //Tile underground
			 (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_GRASS) ||
		     (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == TILE_GRASS))
		{
			return true;
		}
	}

	if(m_bCheckDown)
	{
		int checkPosition_X = (int) ((mapOffset_X+getPos().x) / m_cMap->GetTileSize());
		int checkPosition_Y = m_cMap->GetNumOfTiles_Height() - (int) ceil( (float) (getPos().y) / m_cMap->GetTileSize());

		if ( (m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_GRASS) || 
			(m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+1] == TILE_GRASS) || 
			(m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == TILE_GRASS) ||
			//Tile underground
			(m_cMap->theScreenMap[checkPosition_Y][checkPosition_X] == TILE_GRASS) || 
			(m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+1] == TILE_GRASS) || 
			(m_cMap->theScreenMap[checkPosition_Y][checkPosition_X+2] == TILE_GRASS))
		{
			return true;
		}

	}

	if(getPos().x < 0 )
	{
		return true;
	}

	return false;
}

void CShuriken::Update(CMap* m_cMap,int mapOffset_X,int mapOffset_Y)
{
	this->mapOffset_X = mapOffset_X;
	this->mapOffset_Y = mapOffset_Y;

	Vector3 dir = getDir();
	dir.Normalize();

	Vector3 newPos = getPos();
	
	newPos += Vector3(dir.x * speed,dir.y * speed,dir.z * speed);
	setPos(newPos);

	if(CheckCollision(m_cMap,true,true,true,true))
	{
		setActive(false);
	}
}