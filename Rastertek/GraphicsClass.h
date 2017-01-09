#pragma once

#include "D3DClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "ShaderClass.h"
#include "TextureClass.h"
#include <vector>
#include "LightClass.h"
#include "RenderTextureClass.h"
#include "DepthShaderClass.h"
#include "SpriteFont.h"
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <Effects.h>
#include <CommonStates.h>
#include "MyBoundingBox.h"
#include "KdNode.h"
#include "RayHitStruct.h"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 2000.0f;
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

	bool GenerateScreenBuffer();

private:
	void SetupPrimitiveBatch();
	bool RenderSceneToTexture();
	bool RenderSceneToTexture2();
	bool SetScreenBuffer(float, float, float, float);
	void RenderRay();
	void CastRay();
	bool Render(float, InputClass*);
	void CheckWireframe(InputClass*);
	void CheckMSKeys(InputClass*);
	void CheckRaycast(InputClass*);
	void SetLightDirection(InputClass*);
	void ChangeFillmode(D3D11_FILL_MODE);
	ModelClass* InitializeModel(HWND, char*, WCHAR*, WCHAR*, XMFLOAT3, float);
	void ShutdownModels();
	void RenderText(string texttorender, Vector2 screenPos, bool centerOrigin);
	void RenderText(int inttorender, Vector2 screenPos, bool centerOrigin);

	D3DClass* direct3D;
	CameraClass* camera;
	std::vector<ModelClass*> models;
	ShaderClass* shader;
	HWND* hwndptr;
	LightClass* light, *light2;
	TimerClass* timer;
	RenderTextureClass* renderTexture, *renderTexture2;
	DepthShaderClass* depthShader;

	bool wireframeMode, wireframeKeyToggle, bumpinessKeyToggle, msmodetoggle;
	float bumpiness;
	int currentScreenWidth, currentScreenHeight;
	float fps;

	unique_ptr<DirectX::SpriteFont> m_font;
	DirectX::SimpleMath::Vector2 fpsPos, scPos, qlPos;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;

	ID3D11Texture2D* screenBuffer;
	ID3D11RenderTargetView* screenTargetView;
	ID3D11Texture2D* depthBuffer;
	ID3D11DepthStencilView* depthTargetView;

	PrimitiveBatch<VertexPositionColor>* primitiveBatch; 
	BasicEffect* basicEffect;
	ID3D11InputLayout* inputLayout;

	KdNode* tree;
	Ray ray;
	RayHitStruct hit1;
	RayHitStruct hit2;
	int hitCounter;
	Vector3 ruler;

	bool rayToggle = false;
	bool rayRenderToggle = false;
	bool rayrendermode = false;
	
};