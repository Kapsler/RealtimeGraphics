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
	struct ControlPoint
	{
		Vector3 position;
		Quaternion direction;
	};

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
	std::vector<ControlPoint*> getTrackingPoints();

	void Render();
	void GetViewMatrix(XMMATRIX&);

private:

	ControlPoint* generatePoint(float px, float py, float pz, float ox, float oy, float oz, float ow);
	Quaternion kochanekBartels();

	Vector3 position, rotation;
	Matrix viewMatrix;
	TimerClass* timer;
	bool tracking, trackingKeyToggle;
	Quaternion viewQuaternion;
	std::vector<ControlPoint*> trackingPoints;
	float trackingProgress;
	int currentTrackingPoint;
};