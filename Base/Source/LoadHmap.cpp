#include <iostream>
#include <fstream>
#include "LoadHmap.h"


bool LoadHeightMap(const char *file_path, std::vector<unsigned char> &heightMap)
{
	std::ifstream fileStream(file_path, std::ios::binary);
	if(!fileStream.is_open())
	{
		std::cout << "Impossible to open " << file_path << ". Are you in the right directory ?\n";
		return false;
	}

    fileStream.seekg(0, std::ios::end);
    std::streampos fsize = (unsigned)fileStream.tellg();
	
	fileStream.seekg(0, std::ios::beg);
	heightMap.resize((unsigned)fsize);
	fileStream.read((char *)&heightMap[0], fsize);
	
	fileStream.close();
	for(int z = 0; z < 256; ++z)
	{
		for(int x = 0; x < 256; ++x)
		{
			heights[x][z] = (float)heightMap[z * 256 + x];
		}
	}
	return true;
}
static float barryCentric(Vector3 p1, Vector3 p2, Vector3 p3,float x, float z)
{
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (x - p3.x) + (p3.x - p2.x) * (z - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (x - p3.x) + (p1.x - p3.x) * (z - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}


Vector3 calculateNormal(float x, float z,std::vector<unsigned char> &heightMap)
{
	float heightL = getHeight(heightMap,x - 0.5f/4000.f, z/4000.f );
	float heightR = getHeight(heightMap,x + 0.5f/4000.f, z/4000.f );
	float heightD = getHeight(heightMap,x/4000.f, z - 0.5f/4000.f);
	float heightU = getHeight(heightMap,x/4000.f, z + 0.5f/4000.f);
	Vector3 normal;
	normal.Set(heightL - heightR, 2.f, heightD - heightU);
	normal.Normalize();
	return normal;
}

float getHeight(std::vector<unsigned char> &heightMap, float x, float z)
{
	//Error  Handling
	if(x < -0.5f || x > 0.5f || z < -0.5f || z > 0.5f)
	{
		return 0;
	}
	/*if(heightMap.size() == 0)
	{
		return 0;
	}*/

	unsigned terrainSize = static_cast<unsigned>(sqrt(static_cast<double>(heightMap.size())));

	float gridSquareSize = 1.f / terrainSize;

	unsigned gridX = (x + 0.5f) / gridSquareSize;
	unsigned gridZ = (z + 0.5f) / gridSquareSize;
	
	float xCoord = fmod((x + 0.5f),gridSquareSize)/gridSquareSize;
	float zCoord = fmod((z + 0.5f),gridSquareSize)/gridSquareSize;

	float result = 0.f;
	if(xCoord <= ( 1-zCoord))
	{
		result = barryCentric(Vector3(0,heights[gridX][gridZ],0),
					Vector3(1,heights[gridX + 1][gridZ],0),
					Vector3(0,heights[gridX][gridZ + 1],1),
					xCoord,zCoord);
	}
	else
	{
		result = barryCentric(Vector3(1,heights[gridX+1][gridZ],0),
					Vector3(1,heights[gridX + 1][gridZ+1],1),
					Vector3(0,heights[gridX][gridZ + 1],1),
					xCoord,zCoord);
	}
	return result  / 256.f;
	/*float xCoord = static_cast<unsigned>((x + 0.5f) * terrainSize);
	float zCoord = static_cast<unsigned>((z + 0.5f) * terrainSize);

	return static_cast<float>(heightMap[zCoord * terrainSize + xCoord] / 256.f);*/
}

float ReadHeightMap(std::vector<unsigned char> &heightMap, float x, float z)
{
	//Error  Handling
	if(x < -0.5f || x > 0.5f || z < -0.5f || z > 0.5f)
	{
		return 0;
	}
	if(heightMap.size() == 0)
	{
		return 0;
	}

	//unsigned terrainSize = (unsigned)sqrt((double)heightMap.size());

	//unsigned xCoord = (x + 0.5f) * terrainSize;
	//unsigned zCoord = (z + 0.5f) * terrainSize;

	//return (float)heightMap[zCoord * terrainSize + xCoord] / 256.f;

	unsigned terrainSize = static_cast<unsigned>(sqrt(static_cast<double>(heightMap.size())));

	float xCoord = static_cast<unsigned>((x + 0.5f) * terrainSize);
	float zCoord = static_cast<unsigned>((z + 0.5f) * terrainSize);

	return static_cast<float>(heightMap[zCoord * terrainSize + xCoord] / 256.f);
}