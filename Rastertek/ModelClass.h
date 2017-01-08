#pragma once

#include "TextureClass.h"
#include <DirectXMath.h>
#include <fstream>
#include "ModelType.h"
#include "SimpleMath.h"

using namespace std;

class ModelClass
{
public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*, WCHAR*, const DirectX::SimpleMath::Matrix& worldMatrix);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	int GetVertexCount();
	int GetInstanceCount();

	bool changeInstanceData(ID3D11Device*, XMFLOAT3);

	ID3D11Resource** GetTextureArray();
	ID3D11ShaderResourceView** GetTextureViewArray();

	XMMATRIX worldMatrix;

private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;
		XMFLOAT3 binormal;
	};

	struct InstanceType
	{
		XMFLOAT3 position;
		float scale;
	};

	struct TempVertexType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VectorType
	{
		float x, y, z;
	};

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffer();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTextures(ID3D11Device*, WCHAR*, WCHAR*);
	void ReleaseTextures();

	bool LoadModel(char*);
	void ReleaseModel();

	void CalculateModelVectors();
	void CalculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType, VectorType&, VectorType&);
	void CalculateNormal(VectorType, VectorType, VectorType&);

	ID3D11Buffer *vertexBuffer, *indexBuffer, *instanceBuffer;
	int vertexCount, indexCount, instanceCount;
	ModelType* model;
	TextureClass* textures;
};