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
	
	//HARDCODED DIRECTIONS

	//Where is up
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	//Set default look direction
	forward.x = 0.0f;
	forward.y = 0.0f;
	forward.z = 1.0f;

	//Setting up tracking progress
	trackingProgress = 0.0f;
	currentTrackingPoint = 1;

	//Setting predefined Tracking points
	trackingPoints.push_back(new Vector3(0.0f, 0.0f, 0.0f));
	trackingPoints.push_back(new Vector3(0.0f, 0.0f, 0.0f));
	trackingPoints.push_back(new Vector3(50.0f, 0.0f, 50.0f));
	trackingPoints.push_back(new Vector3(0.0f, 0.0f, 100.0f));
	trackingPoints.push_back(new Vector3(-50.0f, 0.0f, 0.0f));
	trackingPoints.push_back(new Vector3(0.0f, 0.0f, 0.0f));

	trackingPoints.push_back(new Vector3(-30.0f, 0.0f, 25.0f));
	trackingPoints.push_back(new Vector3(-30.0f, 10.0f, 75.0f));
	trackingPoints.push_back(new Vector3(30.0f, 5.0f, 125.0f));
	trackingPoints.push_back(new Vector3(30.0f, 0.0f, 175.0f));
	trackingPoints.push_back(new Vector3(-30.0f, 15.0f, 225.0f));
	trackingPoints.push_back(new Vector3(-30.0f, 5.0f, 275.0f));
	trackingPoints.push_back(new Vector3(0.0f, 0.0f, 300.0f));

	//HARDCODED END

	return true;
}

void CameraClass::Shutdown()
{
	if(timer)
	{
		delete timer;
		timer = nullptr;
	}

	for(Vector3* tmp : trackingPoints)
	{
		delete tmp;
		tmp = nullptr;
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
	Vector3 movementDirection;
	timer->Frame();


	float deltaTime = timer->GetFrameTime();
	float cameraSpeed = 0.03f * deltaTime;

	if(!tracking)
	{

		//Movement
		if (input->IsKeyDown(wkey))
		{
			movementDirection = lookAt;
			movementDirection.Normalize();
			position += cameraSpeed * movementDirection;
		}
		if (input->IsKeyDown(skey))
		{
			movementDirection = lookAt;
			movementDirection.Normalize();
			position -= cameraSpeed * movementDirection;
		}
		if (input->IsKeyDown(akey))
		{
			movementDirection = lookAt.Cross(up);
			movementDirection.Normalize();
			position += cameraSpeed * movementDirection;
		}
		if (input->IsKeyDown(dkey))
		{
			movementDirection = lookAt.Cross(up);
			movementDirection.Normalize();
			position -= cameraSpeed * movementDirection;
		}

		//Rotation
		if(input->IsKeyDown(VK_UP))
		{
			rotation.x -= cameraSpeed * 2;
		}
		if(input->IsKeyDown(VK_DOWN))
		{
			rotation.x += cameraSpeed * 2;
		}
		if(input->IsKeyDown(VK_LEFT))
		{
			rotation.y -= cameraSpeed * 4;
		}
		if(input->IsKeyDown(VK_RIGHT))
		{
			rotation.y += cameraSpeed * 4;
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

}

std::vector<Vector3*> CameraClass::getTrackingPoints()
{
	return trackingPoints;
}

void CameraClass::Render()
{
	XMVECTOR upVector, positionVector, lookAtVector;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	upVector = up;
	positionVector = position;
	lookAtVector = forward;

	//Set Rotation in radians
	pitch = rotation.x * 0.0174532925f;
	yaw = rotation.y * 0.0174532925f;
	roll = rotation.z * 0.0174532925f;

	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	//Tranform vectors by rotationMatrix, so view is correctly rotated to origin
	lookAtVector = DirectX::XMVector3TransformCoord(lookAtVector, rotationMatrix);

	upVector = DirectX::XMVector3TransformCoord(upVector, rotationMatrix);

	lookAt = XMVector3Normalize(lookAtVector);

	if (tracking)
	{
		lookAtVector = kochanekBartels();
		if(position != lastPosition)
		{
			//lookAtVector = XMVector3Normalize(position - lastPosition);
		}
	}
	else
	{
		trackingProgress = 0.0f;
		currentTrackingPoint = 1;
	}

	//Translate to position of viewer
	lookAtVector = DirectX::XMVectorAdd(positionVector, lookAtVector);

	//Finally create view matrix
	viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	lastPosition = position;
}

void CameraClass::GetViewMatrix(XMMATRIX& output)
{
	output = viewMatrix;
}

Vector3 CameraClass::kochanekBartels()
{
	//LookAt interpolation zwischen outgoing tangenten

	float deltaTime = timer->GetFrameTime();
	float movementSpeed = deltaTime * 0.0005f;
	Vector3 tangent1, tangent2, viewDirection;
	float a, b;

	a = 0.0f;
	b = 0.0f;

	if(currentTrackingPoint - 1 >= 0 && currentTrackingPoint + 2 < trackingPoints.size())
	{
		trackingProgress += movementSpeed;

		tangent1 = (((1 - a) * (1 + b)) / 2) * (*trackingPoints[currentTrackingPoint] - *trackingPoints[currentTrackingPoint-1]) + (((1 - a) * (1 - b)) /2) * (*trackingPoints[currentTrackingPoint+1] - *trackingPoints[currentTrackingPoint]);
		if(currentTrackingPoint + 2 >= trackingPoints.size())
		{
			tangent2 = tangent1;
		} else
		{
			tangent2 = (((1 - a) * (1 + b)) / 2) * (*trackingPoints[currentTrackingPoint+1] - *trackingPoints[currentTrackingPoint]) + (((1 - a) * (1 - b)) /2) * (*trackingPoints[currentTrackingPoint+2] - *trackingPoints[currentTrackingPoint+1]);
		}
		
		//position = *(trackingPoints[currentTrackingPoint]) * (1 - progress) + *(trackingPoints[currentTrackingPoint+1]) * progress;
		position = position.Hermite(*(trackingPoints[currentTrackingPoint]), tangent1, *trackingPoints[currentTrackingPoint+1], tangent2, trackingProgress);

		if (trackingProgress != 0.0f) {
			viewDirection = viewDirection.Lerp(tangent1, tangent2, trackingProgress);
			//viewDirection = viewDirection.Lerp(*trackingPoints[currentTrackingPoint] - *trackingPoints[currentTrackingPoint-1], *trackingPoints[currentTrackingPoint+1] - *trackingPoints[currentTrackingPoint], trackingProgress);
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


	return viewDirection;

}
