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

	//Translate to position of viewer
	lookAtVector = DirectX::XMVectorAdd(positionVector, lookAtVector);

	

	//Finally create view matrix
	viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

void CameraClass::GetViewMatrix(XMMATRIX& output)
{
	output = viewMatrix;
}
