#pragma once

#include <d3d11.h>
#include "InputClass.h"
#include "SimpleMath.h"
#include "TimerClass.h"
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

	void Render();
	void GetViewMatrix(XMMATRIX&);

private:
	Vector3 position, rotation, up, forward, lookAt;
	XMMATRIX viewMatrix;
	TimerClass* timer;
	bool tracking, trackingKeyPressed;
};