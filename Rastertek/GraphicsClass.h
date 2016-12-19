#pragma once

#include "D3DClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "ShaderClass.h"
#include "TextureClass.h"
#include <PrimitiveBatch.h>
#include <vector>
#include "LightClass.h"
#include "RenderTextureClass.h"
#include "DepthShaderClass.h"
#include "SpriteFont.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const int SHADOWMAP_WIDTH = 2048;
const int SHADOWMAP_HEIGHT = 2048;

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
	bool RenderSceneToTexture();
	bool RenderSceneToTexture2();
	bool Render(float, InputClass*);
	void CheckWireframe(InputClass*);
	void SetLightDirection(InputClass*);
	void ChangeFillmode(D3D11_FILL_MODE);
	ModelClass* InitializeModel(HWND, char*, WCHAR*, WCHAR*, XMFLOAT3, float);
	void ShutdownModels();

	D3DClass* direct3D;
	CameraClass* camera;
	std::vector<ModelClass*> models;
	ShaderClass* shader;
	HWND* hwndptr;
	LightClass* light, *light2;
	TimerClass* timer;
	RenderTextureClass* renderTexture, *renderTexture2;
	DepthShaderClass* depthShader;

	bool wireframeMode, wireframeKeyToggle, bumpinessKeyToggle;
	float bumpiness;

	unique_ptr<DirectX::SpriteFont> m_font;
	DirectX::SimpleMath::Vector2 m_fontPos;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
};