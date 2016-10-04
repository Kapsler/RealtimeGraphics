#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
using namespace DirectX;
using namespace std;

class ModelClass
{
public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT4 color;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffer();
	void RenderBuffers(ID3D11DeviceContext*);
	bool LoadModel(char*);
	void ReleaseModel();

	ID3D11Buffer *vertexBuffer, *indexBuffer;
	int vertexCount, indexCount;
	ModelType* model;
};