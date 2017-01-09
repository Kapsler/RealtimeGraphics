#pragma once
#include <d3d11.h>
#include "GameWorld.h"
#include "SimpleMath.h"

struct RayHitStruct
{
	GameWorld::Triangle* hitTriangle = nullptr;
	float hitDistance = 0.0f;
	DirectX::SimpleMath::Ray hitray;
	MyBoundingBox* hitBox = nullptr;
	DirectX::SimpleMath::Vector3 hitPoint = DirectX::SimpleMath::Vector3::Zero;
};