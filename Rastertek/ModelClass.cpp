#include "ModelClass.h"
#include <iostream>
#include "ModelLoader.h"

ModelClass::ModelClass()
{
	vertexBuffer = nullptr;
	indexBuffer = nullptr;
	instanceBuffer = nullptr;
	textures = nullptr;
	model = nullptr;
}

ModelClass::ModelClass(const ModelClass&)
{
}

ModelClass::~ModelClass()
{
}

bool ModelClass::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename1, WCHAR* textureFilename2)
{
	bool result;

	////Load Model
	//result = LoadModel(modelFilename);
	//if(!result)
	//{
	//	return false;
	//}

	//CalculateModelVectors();

	////Init vertex und index buffers
	//result = InitializeBuffers(device);
	//if(!result)
	//{
	//	return false;
	//}

	result = ModelLoader::getInstance().GetModel(modelFilename, device, &vertexBuffer, &indexBuffer, &instanceBuffer, &vertexCount, &indexCount, &instanceCount);
	if(!result)
	{
		return false;
	}

	worldMatrix = XMMatrixIdentity();

	//Load Texture
	result = LoadTextures(device, textureFilename1, textureFilename2);
	if(!result)
	{
		return false;
	}

	return true;
}

void ModelClass::Shutdown()
{
	ReleaseTextures();

	//ShutdownBuffer();

	//ReleaseModel();
}

void ModelClass::Render(ID3D11DeviceContext* context)
{
	RenderBuffers(context);
}

int ModelClass::GetIndexCount()
{
	return indexCount;
}

int ModelClass::GetVertexCount()
{
	return vertexCount;
}

int ModelClass::GetInstanceCount()
{
	return instanceCount;
}

