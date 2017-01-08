#pragma once
#include <string>
#include <unordered_map>
#include <directxmath.h>

class GameWorld
{
public:
	struct Triangle
	{
		DirectX::XMFLOAT3 vertices[3];
	};

	static GameWorld& getInstance();

	void AddTriangles(const std::vector<Triangle> newTriangles);
	void AddTriangle(Triangle tri);
	std::vector<Triangle> triangles;

private:


	GameWorld();
	~GameWorld();
};
