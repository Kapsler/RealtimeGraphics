#pragma once
#include <d3d11.h>
#include <SimpleMath.h>
#include <vector>
#include "GameWorld.h"
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <DirectXColors.h>

class MyBoundingBox : public DirectX::BoundingBox
{
public:
	static bool SmallestX(const GameWorld::Triangle* t1, const GameWorld::Triangle* t2);
	static bool SmallestY(const GameWorld::Triangle* t1, const GameWorld::Triangle* t2);
	static bool SmallestZ(const GameWorld::Triangle* t1, const GameWorld::Triangle* t2);
	static bool GreatestX(const GameWorld::Triangle* t1, const GameWorld::Triangle* t2);
	static bool GreatestY(const GameWorld::Triangle* t1, const GameWorld::Triangle* t2);
	static bool GreatestZ(const GameWorld::Triangle* t1, const GameWorld::Triangle* t2);

	MyBoundingBox(const std::vector<GameWorld::Triangle*>& tris);

	DirectX::SimpleMath::Vector3 smallest;
	DirectX::SimpleMath::Vector3 greatest;

	void Draw(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch, DirectX::XMVECTORF32 color);
	int GetLongestAxis() const;
};
