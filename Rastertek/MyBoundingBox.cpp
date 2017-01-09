#include "MyBoundingBox.h"
#include <algorithm>
#include <iostream>
#include <PrimitiveBatch.h>
#include <DirectXColors.h>

bool MyBoundingBox::SmallestX(const GameWorld::Triangle* t1, const GameWorld::Triangle* t2)
{
	float small1 = std::min<float>({ t1->vertices[0].x,t1->vertices[1].x,t1->vertices[2].x});
	float small2 = std::min<float>({ t2->vertices[0].x,t2->vertices[1].x,t2->vertices[2].x});

	return small1 < small2;
}

bool MyBoundingBox::SmallestY(const GameWorld::Triangle* t1, const GameWorld::Triangle* t2)
{
	float small1 = std::min<float>({ t1->vertices[0].y,t1->vertices[1].y,t1->vertices[2].y });
	float small2 = std::min<float>({ t2->vertices[0].y,t2->vertices[1].y,t2->vertices[2].y });

	return small1 < small2;
}

bool MyBoundingBox::SmallestZ(const GameWorld::Triangle* t1, const GameWorld::Triangle* t2)
{
	float small1 = std::min<float>({ t1->vertices[0].z,t1->vertices[1].z,t1->vertices[2].z });
	float small2 = std::min<float>({ t2->vertices[0].z,t2->vertices[1].z,t2->vertices[2].z });

	return small1 < small2;
}

bool MyBoundingBox::GreatestX(const GameWorld::Triangle* t1, const GameWorld::Triangle* t2)
{
	float great1 = std::max<float>({ t1->vertices[0].x,t1->vertices[1].x,t1->vertices[2].x });
	float great2 = std::max<float>({ t2->vertices[0].x,t2->vertices[1].x,t2->vertices[2].x });

	return great1 > great2;
}

bool MyBoundingBox::GreatestY(const GameWorld::Triangle* t1, const GameWorld::Triangle* t2)
{
	float great1 = std::max<float>({ t1->vertices[0].y,t1->vertices[1].y,t1->vertices[2].y });
	float great2 = std::max<float>({ t2->vertices[0].y,t2->vertices[1].y,t2->vertices[2].y });

	return great1 > great2;
}

bool MyBoundingBox::GreatestZ(const GameWorld::Triangle* t1, const GameWorld::Triangle* t2)
{
	float great1 = std::max<float>({ t1->vertices[0].z,t1->vertices[1].z,t1->vertices[2].z });
	float great2 = std::max<float>({ t2->vertices[0].z,t2->vertices[1].z,t2->vertices[2].z });

	return great1 > great2;
}

MyBoundingBox::MyBoundingBox(const std::vector<GameWorld::Triangle*>& tris)
{
	int size = tris.size();

	auto xExtremes = std::minmax_element(tris.begin(), tris.end(), SmallestX);
	auto yExtremes = std::minmax_element(tris.begin(), tris.end(), SmallestY);
	auto zExtremes = std::minmax_element(tris.begin(), tris.end(), SmallestZ);

	smallest.x = std::min<float>({ (*xExtremes.first)->vertices[0].x,(*xExtremes.first)->vertices[1].x,(*xExtremes.first)->vertices[2].x });
	smallest.y = std::min<float>({ (*yExtremes.first)->vertices[0].y,(*yExtremes.first)->vertices[1].y,(*yExtremes.first)->vertices[2].y });
	smallest.z = std::min<float>({ (*zExtremes.first)->vertices[0].z,(*zExtremes.first)->vertices[1].z,(*zExtremes.first)->vertices[2].z });

	greatest.x = std::max<float>({ (*xExtremes.second)->vertices[0].x,(*xExtremes.second)->vertices[1].x,(*xExtremes.second)->vertices[2].x });
	greatest.y = std::max<float>({ (*yExtremes.second)->vertices[0].y,(*yExtremes.second)->vertices[1].y,(*yExtremes.second)->vertices[2].y });
	greatest.z = std::max<float>({ (*zExtremes.second)->vertices[0].z,(*zExtremes.second)->vertices[1].z,(*zExtremes.second)->vertices[2].z });

	Center.x = (smallest.x + greatest.x) / 2.0f;
	Center.y = (smallest.y + greatest.y) / 2.0f;
	Center.z = (smallest.z + greatest.z) / 2.0f;

	Extents.x = greatest.x - Center.x;
	Extents.y = greatest.y - Center.y;
	Extents.z = greatest.z - Center.z;
}

void MyBoundingBox::Draw(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch, DirectX::XMVECTORF32 color)
{
	std::vector<DirectX::SimpleMath::Vector3> corners;
	corners.resize(CORNER_COUNT);
	GetCorners(&corners[0]);
	if (!corners.empty())
	{
		batch->DrawLine(DirectX::VertexPositionColor(corners[0], color), DirectX::VertexPositionColor(corners[1], color));
		batch->DrawLine(DirectX::VertexPositionColor(corners[1], color), DirectX::VertexPositionColor(corners[2], color));
		batch->DrawLine(DirectX::VertexPositionColor(corners[2], color), DirectX::VertexPositionColor(corners[3], color));
		batch->DrawLine(DirectX::VertexPositionColor(corners[3], color), DirectX::VertexPositionColor(corners[0], color));

		batch->DrawLine(DirectX::VertexPositionColor(corners[4], color), DirectX::VertexPositionColor(corners[5], color));
		batch->DrawLine(DirectX::VertexPositionColor(corners[5], color), DirectX::VertexPositionColor(corners[6], color));
		batch->DrawLine(DirectX::VertexPositionColor(corners[6], color), DirectX::VertexPositionColor(corners[7], color));
		batch->DrawLine(DirectX::VertexPositionColor(corners[7], color), DirectX::VertexPositionColor(corners[4], color));

		batch->DrawLine(DirectX::VertexPositionColor(corners[0], color), DirectX::VertexPositionColor(corners[4], color));
		batch->DrawLine(DirectX::VertexPositionColor(corners[1], color), DirectX::VertexPositionColor(corners[5], color));
		batch->DrawLine(DirectX::VertexPositionColor(corners[2], color), DirectX::VertexPositionColor(corners[6], color));
		batch->DrawLine(DirectX::VertexPositionColor(corners[3], color), DirectX::VertexPositionColor(corners[7], color));
	}
}

int MyBoundingBox::GetLongestAxis() const
{
	float longest = std::max<float>({Extents.x, Extents.y, Extents.z});

	if(longest == Extents.x)
	{
		return 0;
	}
	if(longest == Extents.y)
	{
		return 1;
	}
	if(longest == Extents.z)
	{
		return 2;
	}

	return 0;
}
