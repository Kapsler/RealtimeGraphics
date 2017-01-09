#include "GameWorld.h"
#include <iostream>
#include <algorithm>

GameWorld::Triangle::Triangle()
{
}

void GameWorld::Triangle::CalculateSmallest()
{
	smallest.x = std::min<float>({vertices[0].x,vertices[1].x,vertices[2].x});
	smallest.y = std::min<float>({vertices[0].y,vertices[1].y,vertices[2].y});
	smallest.z = std::min<float>({vertices[0].z,vertices[1].z,vertices[2].z});
}

void GameWorld::Triangle::CalculateGreatest()
{
	greatest.x = std::max<float>({vertices[0].x, vertices[1].x, vertices[2].x});
	greatest.y = std::max<float>({vertices[0].y,vertices[1].y,vertices[2].y});
	greatest.z = std::max<float>({vertices[0].z, vertices[1].z, vertices[2].z});
}

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
