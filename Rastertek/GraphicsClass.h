#pragma once

#include "D3DClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "ShaderClass.h"
#include "TextureClass.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render(float);

	D3DClass* direct3D;
	CameraClass* camera;
	ModelClass* model;
	ShaderClass* shader;
};