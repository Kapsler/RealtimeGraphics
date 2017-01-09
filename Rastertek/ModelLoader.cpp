#define TINYOBJLOADER_IMPLEMENTATION
#include "ModelLoader.h"
#include <iostream>
#include "GameWorld.h"
#include <SimpleMath.h>

ModelLoader& ModelLoader::getInstance()
{
	static ModelLoader instance;
	return instance;
}

bool ModelLoader::GetModel(char* filename, ID3D11Device* device, ID3D11Buffer** vertexBuffer, ID3D11Buffer** indexBuffer, ID3D11Buffer** instanceBuffer, int* vertexCount, int* indexCount, int* instanceCount, const DirectX::SimpleMath::Matrix& worldMatrix)
{
	auto i = buffers.find(filename);

	if(i != buffers.end())
	{
		*vertexBuffer = i->second->vertexBuffer;
		*indexBuffer = i->second->indexBuffer;
		*instanceBuffer = i->second->instanceBuffer;
		*vertexCount = i->second->vertexCount;
		*indexCount = i->second->indexCount;
		*instanceCount = i->second->instanceCount;

		for(const auto& t :triangles.at(filename))
		{
			GameWorld::Triangle* tri = new GameWorld::Triangle();
			tri->vertices[0] = static_cast<DirectX::XMFLOAT3>(DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(t->vertices[0]), worldMatrix));
			tri->vertices[1] = static_cast<DirectX::XMFLOAT3>(DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(t->vertices[1]), worldMatrix));
			tri->vertices[2] = static_cast<DirectX::XMFLOAT3>(DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(t->vertices[2]), worldMatrix));
			tri->CalculateGreatest();
			tri->CalculateSmallest();
			GameWorld::getInstance().AddTriangle(tri);
		}
		return true;
	}

	if(ModelFromFile(filename, device))
	{
		auto i = buffers.find(filename);

		if (i != buffers.end())
		{
			*vertexBuffer = i->second->vertexBuffer;
			*indexBuffer = i->second->indexBuffer;
			*instanceBuffer = i->second->instanceBuffer;
			*vertexCount = i->second->vertexCount;
			*indexCount = i->second->indexCount;
			*instanceCount = i->second->instanceCount;

			for (const auto& t : triangles.at(filename))
			{
				GameWorld::Triangle* tri = new GameWorld::Triangle();
				tri->vertices[0] = static_cast<DirectX::XMFLOAT3>(DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(t->vertices[0]), worldMatrix));
				tri->vertices[1] = static_cast<DirectX::XMFLOAT3>(DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(t->vertices[1]), worldMatrix));
				tri->vertices[2] = static_cast<DirectX::XMFLOAT3>(DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(t->vertices[2]), worldMatrix));
				tri->CalculateGreatest();
				tri->CalculateSmallest();
				GameWorld::getInstance().AddTriangle(tri);
			}
			return true;
		}
	} 

	std::cerr << "Couldn't Load Model!" << std::endl;
	return false;
}

std::vector<GameWorld::Triangle*> ModelLoader::GetTriangles(char* filename)
{
	return triangles.at(filename);
}

void ModelLoader::Release()
{
	for(auto i : buffers)
	{
		i.second->vertexBuffer->Release();
		i.second->indexBuffer->Release();
		i.second->instanceBuffer->Release();

		i.second->vertexBuffer = nullptr;
		i.second->indexBuffer = nullptr;
		i.second->instanceBuffer = nullptr;
	}
}

ModelLoader::ModelLoader()
{
}

ModelLoader::~ModelLoader()
{
}

