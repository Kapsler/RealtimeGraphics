#pragma once
#include <directxmath.h>
using namespace DirectX;

class LightClass
{
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);

	XMFLOAT4 GetAmbientColor();
	XMFLOAT4 GetDiffuseColor();
	XMFLOAT3 GetDirection();

private:
	XMFLOAT4 diffuseColor, ambientColor;
	XMFLOAT3 direction;
};
