#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
	direct3D = nullptr;
	camera = nullptr;
	shader = nullptr;
	light = nullptr;
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

	result = camera->Initialize(direct3D->GetDevice());
	if(!result)
	{
		return false;
	}

	camera->SetPosition(0.0f, 0.0f, -5.0f);

	//HARDCODED - Setting up Models

	//Scene
	models.push_back(InitializeModel(hwnd, "./Model/Cube.txt", L"./Model/companion_cube.dds", XMFLOAT3(-20.0f, 0.0f, 50.0f), 4.0f));
	models.push_back(InitializeModel(hwnd, "./Model/Cube.txt", L"./Model/companion_cube.dds", XMFLOAT3(20.0f, 0.0f, 150.0f), 4.0f));
	models.push_back(InitializeModel(hwnd, "./Model/Cube.txt", L"./Model/companion_cube.dds", XMFLOAT3(-20.0f, 0.0f, 250.0f), 4.0f));
	models.push_back(InitializeModel(hwnd, "./Model/Plane.txt", L"./Model/ground.dds", XMFLOAT3(0.0f, -4.0f, 0.0f), 100.0f));
	models.push_back(InitializeModel(hwnd, "./Model/Plane.txt", L"./Model/ground.dds", XMFLOAT3(0.0f, -4.0f, 200.0f), 100.0f));
	models.push_back(InitializeModel(hwnd, "./Model/Plane.txt", L"./Model/ground.dds", XMFLOAT3(0.0f, -4.0f, 400.0f), 100.0f));
	models.push_back(InitializeModel(hwnd, "./Model/Plane.txt", L"./Model/ground.dds", XMFLOAT3(200.0f, -4.0f, 0.0f), 100.0f));
	models.push_back(InitializeModel(hwnd, "./Model/Plane.txt", L"./Model/ground.dds", XMFLOAT3(200.0f, -4.0f, 200.0f), 100.0f));
	models.push_back(InitializeModel(hwnd, "./Model/Plane.txt", L"./Model/ground.dds", XMFLOAT3(200.0f, -4.0f, 400.0f), 100.0f));
	models.push_back(InitializeModel(hwnd, "./Model/Plane.txt", L"./Model/ground.dds", XMFLOAT3(-200.0f, -4.0f, 0.0f), 100.0f));
	models.push_back(InitializeModel(hwnd, "./Model/Plane.txt", L"./Model/ground.dds", XMFLOAT3(-200.0f, -4.0f, 200.0f), 100.0f));
	models.push_back(InitializeModel(hwnd, "./Model/Plane.txt", L"./Model/ground.dds", XMFLOAT3(-200.0f, -4.0f, 400.0f), 100.0f));

	//Lights
	light = new LightClass();
	if(!light)
	{
		return false;
	}
	light->SetDiffuseColor(1.0f, 0.0f, 1.0f, 1.0f);
	light->SetDirection(0.0f, 0.0f, 1.0f);

	//HARDCODED END
	
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
	if(light)
	{
		delete light;
		light = nullptr;
	}

	ShutdownModels();

	if(shader)
	{
		shader->Shutdown();
		delete shader;
		shader = nullptr;
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

	CheckWireframe(input);

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
	XMMATRIX viewMatrix, projectionMatrix;
	bool result;

	//clear Buffer at beginning
	direct3D->BeginScene(0.2f, 0.5f, 0.5f, 0.0f);

	//Generate view matrix based on camera
	camera->DoMovement(input);
	camera->Render();

	//Get world, view and proj matrices
	//direct3D->GetWorldMatrix(worldMatrix);
	camera->GetViewMatrix(viewMatrix);
	direct3D->GetProjectionMatrix(projectionMatrix);

	//Put model vertex and index buffer on pipeline
	for(ModelClass* model : models)
	{
		model->Render(direct3D->GetDeviceContext());

		//Render using shader
		result = shader->Render(direct3D->GetDeviceContext(), model->GetIndexCount(), model->GetInstanceCount(), model->worldMatrix, viewMatrix, projectionMatrix, model->GetTextureView(), light->GetDirection(), light->GetDiffuseColor());
		if(!result)
		{
			return false;
		}
	}

	//Render Trackingpoints
	for(ModelClass* model : camera->getTrackingPointsModels())
	{
		model->Render(direct3D->GetDeviceContext());

		//Render using shader
		result = shader->Render(direct3D->GetDeviceContext(), model->GetIndexCount(), model->GetInstanceCount(), model->worldMatrix, viewMatrix, projectionMatrix, model->GetTextureView(), light->GetDirection(), light->GetDiffuseColor());
		if (!result)
		{
			return false;
		}
	}

	//Output Buffer
	direct3D->EndScene();

	return true;
}

void GraphicsClass::CheckWireframe(InputClass* input)
{
	unsigned int zkey = 0x5A;

	if(!wireframeMode)
	{
		if(input->IsKeyDown(zkey) & !wireframeKeyToggle)
		{
			wireframeMode = true;
			wireframeKeyToggle = true;
			ChangeFillmode(D3D11_FILL_WIREFRAME);
		}	
	} else
	{
		if (input->IsKeyDown(zkey) & !wireframeKeyToggle)
		{
			wireframeMode = false;
			wireframeKeyToggle = true;
			ChangeFillmode(D3D11_FILL_SOLID);
		}
	}
	
	if(input->IsKeyUp(zkey))
	{
		wireframeKeyToggle = false;
	}
}

void GraphicsClass::ChangeFillmode(D3D11_FILL_MODE fillmode)
{
	ID3D11RasterizerState* rasterState;
	D3D11_RASTERIZER_DESC rasterStateDesc;

	direct3D->GetDeviceContext()->RSGetState(&rasterState);
	rasterState->GetDesc(&rasterStateDesc);
	rasterStateDesc.FillMode = fillmode;

	direct3D->GetDevice()->CreateRasterizerState(&rasterStateDesc, &rasterState);
	direct3D->GetDeviceContext()->RSSetState(rasterState);

	rasterState->Release();
	rasterState = nullptr;
}

ModelClass* GraphicsClass::InitializeModel(HWND hwnd, char* modelFilename, WCHAR* textureFilename, XMFLOAT3 position, float scale)
{
	bool result;
	ModelClass* model;

	//Set up model class
	model = new ModelClass();
	if (!model)
	{
		return false;
	}

	result = model->Initialize(direct3D->GetDevice(), modelFilename, textureFilename);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize model", L"Error", MB_OK);
		return false;
	}

	model->worldMatrix *= XMMatrixScaling(scale, scale, scale);
	model->worldMatrix *= XMMatrixTranslation(position.x, position.y, position.z);

	return model;
}

void GraphicsClass::ShutdownModels()
{
	for(ModelClass* model : models)
	{
		model->Shutdown();
		delete model;
	}

	models.clear();

}