bool ModelLoader::LoadModel(char* filename)
{
	std::vector<GameWorld::Triangle*> tempTris;
	std::ifstream fin;
	std::vector<tinyobj::shape_t> shapes;
	std::vector <tinyobj::material_t> materials;
	tinyobj::attrib_t attrib;
	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename);

	if (!err.empty()) { // `err` may contain warning message.
		std::cerr << err << std::endl;
	}

	if (!ret) {
		std::cerr << "Error with Tiny Obj Loader" << std::endl;
		exit(1);
	}
	
	unsigned long long allTriangles = 0;

	//Loop shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		allTriangles += shapes[s].mesh.num_face_vertices.size();
		tempTris.reserve(tempTris.size() + allTriangles);
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];

			GameWorld::Triangle* tri = new GameWorld::Triangle();
			int triindex = 0;

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				ModelType temp;
				temp.x = attrib.vertices[3 * idx.vertex_index + 0];
				temp.y = attrib.vertices[3 * idx.vertex_index + 1];
				temp.z = attrib.vertices[3 * idx.vertex_index + 2];
				temp.nx = attrib.normals[3 * idx.normal_index + 0];
				temp.ny = attrib.normals[3 * idx.normal_index + 1];
				temp.nz = attrib.normals[3 * idx.normal_index + 2];
				temp.tu = attrib.texcoords[2 * idx.texcoord_index + 0];
				temp.tv = attrib.texcoords[2 * idx.texcoord_index + 1];

				tri->vertices[triindex].x = temp.x;
				tri->vertices[triindex].y = temp.y;
				tri->vertices[triindex].z = temp.z;
				triindex++;

				lastmodel.push_back(temp);
			}
			index_offset += fv;

			tempTris.push_back(tri);

			// per-face material
			shapes[s].mesh.material_ids[f];
		}
	}
	
	triangles.insert_or_assign(filename, tempTris);

	lastTriple->vertexCount = lastmodel.size();
	lastTriple->indexCount = lastTriple->vertexCount;
	return true;
}

void ModelLoader::CalculateModelVectors()
{

	int faceCount, index;
	TempVertexType vertex1, vertex2, vertex3;
	VectorType tangent, binormal, normal;

	faceCount = lastTriple->vertexCount / 3;

	index = 0;

	for (auto i = 0; i < faceCount; ++i)
	{
		vertex1.x = lastmodel[index].x;
		vertex1.y = lastmodel[index].y;
		vertex1.z = lastmodel[index].z;
		vertex1.tu = lastmodel[index].tu;
		vertex1.tv = lastmodel[index].tv;
		vertex1.nx = lastmodel[index].nx;
		vertex1.ny = lastmodel[index].ny;
		vertex1.nz = lastmodel[index].nz;
		index++;

		vertex2.x = lastmodel[index].x;
		vertex2.y = lastmodel[index].y;
		vertex2.z = lastmodel[index].z;
		vertex2.tu = lastmodel[index].tu;
		vertex2.tv = lastmodel[index].tv;
		vertex2.nx = lastmodel[index].nx;
		vertex2.ny = lastmodel[index].ny;
		vertex2.nz = lastmodel[index].nz;
		index++;

		vertex3.x = lastmodel[index].x;
		vertex3.y = lastmodel[index].y;
		vertex3.z = lastmodel[index].z;
		vertex3.tu = lastmodel[index].tu;
		vertex3.tv = lastmodel[index].tv;
		vertex3.nx = lastmodel[index].nx;
		vertex3.ny = lastmodel[index].ny;
		vertex3.nz = lastmodel[index].nz;
		index++;

		CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);

		CalculateNormal(tangent, binormal, normal);

		lastmodel[index -1].nx = normal.x;
		lastmodel[index -1].ny = normal.y;
		lastmodel[index -1].nz = normal.z;
		lastmodel[index -1].tx = tangent.x;
		lastmodel[index -1].ty = tangent.y;
		lastmodel[index -1].tz = tangent.z;
		lastmodel[index -1].bx = binormal.x;
		lastmodel[index -1].by = binormal.y;
		lastmodel[index -1].bz = binormal.z;

		lastmodel[index -2].nx = normal.x;
		lastmodel[index -2].ny = normal.y;
		lastmodel[index -2].nz = normal.z;
		lastmodel[index -2].tx = tangent.x;
		lastmodel[index -2].ty = tangent.y;
		lastmodel[index -2].tz = tangent.z;
		lastmodel[index -2].bx = binormal.x;
		lastmodel[index -2].by = binormal.y;
		lastmodel[index -2].bz = binormal.z;

		lastmodel[index -3].nx = normal.x;
		lastmodel[index -3].ny = normal.y;
		lastmodel[index -3].nz = normal.z;
		lastmodel[index -3].tx = tangent.x;
		lastmodel[index -3].ty = tangent.y;
		lastmodel[index -3].tz = tangent.z;
		lastmodel[index -3].bx = binormal.x;
		lastmodel[index -3].by = binormal.y;
		lastmodel[index -3].bz = binormal.z;
	}
}

