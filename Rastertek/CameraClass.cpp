#include "CameraClass.h"
#include <iostream>

CameraClass::CameraClass()
{
	position.x = 0.0f;
	position.y = 0.0f;
	position.z = 0.0f;

	rotation.x = 0.0f;
	rotation.y = 0.0f;
	rotation.z = 0.0f;

	tracking = false;
	useSquad = true;
}

CameraClass::CameraClass(const CameraClass&)
{
}

CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y= y;
	position.z = z;	
}

void CameraClass::SetRotation(float x, float y, float z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
}

bool CameraClass::Initialize(ID3D11Device* odevice)
{
	bool result;

	device = odevice;

	timer = new TimerClass();
	if(!timer)
	{
		return false;
	}

	result = timer->Initialize();
	if(!result)
	{
		return false;
	}
	
	//HARDCODED

	//Setting up tracking progress
	trackingProgress = 0.0f;
	currentTrackingPoint = 1;

	//Setting predefined Tracking points
	trackingPoints.push_back(generatePoint(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f));
	trackingPoints.push_back(generatePoint(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f));
	trackingPoints.push_back(generatePoint(50.0f, 0.0f, 50.0f, 0.0f, 0.0f, 0.0f, 0.0f));
	trackingPoints.push_back(generatePoint(0.0f, 0.0f, 100.0f, 0.0f, 0.0f, 0.0f, 0.0f));
	trackingPoints.push_back(generatePoint(-50.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f));
	trackingPoints.push_back(generatePoint(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f));

	trackingPoints.push_back(generatePoint(-30.0f, 0.0f, 25.0f, 0.0f, 0.0f, 0.0f, 0.0f));
	trackingPoints.push_back(generatePoint(-30.0f, 10.0f, 75.0f, 0.0f, 0.0f, 0.0f, 0.0f));
	trackingPoints.push_back(generatePoint(30.0f, 5.0f, 125.0f, 0.0f, 0.0f, 0.0f, 0.0f));
	trackingPoints.push_back(generatePoint(30.0f, 0.0f, 175.0f, 0.0f, 0.0f, 0.0f, 0.0f));
	trackingPoints.push_back(generatePoint(-30.0f, 15.0f, 225.0f, 0.0f, 0.0f, 0.0f, 0.0f));
	trackingPoints.push_back(generatePoint(-30.0f, 5.0f, 275.0f, 0.0f, 0.0f, 0.0f, 0.0f));
	trackingPoints.push_back(generatePoint(0.0f, 0.0f, 300.0f, 0.0f, 0.0f, 0.0f, 0.0f));
	
	calculateTrack();
	generateModels();

	//HARDCODED END

	return true;
}

void CameraClass::Shutdown()
{	
	resetTrackingPoints();

	if(timer)
	{
		delete timer;
		timer = nullptr;
	}
}

XMFLOAT3 CameraClass::GetPosition()
{
	return XMFLOAT3(position);
}

XMFLOAT3 CameraClass::GetRotation()
{
	return XMFLOAT3(rotation);
}

void CameraClass::DoMovement(InputClass* input)
{
	unsigned int wkey = 0x57;
	unsigned int skey = 0x53;
	unsigned int akey = 0x41;
	unsigned int dkey = 0x44;
	unsigned int tkey = 0x54;
	unsigned int rkey = 0x52;
	unsigned int ekey = 0x45;
	Vector3 movementDirection;
	timer->Frame();


	float deltaTime = timer->GetFrameTime();
	float cameraSpeed = 0.1f * deltaTime;
	viewQuaternion.Inverse(viewQuaternion);

	if(!tracking)
	{
		//Movement
		if (input->IsKeyDown(wkey))
		{
			movementDirection = Vector3::Transform(Vector3::Forward, viewQuaternion);
			position -=  movementDirection * cameraSpeed;
			
		}
		if (input->IsKeyDown(skey))
		{
			movementDirection = Vector3::Transform(Vector3::Forward, viewQuaternion);
			position += cameraSpeed * movementDirection;
		}
		if (input->IsKeyDown(akey))
		{
			movementDirection = Vector3::Transform(Vector3::Forward.Cross(Vector3::Up), viewQuaternion);
			position -= cameraSpeed * movementDirection;
		}
		if (input->IsKeyDown(dkey))
		{
			movementDirection = Vector3::Transform(Vector3::Forward.Cross(Vector3::Up), viewQuaternion);
			position += cameraSpeed * movementDirection;
		}

		//Rotation
		if(input->IsKeyDown(VK_UP))
		{
			rotation.x -= cameraSpeed ;
		}
		if(input->IsKeyDown(VK_DOWN))
		{
			rotation.x += cameraSpeed;
		}
		if(input->IsKeyDown(VK_LEFT))
		{
			rotation.y -= cameraSpeed * 2;
		}
		if(input->IsKeyDown(VK_RIGHT))
		{
			rotation.y += cameraSpeed * 2;
		}

		//reset Tracking points
		if (input->IsKeyDown(rkey))
		{
			resetTrackingPoints();
		}

		//add tracking point
		if (input->IsKeyDown(ekey) && !addPointKeyToggle)
		{
			addTrackingPoint();
			addPointKeyToggle = true;
		}

		//Toggle Tracking
		if (input->IsKeyDown(tkey) && !trackingKeyToggle)
		{
			tracking = true;
			trackingKeyToggle = true;
		}
		
	} else
	{
		//Toggle Tracking
		if (input->IsKeyDown(tkey) && !trackingKeyToggle)
		{
			tracking = false;
			trackingKeyToggle = true;
		}
	}

	//Tracking Toggle Flag
	if(input->IsKeyUp(tkey))
	{
		trackingKeyToggle = false;
	}

	//addPoint Toggle Flag
	if(input->IsKeyUp(ekey))
	{
		addPointKeyToggle = false;
	}

}

