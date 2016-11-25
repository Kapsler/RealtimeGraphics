#pragma once

#include <d3d11.h>
#include "InputClass.h"
#include "SimpleMath.h"
#include "TimerClass.h"
#include <vector>
#include "ModelClass.h"
using namespace DirectX;
using namespace SimpleMath;

class CameraClass
{
public:
	struct ControlPoint
	{
		Vector3 position;
		Quaternion direction;
	};

	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	bool Initialize(ID3D11Device*);
	void Shutdown();

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void DoMovement(InputClass*);
	std::vector<ModelClass*> getTrackingPointsModels();

	void Render();
	void GetViewMatrix(XMMATRIX&);

private:

	ControlPoint* generatePoint(float px, float py, float pz, float ox, float oy, float oz, float ow);
	ControlPoint* generatePoint(Vector3 pos, Quaternion rot);
	ControlPoint* generatePoint(ControlPoint* other);
	void kochanekBartels(Vector3*, Quaternion*, float, int*);
	void resetTrackingPoints();
	void addTrackingPoint();
	ModelClass* InitializeTrackingPointModel(char*, WCHAR*, WCHAR*, Vector3, float, Quaternion);
	void calculateTrack();
	void generateModels();


	ID3D11Device* device;
	Vector3 position, rotation;
	Matrix viewMatrix;
	TimerClass* timer;
	bool tracking, trackingKeyToggle, addPointKeyToggle;
	Quaternion viewQuaternion;
	std::vector<ControlPoint*> trackingPoints;
	std::vector<ControlPoint*> kochanekPoints;
	float trackingProgress;
	int currentTrackingPoint;
	bool useSquad;
	std::vector<ModelClass*> trackingPointsModels;
};