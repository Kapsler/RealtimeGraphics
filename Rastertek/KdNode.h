#pragma once
#include "MyBoundingBox.h"
#include "RayHitStruct.h"

class KdNode
{
public:
	static bool SmallestX(const GameWorld::Triangle* t1, const GameWorld::Triangle* t2);
	static bool SmallestY(const GameWorld::Triangle* t1, const GameWorld::Triangle* t2);
	static bool SmallestZ(const GameWorld::Triangle* t1, const GameWorld::Triangle* t2);

	KdNode();
	KdNode* build(std::vector<GameWorld::Triangle*>* tris, int depth) const;

	static bool hit(KdNode* node, const DirectX::SimpleMath::Ray* ray, float& t, float& tmin, RayHitStruct& rayhit);

	void Draw(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch, DirectX::XMVECTORF32 color);

	MyBoundingBox* bbox;
	KdNode *left, *right;
	std::vector<GameWorld::Triangle*>* triangles;
};
