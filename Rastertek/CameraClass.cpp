#include "CameraClass.h"

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

bool CameraClass::Initialize()
{
	bool result;

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


	//HARDCODED END

	return true;
}

void CameraClass::Shutdown()
{	
	for(ControlPoint* tmp : trackingPoints)
	{
		delete tmp;
	}

	trackingPoints.clear();

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

std::vector<CameraClass::ControlPoint*> CameraClass::getTrackingPoints()
{
	return trackingPoints;
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
		viewQuaternion = kochanekBartels();
	}
	else
	{
		trackingProgress = 0.0f;
		currentTrackingPoint = 1;
	}

	//Translate to position of viewer

	//Finally create view matrix
	//viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
	viewQuaternion.Inverse(viewQuaternion);
	viewMatrix = Matrix::Identity;
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

Quaternion CameraClass::kochanekBartels()
{
	//LookAt interpolation zwischen outgoing tangenten
	
	float deltaTime = timer->GetFrameTime();
	float movementSpeed = deltaTime * 0.0005f;
	Vector3 tangent1, tangent2;
	Quaternion direction;
	float a, b;

	a = 0.0f;
	b = 0.0f;

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
	if(currentTrackingPoint - 1 >= 0 && currentTrackingPoint + 2 < trackingPoints.size())
	{
		trackingProgress += movementSpeed;

		tangent1 = (((1 - a) * (1 + b)) / 2) * (trackingPoints[currentTrackingPoint]->position - trackingPoints[currentTrackingPoint-1]->position) + (((1 - a) * (1 - b)) /2) * (trackingPoints[currentTrackingPoint+1]->position - trackingPoints[currentTrackingPoint]->position);
		if(currentTrackingPoint + 2 >= trackingPoints.size())
		{
			tangent2 = tangent1;
		} else
		{
			tangent2 = (((1 - a) * (1 + b)) / 2) * (trackingPoints[currentTrackingPoint+1]->position - trackingPoints[currentTrackingPoint]->position) + (((1 - a) * (1 - b)) /2) * (trackingPoints[currentTrackingPoint+2]->position - trackingPoints[currentTrackingPoint+1]->position);
		}
		
		//position = *(trackingPoints[currentTrackingPoint]) * (1 - progress) + *(trackingPoints[currentTrackingPoint+1]) * progress;
		position = position.Hermite(trackingPoints[currentTrackingPoint]->position, tangent1, trackingPoints[currentTrackingPoint+1]->position, tangent2, trackingProgress);

		if (trackingProgress != 0.0f) {
			if(useSquad)
			{
				XMVECTOR A, B, C;

				XMQuaternionSquadSetup(&A, &B, &C, trackingPoints[currentTrackingPoint - 1]->direction, trackingPoints[currentTrackingPoint]->direction, trackingPoints[currentTrackingPoint + 1]->direction, trackingPoints[currentTrackingPoint +2]->direction);

				direction = XMQuaternionSquad(trackingPoints[currentTrackingPoint]->direction, A, B, C, trackingProgress);
			} else
			{
				direction = Quaternion::Slerp(trackingPoints[currentTrackingPoint]->direction, trackingPoints[currentTrackingPoint + 1]->direction, trackingProgress);
			}
		}

		if(trackingProgress >= 1.0f)
		{
			trackingProgress = 0;
			currentTrackingPoint += 1;
		}
	} else
	{
		tracking = false;
		currentTrackingPoint = 1;
	}

	return direction;
}

void CameraClass::resetTrackingPoints()
{
	for(ControlPoint* p : trackingPoints)
	{
		delete p;
	}

	trackingPoints.clear();
}

void CameraClass::addTrackingPoint()
{

	ControlPoint* p = new ControlPoint();

	p->position = position;
	p->direction = viewQuaternion;

	trackingPoints.push_back(p);
}
