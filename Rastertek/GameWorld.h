#pragma once
#include <string>
#include <d3d11.h>
#include <unordered_map>
#include <directxmath.h>
#include <SimpleMath.h>

class GameWorld
{
public:
	class Triangle
	{
	public:
		Triangle();

		inline DirectX::SimpleMath::Vector3 getBarycenter() const
		{
			return DirectX::SimpleMath::Vector3((vertices[0].x + vertices[1].x + vertices[2].x) / 3.0f, (vertices[0].y + vertices[1].y + vertices[2].y) / 3.0f, (vertices[0].z + vertices[1].z + vertices[2].z) / 3.0f);
		}

		void CalculateSmallest();
		void CalculateGreatest();

		DirectX::XMFLOAT3 vertices[3];
		DirectX::SimpleMath::Vector3 smallest, greatest;
		bool alreadyCut = false;
	};

	static GameWorld& getInstance();

	void AddTriangles(const std::vector<Triangle*> newTriangles);
	void AddTriangle(Triangle* tri);
	std::vector<Triangle*> triangles;

private:


	GameWorld();
	~GameWorld();
};
