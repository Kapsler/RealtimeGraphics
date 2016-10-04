#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
	direct3D = nullptr;
	camera = nullptr;
	model = nullptr;
	colorShader = nullptr;
}

GraphicsClass::GraphicsClass(const GraphicsClass&)
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	//Set up Direct3D
	direct3D = new D3DClass();
	if(!direct3D)
	{
		return false;
	}

	result = direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	//Set up Camera
	camera = new CameraClass();
	if(!camera)
	{
		return false;
	}

	camera->SetPosition(0.0f, 0.0f, -5.0f);
	camera->SetRotation(1.0f, 5.5f, 1.0f);

	//Set up model class
	model = new ModelClass();
	if(!model)
	{
		return false;
	}

	result = model->Initialize(direct3D->GetDevice(), "./Model/Cube.txt");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize model", L"Error", MB_OK);
		return false;
	}

	//Set up color shader
	colorShader = new ColorShaderClass();
	if(!colorShader)
	{
		return false;
	}

	result = colorShader->Initialize(direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize color shader object", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	if(colorShader)
	{
		colorShader->Shutdown();
		delete colorShader;
		colorShader = nullptr;
	}

	if(model)
	{
		model->Shutdown();
		delete model;
		model = nullptr;
	}

	if(camera)
	{
		delete camera;
		camera = nullptr;
	}

	if(direct3D)
	{
		direct3D->Shutdown();
		delete direct3D;
		direct3D = nullptr;
	}
}

bool GraphicsClass::Frame()
{
	bool result;

	result = Render();
	if(!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	//clear Buffer at beginning
	direct3D->BeginScene(0.9f, 0.5f, 1.0f, 0.0f);

	//Generate view matrix based on camera
	camera->Render();

	//Get world, view and proj matrices
	direct3D->GetWorldMatrix(worldMatrix);
	camera->GetViewMatrix(viewMatrix);
	direct3D->GetProjectionMatrix(projectionMatrix);

	//Put model vertex and index buffer on pipeline
	model->Render(direct3D->GetDeviceContext());

	result = colorShader->Render(direct3D->GetDeviceContext(), model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if(!result)
	{
		return false;
	}

	//Output Buffer
	direct3D->EndScene();

	return true;
}