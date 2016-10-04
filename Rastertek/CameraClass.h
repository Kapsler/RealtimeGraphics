#pragma once

#include <DirectXMath.h>
using namespace DirectX;

class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();
	void GetViewMatrix(XMMATRIX&);

private:
	float positionX, positionY, positionZ;
	float rotationX, rotationY, rotationZ;
	XMMATRIX viewMatrix;

};