std::vector<ModelClass*> CameraClass::getTrackingPointsModels()
{
	return trackingPointsModels;
}

void CameraClass::Render()
{
	float yaw, pitch, roll;

	//Set Rotation in radians
	pitch = rotation.x * 0.0174532925f;
	yaw = rotation.y * 0.0174532925f;
	roll = rotation.z * 0.0174532925f;

	viewQuaternion = Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll);

	if (tracking)
	{
		float deltaTime = timer->GetFrameTime();
		float movementSpeed = deltaTime * 0.0005f;
		trackingProgress += movementSpeed;


		kochanekBartels(&position, &viewQuaternion, trackingProgress, &currentTrackingPoint);

		if (trackingProgress >= 1.0f)
		{
			trackingProgress = 0;
			currentTrackingPoint += 1;
		}
	}
	else
	{
		trackingProgress = 0.0f;
		currentTrackingPoint = 1;
	}


	//Finally create view matrix
	//viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
	viewQuaternion.Inverse(viewQuaternion);
	viewMatrix = Matrix::Identity;
	//Translate to position of viewer
	viewMatrix = viewMatrix.Transform(Matrix::CreateTranslation(-position), viewQuaternion);
}

void CameraClass::GetViewMatrix(XMMATRIX& output)
{
	output = viewMatrix;
}

CameraClass::ControlPoint* CameraClass::generatePoint(float px, float py, float pz, float ox, float oy, float oz, float ow)
{
	ControlPoint* p = new ControlPoint();

	p->position.x = px;
	p->position.y = py;
	p->position.z = pz;

	p->direction.x = ox;
	p->direction.y = oy;
	p->direction.z = oz;
	p->direction.w = ow;

	return p;
}

CameraClass::ControlPoint* CameraClass::generatePoint(Vector3 pos, Quaternion rot)
{
	ControlPoint* p = new ControlPoint();

	p->position = pos;
	p->direction = rot;

	return p;
}

CameraClass::ControlPoint* CameraClass::generatePoint(ControlPoint* other)
{
	ControlPoint* p = new ControlPoint();

	p->position = other->position;
	p->direction = other->direction;

	return p;
}

void CameraClass::kochanekBartels(Vector3* pos, Quaternion* rot, float progress, int* currentPoint)
{
	//LookAt interpolation zwischen outgoing tangenten
	Vector3 tangent1, tangent2;
	float a, b;

	a = 0.0f;
	b = 0.0f;

	if(*currentPoint - 1 >= 0 && *currentPoint + 2 < trackingPoints.size())
	{
		tangent1 = (((1 - a) * (1 + b)) / 2) * (trackingPoints[*currentPoint]->position - trackingPoints[*currentPoint -1]->position) + (((1 - a) * (1 - b)) /2) * (trackingPoints[*currentPoint +1]->position - trackingPoints[*currentPoint]->position);
		if(*currentPoint + 2 >= trackingPoints.size())
		{
			tangent2 = tangent1;
		} else
		{
			tangent2 = (((1 - a) * (1 + b)) / 2) * (trackingPoints[*currentPoint +1]->position - trackingPoints[*currentPoint]->position) + (((1 - a) * (1 - b)) /2) * (trackingPoints[*currentPoint +2]->position - trackingPoints[*currentPoint +1]->position);
		}
		
		//position = *(trackingPoints[currentTrackingPoint]) * (1 - progress) + *(trackingPoints[currentTrackingPoint+1]) * progress;
		*pos = pos->Hermite(trackingPoints[*currentPoint]->position, tangent1, trackingPoints[*currentPoint +1]->position, tangent2, progress);

		if (progress != 0.0f) {
			if(useSquad)
			{
				XMVECTOR A, B, C;

				XMQuaternionSquadSetup(&A, &B, &C, trackingPoints[*currentPoint - 1]->direction, trackingPoints[*currentPoint]->direction, trackingPoints[*currentPoint + 1]->direction, trackingPoints[*currentPoint +2]->direction);

				*rot = XMQuaternionSquad(trackingPoints[*currentPoint]->direction, A, B, C, progress);
			} else
			{
				*rot = Quaternion::Slerp(trackingPoints[*currentPoint]->direction, trackingPoints[*currentPoint + 1]->direction, progress);
			}
		}
	} else
	{
		tracking = false;
		*currentPoint = 1;
	}
}

