#pragma once

#include <d3d11.h>
#include "InputClass.h"
#include "SimpleMath.h"
#include "TimerClass.h"
#include <vector>
using namespace DirectX;
using namespace SimpleMath;

class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	bool Initialize();
	void Shutdown();

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void DoMovement(InputClass*);
	std::vector<Vector3*> getTrackingPoints();

	void Render();
	void GetViewMatrix(XMMATRIX&);

private:
	Vector3 kochanekBartels();

	Vector3 lastPosition, position, rotation, up, forward, lookAt;
	XMMATRIX viewMatrix;
	TimerClass* timer;
	bool tracking, trackingKeyToggle;
	Quaternion test;
	std::vector<Vector3*> trackingPoints;
	float trackingProgress;
	int currentTrackingPoint;
};