#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

class CMap
{
public:
	CMap(void);
	~CMap(void);

	void Init(const int theScreen_Height, const int theScreen_Width, 
			  const int theNumOfTiles_Height, const int theNumOfTiles_Width, 
			  const int theMap_Height, const int theMap_Width, 
			  const int theTileSize=25);	//Initialize map

	bool LoadMap(const string mapName);
	int GetNumOfTiles_Height(void);	//Get num of tiles for height of screen
	int GetNumOfTiles_Width(void);	//Get num of tiles for width of screen
	int GetTileSize(void);	//Get number of tiles for map

	int getNumOfTiles_MapHeight(void);	//Get num of tiles for height of map
	int getNumOfTiles_MapWidth(void);	//Get num of tiles for width of map

	vector<vector<int>> theScreenMap;

private:
	int theScreen_Height;	//Get screen height
	int theScreen_Width;	//Get screen width
	int theNumOfTiles_Height;	// Number of tiles in the screen's height
	int theNumOfTiles_Width;	// Number of tiles in the screen's width
	int theTileSize;

	int theMap_Height;	//The Map's height;
	int theMap_Width;	//The Map's width;
	int theNumOfTiles_MapHeight;	//Number of tiles in the map's height
	int theNumOfTiles_MapWidth;		//Number of tiles in the map's width

	bool LoadFile(const string mapName);
};
