#include "LightClass.h"

LightClass::LightClass()
{
}

LightClass::LightClass(const LightClass&)
{
}

LightClass::~LightClass()
{
}

void LightClass::SetAmbientColor(float red, float green, float blue, float alpha)
{
	ambientColor = XMFLOAT4(red, green, blue, alpha);
}

XMFLOAT4 LightClass::GetAmbientColor()
{
	return  ambientColor;
}

void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	diffuseColor = XMFLOAT4(red, green, blue, alpha);
}

void LightClass::SetDirection(float x, float y, float z)
{
	direction = XMFLOAT3(x, y, z);
}

XMFLOAT4 LightClass::GetDiffuseColor()
{
	return diffuseColor;
}

XMFLOAT3 LightClass::GetDirection()
{
	return direction;
}