void CameraClass::resetTrackingPoints()
{
	for (ControlPoint* p : trackingPoints)
	{
		delete p;
	}

	for (ModelClass* p : trackingPointsModels)
	{
		p->Shutdown();
		delete p;
	}

	trackingPoints.clear();
	trackingPointsModels.clear();
}

void CameraClass::addTrackingPoint()
{

	ControlPoint* p = new ControlPoint();

	p->position = position;
	p->direction = viewQuaternion;

	trackingPoints.push_back(p);

	calculateTrack();
	generateModels();
}

ModelClass* CameraClass::InitializeTrackingPointModel(char* modelFilename, WCHAR* textureFilename1, WCHAR* textureFilename2, Vector3 pos, float scale, Quaternion rot)
{
	bool result;
	ModelClass* model;

	//Set up model class
	model = new ModelClass();
	if (!model)
	{
		return nullptr;
	}

	result = model->Initialize(device, modelFilename, textureFilename1,textureFilename2);
	if (!result)
	{
		
		std::cerr << "Error initializing model" << endl;
	}

	model->worldMatrix *= XMMatrixRotationQuaternion(rot);
	model->worldMatrix *= XMMatrixScaling(scale, scale, scale);
	model->worldMatrix *= XMMatrixTranslation(pos.x, pos.y, pos.z);

	return model;
}

void CameraClass::calculateTrack()
{
	//need second vector for points between controlpoints

	for(ControlPoint* p : kochanekPoints)
	{
		delete p;
	}

	kochanekPoints.clear();

	if(trackingPoints.size() > 2)
	{

		if (trackingPoints[trackingPoints.size() - 2]->position == trackingPoints[trackingPoints.size() - 3]->position && trackingPoints[trackingPoints.size() - 2]->direction == trackingPoints[trackingPoints.size() - 3]->direction)
		{
			trackingPoints.erase(trackingPoints.end() - 2);
		}
	}

	if (trackingPoints.size() > 1)
	{
		if (trackingPoints[0]->position != trackingPoints[1]->position && trackingPoints[0]->direction != trackingPoints[1]->direction)
		{
			trackingPoints.insert(trackingPoints.begin(), generatePoint(trackingPoints[0]));
		}

		if (trackingPoints[trackingPoints.size() - 1]->position != trackingPoints[trackingPoints.size() - 2]->position && trackingPoints[trackingPoints.size() - 1]->direction != trackingPoints[trackingPoints.size() - 2]->direction)
		{
			trackingPoints.push_back(generatePoint(trackingPoints[trackingPoints.size() - 1]));
		}
	}

	std::vector<ControlPoint*> newPointsVector;

	int numberOfPoints = static_cast<int>(trackingPoints.size());

	if(numberOfPoints > 1)
	{
		for(int i = 0; i < numberOfPoints; i++)
		{
			int tmp = i;
			for(float t = 0; t <= 1.0f; t += 1.0f/5.0f)
			{
				ControlPoint* p = new ControlPoint();
				kochanekBartels(&(*p).position, &(*p).direction, t, &tmp);
				newPointsVector.push_back(p);
			}
		}
	}
	

	kochanekPoints = newPointsVector;
	

}

void CameraClass::generateModels()
{

	for (ModelClass* p : trackingPointsModels)
	{
		p->Shutdown();
		delete p;
	}

	trackingPointsModels.clear();

	for(ControlPoint* p : trackingPoints)
	{
		trackingPointsModels.push_back(InitializeTrackingPointModel("./Model/Cube.txt", L"./Model/companion_cube.dds", L"./Model/companion_cubebump.dds", p->position, 0.8f, p->direction));
	}

	for(ControlPoint* p : kochanekPoints)
	{
		trackingPointsModels.push_back(InitializeTrackingPointModel("./Model/Cube.txt", L"./Model/companion_cube.dds", L"./Model/companion_cubebump.dds", p->position, 0.3f, p->direction));
	}

}
