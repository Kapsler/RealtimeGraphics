#pragma once
#include "ModelType.h"
#include <d3d11.h>
#include <unordered_map>
#include <fstream>
#include <directxmath.h>
#include "tiny_obj_loader.h"
#include "GameWorld.h"

namespace DirectX {namespace SimpleMath {
	struct Matrix;
}
}

class ModelLoader
{
public:
	static ModelLoader& getInstance();

	ModelLoader(ModelLoader const&);
	void operator= (ModelLoader const&);

	bool GetModel(char* filename, ID3D11Device* device, ID3D11Buffer** vertexBuffer, ID3D11Buffer** indexBuffer, ID3D11Buffer** instanceBuffer, int* vertexCount, int* indexCount, int* instanceCount, const DirectX::SimpleMath::Matrix& worldMatrix);
	std::vector<GameWorld::Triangle*> GetTriangles(char* filename);
	void Release();
private:

	struct VectorType
	{
		float x, y, z;
	};

	struct TempVertexType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};
	
	struct bufferStruct
	{
		ID3D11Buffer* vertexBuffer;
		ID3D11Buffer* indexBuffer;
		ID3D11Buffer* instanceBuffer;
		int vertexCount;
		int indexCount;
		int instanceCount;
	};

	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT3 tangent;
		DirectX::XMFLOAT3 binormal;
	};

	struct InstanceType
	{
		DirectX::XMFLOAT3 position;
		float scale;
	};

	ModelLoader();
	~ModelLoader();

	bool LoadModel(char* filename);

	void CalculateModelVectors();
	void CalculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType, VectorType&, VectorType&);
	void CalculateNormal(VectorType, VectorType, VectorType&);

	bool InitializeBuffers(ID3D11Device* device);
	bool ModelFromFile(char* filename, ID3D11Device* device);

	std::unordered_map<std::string, bufferStruct*> buffers;
	std::unordered_map<std::string, std::vector<GameWorld::Triangle*>> triangles;

	bufferStruct* lastTriple;
	std::vector<ModelType> lastmodel;
	//int lastvertexCount, lastindexCount, lastinstanceCount;
};
