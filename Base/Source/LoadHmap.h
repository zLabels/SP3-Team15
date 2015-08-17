#ifndef LOAD_HMAP_H
#define LOAD_HMAP_H

#include "MeshBuilder.h"
#include "Vector3.h"
#include "Vector2.h"
#include <vector>
#include <cmath>

bool LoadHeightMap(const char *file_path, std::vector<unsigned char> &heightMap);

float ReadHeightMap(std::vector<unsigned char> &heightMap, float x, float z);

float getHeight(std::vector<unsigned char> &heightMap, float x, float z);

Vector3 calculateNormal(float x, float z,std::vector<unsigned char> &heightMap);

static float barryCentric(Vector3 p1, Vector3 p2, Vector3 p3,float x, float z);

static float heights[256][256];

#endif