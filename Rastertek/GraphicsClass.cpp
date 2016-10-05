#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
	direct3D = nullptr;
	camera = nullptr;
	model = nullptr;
	shader = nullptr;
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

	result = camera->Initialize();
	if(!result)
	{
		return false;
	}

	camera->SetPosition(0.0f, 0.0f, -5.0f);

	//Set up model class
	model = new ModelClass();
	if(!model)
	{
		return false;
	}

	result = model->Initialize(direct3D->GetDevice(), "./Model/Cube.txt", L"./Model/hearts.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize model", L"Error", MB_OK);
		return false;
	}

	//Set up shader
	shader = new ShaderClass();
	if(!shader)
	{
		return false;
	}

	result = shader->Initialize(direct3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize shader object", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	if(shader)
	{
		shader->Shutdown();
		delete shader;
		shader = nullptr;
	}

	if(model)
	{
		model->Shutdown();
		delete model;
		model = nullptr;
	}

	if(camera)
	{
		camera->Shutdown();
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

bool GraphicsClass::Frame(InputClass* input)
{
	bool result;
	static float rotation = 0.0f;

	rotation += static_cast<float>(XM_PI * 0.001f);
	if(rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	result = Render(rotation, input);
	if(!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render(float rotation, InputClass* input)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	//clear Buffer at beginning
	direct3D->BeginScene(0.9f, 0.5f, 1.0f, 0.0f);

	//Generate view matrix based on camera
	camera->DoMovement(input);
	camera->Render();

	//Get world, view and proj matrices
	direct3D->GetWorldMatrix(worldMatrix);
	camera->GetViewMatrix(viewMatrix);
	direct3D->GetProjectionMatrix(projectionMatrix);

	//Use rotation
	//worldMatrix *= XMMatrixRotationX(rotation);
	//worldMatrix *= XMMatrixRotationY(rotation);

	//Put model vertex and index buffer on pipeline
	model->Render(direct3D->GetDeviceContext());

	//Render using shader
	result = shader->Render(direct3D->GetDeviceContext(), model->GetIndexCount(), model->GetInstanceCount(), worldMatrix, viewMatrix, projectionMatrix, model->GetTextureView());
	if(!result)
	{
		return false;
	}

	//Output Buffer
	direct3D->EndScene();

	return true;
}