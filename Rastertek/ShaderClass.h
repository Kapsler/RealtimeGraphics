#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
using namespace DirectX;
using namespace std;

class ShaderClass
{
public:
	ShaderClass();
	ShaderClass(const ShaderClass&);
	~ShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext* context, int indexCount, int instanceCount,
		XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX	projectionMatrix,
		XMMATRIX lightViewMatrix, XMMATRIX lightProjectionMatrix, ID3D11ShaderResourceView* textureView,
		ID3D11ShaderResourceView* depthMapTexture, XMFLOAT3 lightPosition, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor,
		XMMATRIX lightViewMatrix2, XMMATRIX lightProjectionMatrix2, ID3D11ShaderResourceView* depthMapTexture2, XMFLOAT3 lightPosition2, XMFLOAT4 diffuseColor2);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext* context, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix,
		XMMATRIX lightViewMatrix, XMMATRIX lightProjectionMatrix, ID3D11ShaderResourceView* textureView, ID3D11ShaderResourceView* depthMapTexture, XMFLOAT3 lightPosition, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor,
		XMMATRIX lightViewMatrix2, XMMATRIX lightProjectionMatrix2, ID3D11ShaderResourceView* depthMapTexture2, XMFLOAT3 lightPosition2, XMFLOAT4 diffuseColor2);
	void RenderShader(ID3D11DeviceContext*, int, int);

	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
		XMMATRIX lightView;
		XMMATRIX lightProjection;
		XMMATRIX lightView2;
		XMMATRIX lightProjection2;
	};

	struct LightBufferType
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 diffuseColor;
		XMFLOAT4 diffuseColor2;
	};

	struct LightBufferType2
	{
		XMFLOAT3 lightPosition;
		float padding1;
		XMFLOAT3 lightPosition2;
		float padding2;
	};

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleStateWrap;
	ID3D11SamplerState* sampleStateClamp;
	ID3D11Buffer* lightBuffer;
	ID3D11Buffer* lightBuffer2;
};