bool ModelClass::changeInstanceData(ID3D11Device* device, XMFLOAT3 newPosition)
{
	D3D11_BUFFER_DESC tempBufferDesc;
	D3D11_SUBRESOURCE_DATA instanceData;
	InstanceType* instance;
	bool result;

	instanceBuffer->GetDesc(&tempBufferDesc);


	instance = new InstanceType();
	if (!instance)
	{
		return false;
	}

	//Load instance array with data
	instance->position = newPosition;
	//HARDCODED ENDE

	instanceData.pSysMem = instance;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	//Create Instance Buffer
	result = device->CreateBuffer(&tempBufferDesc, &instanceData, &instanceBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//release instance array
	delete instance;
	instance = nullptr;

	return true;
}

ID3D11Resource** ModelClass::GetTextureArray()
{
	return textures->GetTextureArray();
}

ID3D11ShaderResourceView** ModelClass::GetTextureViewArray()
{
	return textures->GetTextureViewArray();
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	InstanceType* instances;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc, instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData, instanceData;
	HRESULT result;

	vertices = new VertexType[vertexCount];
	if(!vertices)
	{
		return false;
	}

	indices = new unsigned long[indexCount];
	if(!indices)
	{
		return false;
	}

	//Load vertex and index array with data
	for(auto i = 0; i < vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(model[i].x, model[i].y, model[i].z);
		vertices[i].texture = XMFLOAT2(model[i].tu, model[i].tv);
		vertices[i].normal = XMFLOAT3(model[i].nx, model[i].ny, model[i].nz);
		vertices[i].tangent = XMFLOAT3(model[i].tx, model[i].ty, model[i].tz);
		vertices[i].binormal = XMFLOAT3(model[i].bx, model[i].by, model[i].bz);

		indices[i] = i;
	}

	//Set up static vertex buffer desc
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//Create Vertex Buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	//Set up static index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	//Release arrays
	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;

	//HARDCODED
	instanceCount = 1;

	instances = new InstanceType[instanceCount];
	if(!instances)
	{
		return false;
	}

	//Load instance array with data
	instances[0].position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	instances[0].scale = 1.0f;
	//HARDCODED ENDE


	//Instance Description
	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	instanceData.pSysMem = instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	//Create Instance Buffer
	result = device->CreateBuffer(&instanceBufferDesc, &instanceData, &instanceBuffer);
	if(FAILED(result))
	{
		return false;
	}

	//release instance array
	delete[] instances;
	instances = nullptr;

	worldMatrix = XMMatrixIdentity();

	return true;
}

void ModelClass::ShutdownBuffer()
{
	if(instanceBuffer)
	{
		instanceBuffer->Release();
		instanceBuffer = nullptr;
	}

	if(indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = nullptr;
	}

	if(vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = nullptr;
	}
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* context)
{
	unsigned int strides[2];
	unsigned int offsets[2];
	ID3D11Buffer* bufferPointers[2];

	strides[0] = sizeof(VertexType);
	strides[1] = sizeof(InstanceType);
	offsets[0] = 0;
	offsets[1] = 0;
	bufferPointers[0] = vertexBuffer;
	bufferPointers[1] = instanceBuffer;

	//Set vertex buffer active
	context->IASetVertexBuffers(0, 2, bufferPointers, strides, offsets);

	//Set index buffer actice
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Set type of primitive that should be rendered
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool ModelClass::LoadTextures(ID3D11Device* device, WCHAR* filename1, WCHAR* filename2)
{
	bool result;

	textures = new TextureClass;
	if(!textures)
	{
		return false;
	}

	result = textures->Initialize(device, filename1, filename2);
	if(!result)
	{
		return false;
	}

	return true;
}

void ModelClass::ReleaseTextures()
{
	if(textures)
	{
		textures->Shutdown();
		delete textures;
		textures = nullptr;
	}
}

bool ModelClass::LoadModel(char* filename)
{
	ifstream fin;
	char input;


	//open file
	fin.open(filename);

	if(fin.fail())
	{
		return false;
	}

	//First ":" for vertex count
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	fin >> vertexCount;
	indexCount = vertexCount;

	model = new ModelType[vertexCount];
	if(!model)
	{
		return false;
	}

	//Read until data follows
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	//Read vertex data
	for(auto i = 0; i < vertexCount; i++)
	{
		fin >> model[i].x >> model[i].y >> model[i].z;
		fin >> model[i].tu >> model[i].tv;
		fin >> model[i].nx >> model[i].ny >> model[i].nz;
	}

	//Close file
	fin.close();

	return true;
}

void ModelClass::ReleaseModel()
{
	if(model)
	{
		delete[] model;
		model = nullptr;
	}
}

void ModelClass::CalculateModelVectors()
{
	int faceCount, index;
	TempVertexType vertex1, vertex2, vertex3;
	VectorType tangent, binormal, normal;

	faceCount = vertexCount / 3;

	index = 0;

	for(auto i = 0; i < faceCount; ++i)
	{
		vertex1.x = model[index].x;
		vertex1.y = model[index].y;
		vertex1.z = model[index].z;
		vertex1.tu = model[index].tu;
		vertex1.tv = model[index].tv;
		vertex1.nx = model[index].nx;
		vertex1.ny = model[index].ny;
		vertex1.nz = model[index].nz;
		index++;

		vertex2.x = model[index].x;
		vertex2.y = model[index].y;
		vertex2.z = model[index].z;
		vertex2.tu = model[index].tu;
		vertex2.tv = model[index].tv;
		vertex2.nx = model[index].nx;
		vertex2.ny = model[index].ny;
		vertex2.nz = model[index].nz;
		index++;

		vertex3.x = model[index].x;
		vertex3.y = model[index].y;
		vertex3.z = model[index].z;
		vertex3.tu = model[index].tu;
		vertex3.tv = model[index].tv;
		vertex3.nx = model[index].nx;
		vertex3.ny = model[index].ny;
		vertex3.nz = model[index].nz;
		index++;

		CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);

		CalculateNormal(tangent, binormal, normal);

		model[index - 1].nx = normal.x;
		model[index - 1].ny = normal.y;
		model[index - 1].nz = normal.z;
		model[index - 1].tx = tangent.x;
		model[index - 1].ty = tangent.y;
		model[index - 1].tz = tangent.z;
		model[index - 1].bx = binormal.x;
		model[index - 1].by = binormal.y;
		model[index - 1].bz = binormal.z;

		model[index - 2].nx = normal.x;
		model[index - 2].ny = normal.y;
		model[index - 2].nz = normal.z;
		model[index - 2].tx = tangent.x;
		model[index - 2].ty = tangent.y;
		model[index - 2].tz = tangent.z;
		model[index - 2].bx = binormal.x;
		model[index - 2].by = binormal.y;
		model[index - 2].bz = binormal.z;

		model[index - 3].nx = normal.x;
		model[index - 3].ny = normal.y;
		model[index - 3].nz = normal.z;
		model[index - 3].tx = tangent.x;
		model[index - 3].ty = tangent.y;
		model[index - 3].tz = tangent.z;
		model[index - 3].bx = binormal.x;
		model[index - 3].by = binormal.y;
		model[index - 3].bz = binormal.z;
	}

}

void ModelClass::CalculateTangentBinormal(TempVertexType vertex1, TempVertexType vertex2, TempVertexType vertex3, VectorType& tangent, VectorType& binormal)
{
	float vector1[3], vector2[3];
	float tuVector[2], tvVector[2];
	float den;
	float length;

	// Calculate the two vectors for this face.
	vector1[0] = vertex2.x - vertex1.x;
	vector1[1] = vertex2.y - vertex1.y;
	vector1[2] = vertex2.z - vertex1.z;

	vector2[0] = vertex3.x - vertex1.x;
	vector2[1] = vertex3.y - vertex1.y;
	vector2[2] = vertex3.z - vertex1.z;

	// Calculate the tu and tv texture space vectors.
	tuVector[0] = vertex2.tu - vertex1.tu;
	tvVector[0] = vertex2.tv - vertex1.tv;

	tuVector[1] = vertex3.tu - vertex1.tu;
	tvVector[1] = vertex3.tv - vertex1.tv;

	// Calculate the denominator of the tangent/binormal equation.
	den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// Calculate the length of this normal.
	length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	// Normalize the normal and then store it
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// Calculate the length of this normal.
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	// Normalize the normal and then store it
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;
}

void ModelClass::CalculateNormal(VectorType tangent, VectorType binormal, VectorType& normal)
{
	float length;

	// Calculate the cross product of the tangent and binormal which will give the normal vector.
	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	// Calculate the length of the normal.
	length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	// Normalize the normal.
	normal.x = normal.x / length;
	normal.y = normal.y / length;
	normal.z = normal.z / length;

}
