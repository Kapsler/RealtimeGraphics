#include "ModelClass.h"

ModelClass::ModelClass()
{
	vertexBuffer = nullptr;
	indexBuffer = nullptr;
	instanceBuffer = nullptr;
	texture = nullptr;
	model = nullptr;
}

ModelClass::ModelClass(const ModelClass&)
{
}

ModelClass::~ModelClass()
{
}

bool ModelClass::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename)
{
	bool result;

	//Load Model
	result = LoadModel(modelFilename);
	if(!result)
	{
		return false;
	}

	//Init vertex und index buffers
	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	//Load Texture
	result = LoadTexture(device, textureFilename);
	if(!result)
	{
		return false;
	}

	return true;
}

void ModelClass::Shutdown()
{
	ReleaseTexture();

	ShutdownBuffer();

	ReleaseModel();
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

ID3D11Resource* ModelClass::GetTexture()
{
	return texture->GetTexture();
}

ID3D11ShaderResourceView* ModelClass::GetTextureView()
{
	return texture->GetTextureView();
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
	instanceCount = 4;

	instances = new InstanceType[instanceCount];
	if(!instances)
	{
		return false;
	}

	//Load instance array with data
	instances[0].position = XMFLOAT3(-1.5, -1.5f, 5);
	instances[1].position = XMFLOAT3(-1.5, 1.5f, 5);
	instances[2].position = XMFLOAT3(1.5, -1.5f, 5);
	instances[3].position = XMFLOAT3(1.5, 1.5f, 5);
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

bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;

	texture = new TextureClass;
	if(!texture)
	{
		return false;
	}

	result = texture->Initialize(device, filename);
	if(!result)
	{
		return false;
	}

	return true;
}

void ModelClass::ReleaseTexture()
{
	if(texture)
	{
		texture->Shutdown();
		delete texture;
		texture = nullptr;
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