void ModelLoader::CalculateTangentBinormal(TempVertexType vertex1, TempVertexType vertex2, TempVertexType vertex3, VectorType& tangent, VectorType& binormal)
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

void ModelLoader::CalculateNormal(VectorType tangent, VectorType binormal, VectorType& normal)
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

bool ModelLoader::InitializeBuffers(ID3D11Device* device)
{
	std::vector<VertexType> vertices;
	InstanceType* instances;
	std::vector<unsigned long>  indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc, instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData, instanceData;
	HRESULT result;

	vertices.reserve(lastTriple->vertexCount); 
	indices.reserve(lastTriple->indexCount);

	//Load vertex and index array with data
	std::cout << "Starting vertice collection: " << lastmodel.size() << std::endl;
	int index = 0;
	for(auto it = lastmodel.begin(); it != lastmodel.end(); ++it)
	{
		VertexType temp;
		temp.position = DirectX::XMFLOAT3(it->x, it->y, it->z);
		temp.texture = DirectX::XMFLOAT2(it->tu, it->tv);
		temp.normal = DirectX::XMFLOAT3(it->nx, it->ny, it->nz);
		temp.tangent = DirectX::XMFLOAT3(it->tx, it->ty, it->tz);
		temp.binormal = DirectX::XMFLOAT3(it->bx, it->by, it->bz);

		vertices.push_back(temp);

		indices.push_back(index);
		index++;
	}
	std::cout << "Ending vertice collection" << std::endl;

	lastmodel.clear();

	//Set up static vertex buffer desc
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * lastTriple->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = &vertices[0];
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//Create Vertex Buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &lastTriple->vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//Set up static index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * lastTriple->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = &indices[0];
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &lastTriple->indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//Release arrays
	vertices.clear();
	indices.clear();

	//HARDCODED
	lastTriple->instanceCount = 1;

	instances = new InstanceType[lastTriple->instanceCount];
	if (!instances)
	{
		return false;
	}

	//Load instance array with data
	instances[0].position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	instances[0].scale = 1.0f;
	//HARDCODED ENDE


	//Instance Description
	instanceBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	instanceBufferDesc.ByteWidth = sizeof(InstanceType) * lastTriple->instanceCount;
	instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBufferDesc.CPUAccessFlags = 0;
	instanceBufferDesc.MiscFlags = 0;
	instanceBufferDesc.StructureByteStride = 0;

	instanceData.pSysMem = instances;
	instanceData.SysMemPitch = 0;
	instanceData.SysMemSlicePitch = 0;

	//Create Instance Buffer
	result = device->CreateBuffer(&instanceBufferDesc, &instanceData, &lastTriple->instanceBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//release instance array
	delete[] instances;
	instances = nullptr;

	return true;
}

bool ModelLoader::ModelFromFile(char* filename, ID3D11Device* device)
{
	bool result;
	lastTriple = new bufferStruct;

	std::cout << "Loading: " << filename << std::endl;

	LoadModel(filename);

	CalculateModelVectors();

	//Init vertex und index buffers
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	buffers.insert_or_assign(filename, lastTriple);

	return true;
}
