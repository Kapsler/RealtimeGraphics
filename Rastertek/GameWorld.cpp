#include "GameWorld.h"
#include <iostream>

GameWorld& GameWorld::getInstance()
{
	static GameWorld instance;
	return instance;
}

void GameWorld::AddTriangles(const std::vector<Triangle*> newTriangles)
{
	triangles.insert(triangles.end(), newTriangles.begin(), newTriangles.end());
	std::cout << triangles.size() << std::endl;
}

void GameWorld::AddTriangle(Triangle* tri)
{
	triangles.push_back(tri);
}

GameWorld::GameWorld()
{
}

GameWorld::~GameWorld()
{
}
