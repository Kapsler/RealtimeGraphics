#include "KdNode.h"
#include <algorithm>
#include <iostream>

bool KdNode::SmallestX(const GameWorld::Triangle* t1, const GameWorld::Triangle* t2)
{
	float small1 = (t1->vertices[0].x + t1->vertices[1].x + t1->vertices[2].x) / 3.0f;
	float small2 = (t2->vertices[0].x + t2->vertices[1].x + t2->vertices[2].x) / 3.0f;

	return small1 < small2;
}

bool KdNode::SmallestY(const GameWorld::Triangle* t1, const GameWorld::Triangle* t2)
{
	float small1 = (t1->vertices[0].y + t1->vertices[1].y + t1->vertices[2].y) / 3.0f;
	float small2 = (t2->vertices[0].y + t2->vertices[1].y + t2->vertices[2].y) / 3.0f;

	return small1 < small2;
}

bool KdNode::SmallestZ(const GameWorld::Triangle* t1, const GameWorld::Triangle* t2)
{
	float small1 = (t1->vertices[0].z + t1->vertices[1].z + t1->vertices[2].z) / 3.0f;
	float small2 = (t2->vertices[0].z + t2->vertices[1].z + t2->vertices[2].z) / 3.0f;

	return small1 < small2;
}

KdNode::KdNode()
{
	left = nullptr;
	right = nullptr;
	bbox = nullptr;
}

KdNode* KdNode::build(std::vector<GameWorld::Triangle*>* tris, int depth) const
{
	KdNode* node = new KdNode();
	node->triangles = tris;
	node->left = nullptr;
	node->right = nullptr;
	node->bbox = nullptr;

	if(tris->empty())
	{
		return node;
	}

	if(tris->size() <= 100 || depth > 1000)
	{
		node->bbox = new MyBoundingBox(*tris);
		node->left = new KdNode();
		node->right = new KdNode();
		node->left->triangles = new std::vector<GameWorld::Triangle*>();
		node->right->triangles = new std::vector<GameWorld::Triangle*>();
		return node;
	}

	node->bbox = new MyBoundingBox(*tris);

	std::vector<GameWorld::Triangle*>* leftTris = new std::vector<GameWorld::Triangle*>();
	std::vector<GameWorld::Triangle*>* rightTris = new std::vector<GameWorld::Triangle*>();;
	GameWorld::Triangle* medTri = nullptr;
	float axisBaryCenter = 0.0f;
	int axis = node->bbox->GetLongestAxis();

	if (axis == 0) {
		std::nth_element(tris->begin(), tris->begin() + tris->size() / 2, tris->end(), SmallestX);
		medTri = (*(tris->begin() + tris->size() / 2));
		axisBaryCenter = medTri->getBarycenter().x;
	} else if (axis == 1) {
		std::nth_element(tris->begin(), tris->begin() + tris->size() / 2, tris->end(), SmallestY);
		medTri = (*(tris->begin() + tris->size() / 2));
		axisBaryCenter = medTri->getBarycenter().y;
	}
	else {
		std::nth_element(tris->begin(), tris->begin() + tris->size() / 2, tris->end(), SmallestZ);
		medTri = (*(tris->begin() + tris->size() / 2));
		axisBaryCenter = medTri->getBarycenter().z;
	}

	for (int i = 0; i < tris->size(); ++i)
	{
		switch (axis)
		{
		case 0:
			if ((*tris)[i]->smallest.x < axisBaryCenter)
			{
				leftTris->push_back((*tris)[i]);
			}
			if ((*tris)[i]->greatest.x >= axisBaryCenter)
			{
				rightTris->push_back((*tris)[i]);
			}
			break;
		case 1:
			if ((*tris)[i]->smallest.y < axisBaryCenter)
			{
				leftTris->push_back((*tris)[i]);
			}
			if ((*tris)[i]->greatest.y >= axisBaryCenter)
			{
				rightTris->push_back((*tris)[i]);
			}
			break;
		case 2:
			if ((*tris)[i]->smallest.z < axisBaryCenter)
			{
				leftTris->push_back((*tris)[i]);
			}
			if ((*tris)[i]->greatest.z >= axisBaryCenter)
			{
				rightTris->push_back((*tris)[i]);
			}
			break;
		default: break;
		}
	}
	node->left = build(leftTris, depth + 1);
	node->right = build(rightTris, depth + 1);

	return node;
}

bool KdNode::hit(KdNode* node, const DirectX::SimpleMath::Ray* ray, float& t, float& tmin, RayHitStruct& rayhit)
{
	float f;
	if (ray->Intersects(*node->bbox, f))
	{
		if(node->left->triangles->size() > 0 || node->right->triangles->size() > 0)
		{
			bool hitleft = false;
			bool hitright = false;

			if(node->left->triangles->size() > 0)
			{
				if (hit(node->left, ray, t, tmin, rayhit))
				{
					hitleft = true;
				}
			}
			
			if(node->right->triangles->size() > 0)
			{
				if (hit(node->right, ray, t, tmin, rayhit))
				{
					hitright = true;
				}
			}

			return hitleft || hitright;
		}
		else
		{
			bool hitBool = false;
			for(const auto tri : *node->triangles)
			{
				if (ray->Intersects(tri->vertices[0], tri->vertices[1], tri->vertices[2], t))
				{
					if(t < tmin)
					{
						tmin = t;
						rayhit.hitDistance = t;
						rayhit.hitTriangle = tri;
						rayhit.hitray = *ray;
						rayhit.hitBox = node->bbox;
						rayhit.hitPoint = rayhit.hitray.position + rayhit.hitray.direction * rayhit.hitDistance;
						hitBool = true;
					}
				}
			}

			return hitBool;
		}

	}

	return false;
}

void KdNode::Draw(DirectX::PrimitiveBatch<DirectX::VertexPositionColor>* batch, DirectX::XMVECTORF32 color)
{
	if (this->bbox != nullptr)
	{
		this->bbox->Draw(batch, color);
	}if (this->left != nullptr)
	{
		this->left->Draw(batch, color);
	}
	if (this->right != nullptr)
	{
		this->right->Draw(batch, color);
	}
}
