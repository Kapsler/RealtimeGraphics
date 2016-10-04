#pragma once

#include "TextureClass.h"
#include <DirectXMath.h>
#include <fstream>
using namespace std;

class ModelClass
{
public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	ID3D11Resource* GetTexture();
	ID3D11ShaderResourceView* GetTextureView();

private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
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

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();

	ID3D11Buffer *vertexBuffer, *indexBuffer;
	int vertexCount, indexCount;
	ModelType* model;
	TextureClass* texture;
};