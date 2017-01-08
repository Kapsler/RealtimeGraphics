#pragma once
#include "GameWorld.h"

struct ModelType
{
	float x, y, z;
	float tu, tv;
	float nx, ny, nz;
	float tx, ty, tz;
	float bx, by, bz;
	GameWorld::Triangle triangle;
};
