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
	bool Frame(InputClass*);

private:
	bool Render(float, InputClass*);
	void CheckWireframe(InputClass*);
	void ChangeFillmode(D3D11_FILL_MODE);

	D3DClass* direct3D;
	CameraClass* camera;
	ModelClass* model;
	ShaderClass* shader;

	bool wireframeMode, wireframeKeyToggle;

};