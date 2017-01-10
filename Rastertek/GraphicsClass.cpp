#include "GraphicsClass.h"
#include <memory>
#include <SpriteFont.h>
#include <SimpleMath.inl>
#include <string>
#include <iostream>
#include "ModelLoader.h"

GraphicsClass::GraphicsClass()
{
	direct3D = nullptr;
	camera = nullptr;
	shader = nullptr;
	light = nullptr;
	light2 = nullptr;
	renderTexture = nullptr;
	renderTexture2 = nullptr;
	timer = nullptr;
	depthShader = nullptr;
	bumpiness = 1;
	screenBuffer = nullptr;
	screenTargetView = nullptr;
	depthBuffer = nullptr;
	depthTargetView = nullptr;
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
	
	currentScreenWidth = screenWidth;
	currentScreenHeight = screenHeight;
	hwndptr = &hwnd;

	//Set up Direct3D
	direct3D = new D3DClass();
	if (!direct3D)
	{
		return false;
	}

	result = direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	//Set up Camera
	camera = new CameraClass();
	if (!camera)
	{
		return false;
	}

	result = camera->Initialize(direct3D->GetDevice());
	if (!result)
	{
		return false;
	}

	camera->SetPosition(0.0f, 0.0f, -5.0f);


	timer = new TimerClass();
	if (!timer)
	{
		return false;
	}

	result = timer->Initialize();
	if (!result)
	{
		return false;
	}

	//HARDCODED - Setting up Models

	//models.push_back(InitializeModel(hwnd, "./Model/rungholt.obj", L"./Model/brickwall.dds", L"./Model/brickbump.dds", XMFLOAT3(-40, 0.0f, 50.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/hairball.obj", L"./Model/ground.dds", L"./Model/brickbump.dds", XMFLOAT3(-40, 0.0f, 50.0f), 500.0f));
	models.push_back(InitializeModel(hwnd, "./Model/buddha.obj", L"./Model/brickwall.dds", L"./Model/brickbump.dds", XMFLOAT3(-400, 0.0f, 50.0f), 500.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/house.obj", L"./Model/brickwall.dds", L"./Model/brickbump.dds", XMFLOAT3(-40, 0.0f, 50.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/house.obj", L"./Model/brickwall.dds", L"./Model/brickbump.dds", XMFLOAT3(-400, 0.0f, 500.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/house.obj", L"./Model/brickwall.dds", L"./Model/brickbump.dds", XMFLOAT3(-200, 0.0f, 200.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/house.obj", L"./Model/brickwall.dds", L"./Model/brickbump.dds", XMFLOAT3(200, 0.0f, -200.0f), 4.0f));
	models.push_back(InitializeModel(hwnd, "./Model/house.obj", L"./Model/brickwall.dds", L"./Model/brickbump.dds", XMFLOAT3(400, 0.0f, -400.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/house.obj", L"./Model/brickwall.dds", L"./Model/brickbump.dds", XMFLOAT3(0, 200.0f, 200.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/house.obj", L"./Model/brickwall.dds", L"./Model/brickbump.dds", XMFLOAT3(-400, 0.0f, 400.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/house.obj", L"./Model/brickwall.dds", L"./Model/brickbump.dds", XMFLOAT3(0, -200.0f, -200.0f), 4.0f));

	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/brickwall.dds", L"./Model/brickbump.dds", XMFLOAT3(-40, 0.0f, 50.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/brickwall.dds", L"./Model/brickbump.dds", XMFLOAT3(-50, 0.0f, 50.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/brickwall.dds", L"./Model/brickbump.dds", XMFLOAT3(-60, 0.0f, 50.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/brickwall.dds", L"./Model/brickbump.dds", XMFLOAT3(-70, 0.0f, 50.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/brickwall.dds", L"./Model/brickbump.dds", XMFLOAT3(-80, 0.0f, 50.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/brickwall.dds", L"./Model/brickbump.dds", XMFLOAT3(-90, 0.0f, 50.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/brickwall.dds", L"./Model/brickbump.dds", XMFLOAT3(-100, 0.0f, 50.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/brickwall.dds", L"./Model/brickbump.dds", XMFLOAT3(-110, 0.0f, 50.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/brickwall.dds", L"./Model/brickbump.dds", XMFLOAT3(-120, 0.0f, 50.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/brickwall.dds", L"./Model/brickbump.dds", XMFLOAT3(-130, 0.0f, 50.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/brickwall.dds", L"./Model/brickbump.dds", XMFLOAT3(-140, 0.0f, 50.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/brickwall.dds", L"./Model/brickbump.dds", XMFLOAT3(-150, 0.0f, 50.0f), 4.0f));

	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/icetex.dds", L"./Model/icenormal.dds", XMFLOAT3(30.0f, 0.0f, 150.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/icetex.dds", L"./Model/icenormal.dds", XMFLOAT3(40.0f, 0.0f, 150.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/icetex.dds", L"./Model/icenormal.dds", XMFLOAT3(50.0f, 0.0f, 150.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/icetex.dds", L"./Model/icenormal.dds", XMFLOAT3(60.0f, 0.0f, 150.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/icetex.dds", L"./Model/icenormal.dds", XMFLOAT3(70.0f, 0.0f, 150.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/icetex.dds", L"./Model/icenormal.dds", XMFLOAT3(80.0f, 0.0f, 150.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/icetex.dds", L"./Model/icenormal.dds", XMFLOAT3(90.0f, 0.0f, 150.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/icetex.dds", L"./Model/icenormal.dds", XMFLOAT3(100.0f, 0.0f, 150.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/icetex.dds", L"./Model/icenormal.dds", XMFLOAT3(110.0f, 0.0f, 150.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/icetex.dds", L"./Model/icenormal.dds", XMFLOAT3(120.0f, 0.0f, 150.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/icetex.dds", L"./Model/icenormal.dds", XMFLOAT3(130.0f, 0.0f, 150.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/icetex.dds", L"./Model/icenormal.dds", XMFLOAT3(140.0f, 0.0f, 150.0f), 4.0f));


	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/stone01.dds", L"./Model/bump01.dds", XMFLOAT3(-20, 0.0f, 250.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/stone01.dds", L"./Model/bump01.dds", XMFLOAT3(-30, 0.0f, 250.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/stone01.dds", L"./Model/bump01.dds", XMFLOAT3(-40, 0.0f, 250.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/stone01.dds", L"./Model/bump01.dds", XMFLOAT3(-50, 0.0f, 250.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/stone01.dds", L"./Model/bump01.dds", XMFLOAT3(-60, 0.0f, 250.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/stone01.dds", L"./Model/bump01.dds", XMFLOAT3(-70, 0.0f, 250.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/stone01.dds", L"./Model/bump01.dds", XMFLOAT3(-80, 0.0f, 250.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/stone01.dds", L"./Model/bump01.dds", XMFLOAT3(-90, 0.0f, 250.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/stone01.dds", L"./Model/bump01.dds", XMFLOAT3(-100, 0.0f, 250.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/stone01.dds", L"./Model/bump01.dds", XMFLOAT3(-110, 0.0f, 250.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/stone01.dds", L"./Model/bump01.dds", XMFLOAT3(-120, 0.0f, 250.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/stone01.dds", L"./Model/bump01.dds", XMFLOAT3(-130, 0.0f, 250.0f), 4.0f));

	////Scene
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/brickwall.dds", L"./Model/brickbump.dds", XMFLOAT3(-30, 0.0f, 50.0f), 4.0f));

	models.push_back(InitializeModel(hwnd, "./Model/Cube.obj", L"./Model/brickwall.dds", L"./Model/brickbump.dds", XMFLOAT3(30, 0.0f, 50.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/icetex.dds", L"./Model/icenormal.dds", XMFLOAT3(20.0f, 0.0f, 150.0f), 4.0f));
	models.push_back(InitializeModel(hwnd, "./Model/Cube.obj", L"./Model/crate.dds", L"./Model/cratebump.dds", XMFLOAT3(-20.0f, 0.0f, 150.0f), 4.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/stone01.dds", L"./Model/bump01.dds", XMFLOAT3(-10, 0.0f, 250.0f), 4.0f));
	models.push_back(InitializeModel(hwnd, "./Model/Cube.obj", L"./Model/stone01.dds", L"./Model/bump01.dds", XMFLOAT3(10, 0.0f, 250.0f), 4.0f));

	//models.push_back(InitializeModel(hwnd, "./Model/Plane.txt", L"./Model/ground.dds", L"./Model/groundbump.dds", XMFLOAT3(0.0f, -10.0f, 0.0f), 100.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Plane.txt", L"./Model/ground.dds", L"./Model/groundbump.dds", XMFLOAT3(0.0f, -10.0f, 200.0f), 100.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Plane.txt", L"./Model/ground.dds", L"./Model/groundbump.dds", XMFLOAT3(0.0f, -10.0f, 400.0f), 100.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Plane.txt", L"./Model/ground.dds", L"./Model/groundbump.dds", XMFLOAT3(200.0f, -10.0f, 0.0f), 100.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Plane.txt", L"./Model/ground.dds", L"./Model/groundbump.dds", XMFLOAT3(200.0f, -10.0f, 200.0f), 100.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Plane.txt", L"./Model/ground.dds", L"./Model/groundbump.dds", XMFLOAT3(200.0f, -10.0f, 400.0f), 100.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Plane.txt", L"./Model/ground.dds", L"./Model/groundbump.dds", XMFLOAT3(-200.0f, -10.0f, 0.0f), 100.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Plane.txt", L"./Model/ground.dds", L"./Model/groundbump.dds", XMFLOAT3(-200.0f, -10.0f, 200.0f), 100.0f));
	//models.push_back(InitializeModel(hwnd, "./Model/Plane.txt", L"./Model/ground.dds", L"./Model/groundbump.dds", XMFLOAT3(-200.0f, -10.0f, 400.0f), 100.0f));

	//for(auto t : GameWorld::getInstance().triangles)
	//{
	//	std::cout << "Tri: " << std::endl;
	//	std::cout << t->vertices[0].x << ", " << t->vertices[0].y << ", " << t->vertices[0].z << std::endl;
	//	std::cout << t->vertices[1].x << ", " << t->vertices[1].y << ", " << t->vertices[1].z << std::endl;
	//	std::cout << t->vertices[2].x << ", " << t->vertices[2].y << ", " << t->vertices[2].z << std::endl;
	//	std::cout << std::endl;
	//}

	//Lights
	light = new LightClass();
	if (!light)
	{
		return false;
	}
	light->SetPosition(-30.0f, 100.0f, -100.0f);
	light->SetAmbientColor(0.2f, 0.2f, 0.2f, 1.0f);
	light->SetDiffuseColor(1.0, 1.0, 1.0, 1.0f);
	light->SetLookAt(0.0f, 0.0f, 20.0f);
	light->GenerateProjectionsMatrix(SCREEN_DEPTH, SCREEN_NEAR);

	//Lights
	light2 = new LightClass();
	if (!light2)
	{
		return false;
	}
	light2->SetPosition(30.0f, 100.0f, -100.0f);
	light2->SetAmbientColor(0.0f, 0.0f, 0.0f, 0.0f);
	light2->SetDiffuseColor(1.0, 1.0, 1.0, 1.0f);
	light2->SetLookAt(0.0f, 0.0f, 20.0f);
	light2->GenerateProjectionsMatrix(SCREEN_DEPTH, SCREEN_NEAR);

	//HARDCODED END

	//Create RenderToTexture
	renderTexture = new RenderTextureClass();
	if (!renderTexture)
	{
		return false;
	}

	result = renderTexture->Initialize(direct3D->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_DEPTH, SCREEN_NEAR, direct3D->GetCurrentSampleCount(), direct3D->GetCurrentQualityLevel());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the render to texture object.", L"Error", MB_OK);
		return false;
	}

	renderTexture2 = new RenderTextureClass();
	if (!renderTexture2)
	{
		return false;
	}

	result = renderTexture2->Initialize(direct3D->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_DEPTH, SCREEN_NEAR, direct3D->GetCurrentSampleCount(), direct3D->GetCurrentQualityLevel());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the render to texture object.", L"Error", MB_OK);
		return false;
	}

	depthShader = new DepthShaderClass();
	if (!depthShader)
	{
		return false;
	}

	result = depthShader->Initialize(direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the depth shader object.", L"Error", MB_OK);
		return false;
	}

	//Set up shader
	shader = new ShaderClass();
	if (!shader)
	{
		return false;
	}

	result = shader->Initialize(direct3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize shader object", L"Error", MB_OK);
		return false;
	}

	//Text
	m_font.reset(new SpriteFont(direct3D->GetDevice(), L"./Model/myfile.spritefont"));

	m_spriteBatch.reset(new SpriteBatch(direct3D->GetDeviceContext()));

	fpsPos.x = screenWidth / 80.0f;
	fpsPos.y = screenHeight / 80.0f + 10;
	scPos.x = screenWidth / 80.0f;
	scPos.y = screenHeight / 80.0f + 50;
	qlPos.x = screenWidth / 80.0f;
	qlPos.y = screenHeight / 80.0f + 90;

	GenerateScreenBuffer();

	SetupPrimitiveBatch();

	std::cout << "Generating KD-Tree for " << GameWorld::getInstance().triangles.size() << " triangles" << std::endl;
	tree = new KdNode();
	tree = tree->build(&GameWorld::getInstance().triangles, 0);
	std::cout << "KD-Tree finished" << std::endl;

	return true;
}

void GraphicsClass::Shutdown()
{
	if (depthShader)
	{
		depthShader->Shutdown();
		delete depthShader;
		depthShader = nullptr;
	}

	if (renderTexture)
	{
		renderTexture->Shutdown();
		delete renderTexture;
		renderTexture = nullptr;
	}

	if (light)
	{
		delete light;
		light = nullptr;
	}

	if (renderTexture2)
	{
		renderTexture2->Shutdown();
		delete renderTexture2;
		renderTexture2 = nullptr;
	}

	if (light2)
	{
		delete light2;
		light2 = nullptr;
	}

	ShutdownModels();

	if (timer)
	{
		delete timer;
		timer = nullptr;
	}

	if (shader)
	{
		shader->Shutdown();
		delete shader;
		shader = nullptr;
	}

	if (camera)
	{
		camera->Shutdown();
		delete camera;
		camera = nullptr;
	}

	if (direct3D)
	{
		direct3D->Shutdown();
		delete direct3D;
		direct3D = nullptr;
	}
}

bool GraphicsClass::Frame(InputClass* input)
{
	bool result;
	static float counter = -5.0f;
	static float totaltime = 0.0f;
	timer->Frame();

	float deltaTime = timer->GetFrameTime();
	totaltime += deltaTime;

	if(totaltime > 1000)
	{
		fps = 1000.0f / deltaTime;
		totaltime = 0.0f;
	}

	CheckWireframe(input); 
	CheckMSKeys(input);
	CheckRaycast(input);

	//Lightmovement
	counter += deltaTime * 0.03f;
	if (counter > 100.0f)
	{
		counter = -100.0f;
	}
	light->SetPosition(counter, light->GetPosition().y, light->GetPosition().z);
	SetLightDirection(input);


	result = Render(counter, input);
	if (!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::GenerateScreenBuffer()
{
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT result;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	if (screenBuffer)
	{
		screenBuffer->Release();
		screenBuffer = nullptr;
	}

	if (screenTargetView)
	{
		screenTargetView->Release();
		screenTargetView = nullptr;
	}

	if (depthBuffer)
	{
		depthBuffer->Release();
		depthBuffer = nullptr;
	}

	if (depthTargetView)
	{
		depthTargetView->Release();
		depthTargetView = nullptr;
	}

	ZeroMemory(&textureDesc, sizeof(textureDesc));

	//Setup Render target texture desc
	textureDesc.Width = currentScreenWidth;
	textureDesc.Height = currentScreenHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	//Multisample
	textureDesc.SampleDesc.Count = direct3D->GetCurrentSampleCount();
	textureDesc.SampleDesc.Quality = direct3D->GetCurrentQualityLevel();

	// Create the render target texture.
	result = direct3D->GetDevice()->CreateTexture2D(&textureDesc, nullptr, &screenBuffer);
	if (FAILED(result))
	{
		return false;
	}

	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	// Create the render target view.
	result = direct3D->GetDevice()->CreateRenderTargetView(screenBuffer, &renderTargetViewDesc, &screenTargetView);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = currentScreenWidth;
	depthBufferDesc.Height = currentScreenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	//Multisample
	depthBufferDesc.SampleDesc.Count = direct3D->GetCurrentSampleCount();
	depthBufferDesc.SampleDesc.Quality = direct3D->GetCurrentQualityLevel();

	// Create the texture for the depth buffer using the filled out description.
	result = direct3D->GetDevice()->CreateTexture2D(&depthBufferDesc, nullptr, &depthBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Initailze the depth stencil view description.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = direct3D->GetDevice()->CreateDepthStencilView(depthBuffer, &depthStencilViewDesc, &depthTargetView);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void GraphicsClass::SetupPrimitiveBatch()
{
	primitiveBatch = new PrimitiveBatch<VertexPositionColor>(direct3D->GetDeviceContext());

	basicEffect = new BasicEffect(direct3D->GetDevice());
	basicEffect->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	direct3D->GetDevice()->CreateInputLayout(VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		&inputLayout);
}

bool GraphicsClass::RenderSceneToTexture()
{
	XMMATRIX lightViewMatrix, lightProjectionMatrix;
	bool result;

	//Set texture as render target
	renderTexture->SetRenderTarget(direct3D->GetDeviceContext());

	//Clear rendertexture
	renderTexture->ClearRenderTarget(direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	light->GenerateViewMatrix();

	light->GetViewMatrix(lightViewMatrix);
	light->GetProjectionMatrix(lightProjectionMatrix);

	for (auto i : models)
	{
		i->Render(direct3D->GetDeviceContext());
		result = depthShader->Render(direct3D->GetDeviceContext(), i->GetIndexCount(), i->GetInstanceCount(), i->worldMatrix, lightViewMatrix, lightProjectionMatrix);
		if (!result)
		{
			return false;
		}
	}

	direct3D->SetBackBufferRenderTarget();
	direct3D->ResetViewport();

	return true;
}

bool GraphicsClass::RenderSceneToTexture2()
{
	XMMATRIX lightViewMatrix, lightProjectionMatrix;
	bool result;

	//Set texture as render target
	renderTexture2->SetRenderTarget(direct3D->GetDeviceContext());

	//Clear rendertexture
	renderTexture2->ClearRenderTarget(direct3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	light2->GenerateViewMatrix();

	light2->GetViewMatrix(lightViewMatrix);
	light2->GetProjectionMatrix(lightProjectionMatrix);

	for (auto i : models)
	{
		i->Render(direct3D->GetDeviceContext());
		result = depthShader->Render(direct3D->GetDeviceContext(), i->GetIndexCount(), i->GetInstanceCount(), i->worldMatrix, lightViewMatrix, lightProjectionMatrix);
		if (!result)
		{
			return false;
		}
	}

	direct3D->SetBackBufferRenderTarget();
	direct3D->ResetViewport();

	return true;
}

bool GraphicsClass::SetScreenBuffer(float red, float green, float blue, float alpha)
{
	XMMATRIX lightViewMatrix, lightProjectionMatrix;

	//Set texture as render target
	renderTexture->SetRenderTarget(direct3D->GetDeviceContext());
	direct3D->GetDeviceContext()->OMSetRenderTargets(1, &screenTargetView, depthTargetView);
	direct3D->GetDeviceContext()->RSSetViewports(1, &direct3D->viewport);

	//Clear rendertexture
	float color[4];

	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;

	direct3D->GetDeviceContext()->ClearRenderTargetView(screenTargetView, color);

	direct3D->GetDeviceContext()->ClearDepthStencilView(depthTargetView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return true;
}

void GraphicsClass::RenderRay()
{
	ID3D11DepthStencilState* depthstate;
	ID3D11RasterizerState* rsstate;
	direct3D->GetDeviceContext()->OMGetDepthStencilState(&depthstate, nullptr);
	direct3D->GetDeviceContext()->RSGetState(&rsstate);

	//Primitive Batch Begin
	CommonStates states(direct3D->GetDevice());
	direct3D->GetDeviceContext()->OMSetBlendState(states.Opaque(), nullptr, 0xFFFFFFFF);
	direct3D->GetDeviceContext()->OMSetDepthStencilState(states.DepthNone(), 0);
	direct3D->GetDeviceContext()->RSSetState(states.CullNone());


	primitiveBatch->Begin();

	//Ray1
	primitiveBatch->DrawLine(VertexPositionColor(hit1.hitray.position, Colors::Blue), VertexPositionColor(hit1.hitray.position + hit1.hitray.direction * hit1.hitDistance, Colors::Blue));

	if (hit1.hitTriangle != nullptr)
	{
		primitiveBatch->DrawTriangle(VertexPositionColor(hit1.hitTriangle->vertices[0], XMFLOAT4(Colors::Blue)), VertexPositionColor(hit1.hitTriangle->vertices[1], XMFLOAT4(Colors::Blue)), VertexPositionColor(hit1.hitTriangle->vertices[2], XMFLOAT4(Colors::Blue)));
	}

	if(hit1.hitBox != nullptr)
	{
		hit1.hitBox->Draw(primitiveBatch, Colors::Blue);
	}
	
	//Ray2
	primitiveBatch->DrawLine(VertexPositionColor(hit2.hitray.position, Colors::Red), VertexPositionColor(hit2.hitray.position + hit2.hitray.direction * hit2.hitDistance, Colors::Red));

	if (hit2.hitTriangle != nullptr)
	{
		primitiveBatch->DrawTriangle(VertexPositionColor(hit2.hitTriangle->vertices[0], XMFLOAT4(Colors::Red)), VertexPositionColor(hit2.hitTriangle->vertices[1], XMFLOAT4(Colors::Red)), VertexPositionColor(hit2.hitTriangle->vertices[2], XMFLOAT4(Colors::Red)));
	}

	if(hit2.hitBox != nullptr)
	{
		hit2.hitBox->Draw(primitiveBatch, Colors::Red);
	}

	//Ruler

	primitiveBatch->DrawLine(VertexPositionColor(hit1.hitPoint, Colors::Magenta), VertexPositionColor(hit2.hitPoint, Colors::Magenta));

	primitiveBatch->End();

	direct3D->GetDeviceContext()->OMSetDepthStencilState(depthstate, 0);
	direct3D->GetDeviceContext()->RSSetState(rsstate);
	//Primitive Batch End
}

void GraphicsClass::CastRay()
{
	float maxRange = 100000.0f;
	float hitfloat = 100000.0f;

	if(hitCounter%2 == 0)
	{
		if(KdNode::hit(tree, &ray, hitfloat, maxRange, hit1))
		{
			hitCounter++;
		}
	} else
	{
		if (KdNode::hit(tree, &ray, hitfloat, maxRange, hit2))
		{
			hitCounter++;
		}
	}

	ruler = hit2.hitPoint - hit1.hitPoint;
	std::cout << "Distance between Points: " << ruler.Length() << std::endl;

}

bool GraphicsClass::Render(float rotation, InputClass* input)
{
	XMMATRIX viewMatrix, projectionMatrix, translateMatrix;
	XMMATRIX lightViewMatrix, lightProjectionMatrix;
	XMMATRIX lightViewMatrix2, lightProjectionMatrix2;
	bool result;

	////Render scene to texture
	//result = RenderSceneToTexture();
	//if (!result)
	//{
	//	return false;
	//}

	////Render scene to texture 2
	//result = RenderSceneToTexture2();
	//if (!result)
	//{
	//	return false;
	//}


	//clear Buffer at beginning
	SetScreenBuffer(0.2f, 0.5f, 0.5f, 0.0f);
	//direct3D->BeginScene(0.2f, 0.5f, 0.5f, 0.0f);

	//Generate view matrix based on camera
	camera->DoMovement(input);
	camera->Render();

	//Get world, view and proj matrices
	//direct3D->GetWorldMatrix(worldMatrix);
	camera->GetViewMatrix(viewMatrix);
	direct3D->GetProjectionMatrix(projectionMatrix);


	////Lighting
	//light->GenerateViewMatrix();
	//light->GetViewMatrix(lightViewMatrix);
	//light->GetProjectionMatrix(lightProjectionMatrix);
	////Lighting2
	//light2->GenerateViewMatrix();
	//light2->GetViewMatrix(lightViewMatrix2);
	//light2->GetProjectionMatrix(lightProjectionMatrix2);

	//Put model vertex and index buffer on pipeline
	for (ModelClass* model : models)
	{
		model->Render(direct3D->GetDeviceContext());

		//Render using shader
		result = shader->Render(direct3D->GetDeviceContext(), model->GetIndexCount(), model->GetInstanceCount(), model->worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix, model->GetTextureViewArray(), renderTexture->GetShaderResourceView(), light->GetPosition(), light->GetAmbientColor(), light->GetDiffuseColor(), lightViewMatrix2, lightProjectionMatrix2, renderTexture2->GetShaderResourceView(), light2->GetPosition(), light2->GetDiffuseColor(), bumpiness);
		if (!result)
		{
			return false;
		}
	}

	//Render Trackingpoints
	//for(ModelClass* model : camera->getTrackingPointsModels())
	//{

	//	model->Render(direct3D->GetDeviceContext());

	//	//Render using shader
	//	result = shader->Render(direct3D->GetDeviceContext(), model->GetIndexCount(), model->GetInstanceCount(), model->worldMatrix, viewMatrix, projectionMatrix, lightViewMatrix, lightProjectionMatrix, model->GetTextureView(), renderTexture->GetShaderResourceView(), light->GetPosition(), light->GetAmbientColor(), light->GetDiffuseColor());
	//	{
	//		return false;
	//	}
	//}

	//Text
	ID3D11DepthStencilState* depthstate;
	ID3D11RasterizerState* rsstate;
	direct3D->GetDeviceContext()->OMGetDepthStencilState(&depthstate, nullptr);
	direct3D->GetDeviceContext()->RSGetState(&rsstate);
	m_spriteBatch->Begin(SpriteSortMode_Deferred, nullptr, nullptr, depthstate, rsstate);

	RenderText(".", Vector2(currentScreenWidth / 2.0f, currentScreenHeight / 2.0f), true);
	RenderText("FPS: " + std::to_string(static_cast<int>(fps)), fpsPos, false);
	RenderText("SampleCount: " + std::to_string(direct3D->GetCurrentSampleCount()) + " of " + std::to_string(direct3D->GetMaxSampleCount()), scPos, false);
	RenderText("QualityLevel: " + std::to_string(direct3D->GetCurrentQualityLevel()) + " of " + std::to_string(direct3D->GetMaxQualityLevels()), qlPos, false);
	m_spriteBatch->End();


	//Primitive Batch Begin
	CommonStates states(direct3D->GetDevice());
	//direct3D->GetDeviceContext()->OMSetBlendState(states.Opaque(), nullptr, 0xFFFFFFFF);
	//direct3D->GetDeviceContext()->OMSetDepthStencilState(states.DepthNone(), 0);
	//direct3D->GetDeviceContext()->RSSetState(states.CullNone());

	basicEffect->SetWorld(XMMatrixIdentity());
	basicEffect->SetView(viewMatrix);
	basicEffect->SetProjection(projectionMatrix);
	basicEffect->Apply(direct3D->GetDeviceContext());
	direct3D->GetDeviceContext()->IASetInputLayout(inputLayout);

	if(rayrendermode)
	{

		primitiveBatch->Begin();
		//for(const auto t : GameWorld::getInstance().triangles)
		//{
		//	primitiveBatch->DrawTriangle(VertexPositionColor(Vector3(t->vertices[0]), Colors::Red), VertexPositionColor(Vector3(t->vertices[1]), Colors::Red), VertexPositionColor(Vector3(t->vertices[2]), Colors::Red));
		//}
		//primitiveBatch->DrawTriangle(VertexPositionColor(Vector3(GameWorld::getInstance().triangles[0]->vertices[0]), Colors::Red), VertexPositionColor(Vector3(GameWorld::getInstance().triangles[0]->vertices[1]), Colors::Red), VertexPositionColor(Vector3(GameWorld::getInstance().triangles[0]->vertices[2]), Colors::Red));

		tree->Draw(primitiveBatch, Colors::LightGreen);

		primitiveBatch->End();
		//Primitive Batch End

		//direct3D->GetDeviceContext()->OMSetDepthStencilState(depthstate, 0);
		//direct3D->GetDeviceContext()->RSSetState(rsstate);

			RenderRay();
	}

	ID3D11Texture2D* backBuffer;
	direct3D->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&backBuffer));
	direct3D->GetDeviceContext()->ResolveSubresource(backBuffer, 0, screenBuffer, 0, DXGI_FORMAT_R8G8B8A8_UNORM);

	//Output Buffer
	direct3D->EndScene();

	return true;
}

void GraphicsClass::CheckWireframe(InputClass* input)
{
	unsigned int xkey = 0x58;
	unsigned int ykey = 0x59;
	unsigned int zkey = 0x5A;
	unsigned int bkey = 0x42;

	if (!wireframeMode && !wireframeKeyToggle)
	{
		if (input->IsKeyDown(zkey))
		{
			wireframeMode = true;
			wireframeKeyToggle = true;
			ChangeFillmode(D3D11_FILL_WIREFRAME);
		}
	}
	else if(wireframeMode && !wireframeKeyToggle)
	{
		if (input->IsKeyDown(zkey))
		{
			wireframeMode = false;
			wireframeKeyToggle = true;
			ChangeFillmode(D3D11_FILL_SOLID);
		}
	}

	if (!rayRenderToggle && !rayrendermode)
	{
		if (input->IsKeyDown(bkey))
		{
			rayrendermode = true;
			rayRenderToggle = true;
		}
	}
	else if (!rayRenderToggle && rayrendermode)
	{
		if (input->IsKeyDown(bkey))
		{
			rayrendermode = false;
			rayRenderToggle = true;
		}
	}

	if (input->IsKeyUp(zkey))
	{
		wireframeKeyToggle = false;
	}

	if (input->IsKeyUp(bkey))
	{
		rayRenderToggle = false;
	}

	if (!bumpinessKeyToggle)
	{
		if (input->IsKeyDown(ykey))
		{
			if (bumpiness < 10)
			{
				bumpiness += 1;
			}
			bumpinessKeyToggle = true;
		}
		else if (input->IsKeyDown(xkey))
		{
			if (bumpiness > 0)
			{
				bumpiness -= 1;
			}
			bumpinessKeyToggle = true;
		}
	}

	if (input->IsKeyUp(ykey) && input->IsKeyUp(xkey))
	{
		bumpinessKeyToggle = false;
	}
}

void GraphicsClass::CheckMSKeys(InputClass* input)
{
	unsigned int mkey = 0x4D;
	unsigned int commakey = VK_OEM_COMMA;
	unsigned int periodkey = VK_OEM_PERIOD;

	if (!msmodetoggle)
	{
		if (input->IsKeyDown(commakey))
		{
			direct3D->IncreaseSampleCount();
			msmodetoggle = true;
		}
		if (input->IsKeyDown(periodkey))
		{
			direct3D->IncreaseQualityLevel();
			msmodetoggle = true;
		}
		if (input->IsKeyDown(mkey))
		{
			GenerateScreenBuffer();
			msmodetoggle = true;
		}
	}

	if (input->IsKeyUp(mkey) && input->IsKeyUp(periodkey) && input->IsKeyUp(commakey))
	{
		msmodetoggle = false;
	}
}

void GraphicsClass::CheckRaycast(InputClass* input)
{
	unsigned int spaceKey = VK_SPACE;

	if(!rayToggle)
	{
		if(input->IsKeyDown(spaceKey))
		{
			ray.position = camera->GetPosition();
			XMMATRIX newdir;
			camera->GetViewMatrix(newdir);	
			ray.direction = Matrix(newdir).Transpose().Backward();
			rayToggle = true;

			CastRay();
		}
	}

	if(rayToggle && input->IsKeyUp(spaceKey))
	{
		rayToggle = false;
	}
}

void GraphicsClass::SetLightDirection(InputClass* input)
{
	unsigned int onekey = 0x31;
	unsigned int twokey = 0x32;

	if (input->IsKeyDown(onekey))
	{
		Vector3 newposition = camera->GetPosition();
		Vector3 newrotation = camera->GetRotation();
		//newrotation.Normalize();

		light->SetPosition(newposition.x, newposition.y, newposition.z);
		//light->SetLookAt(newrotation.x, newrotation.y, newrotation.z);
		light->SetLookAt(0, 0, 300);
	}

	if (input->IsKeyDown(twokey))
	{
		Vector3 newposition = camera->GetPosition();
		Vector3 newrotation = camera->GetRotation();
		newrotation.Normalize();

		light2->SetPosition(newposition.x, newposition.y, newposition.z);
		//light2->SetLookAt(newrotation.x, newrotation.y, newrotation.z);
		light2->SetLookAt(0, 0, 300);
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


ModelClass* GraphicsClass::InitializeModel(HWND hwnd, char* modelFilename, WCHAR* textureFilename1, WCHAR* textureFilename2, XMFLOAT3 position, float scale)
{
	bool result;
	ModelClass* model;

	//Set up model class
	model = new ModelClass();
	if (!model)
	{
		return nullptr;
	}

	model->worldMatrix = XMMatrixIdentity();
	model->worldMatrix *= XMMatrixScaling(scale, scale, scale);
	model->worldMatrix *= XMMatrixTranslation(position.x, position.y, position.z);

	result = model->Initialize(direct3D->GetDevice(), modelFilename, textureFilename1, textureFilename2, model->worldMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize model", L"Error", MB_OK);
		return nullptr;
	}


	ModelLoader::getInstance().GetTriangles(modelFilename);

	return model;
}

void GraphicsClass::ShutdownModels()
{
	for (ModelClass* model : models)
	{
		model->Shutdown();
		delete model;
	}

	models.clear();
}

void GraphicsClass::RenderText(string texttorender, Vector2 screenPos, bool centerOrigin)
{
	wstring test(texttorender.begin(), texttorender.end());
	const wchar_t* output = test.c_str();
	Vector2 origin;

	if(centerOrigin)
	{
		 origin = m_font->MeasureString(output) / 2.f;
	} else
	{
		origin = Vector2::Zero;
	}

	m_font->DrawString(m_spriteBatch.get(), output,
		screenPos, Colors::White, 0.f, origin, 0.5f);
}

void GraphicsClass::RenderText(int inttorender, Vector2 screenPos, bool centerOrigin)
{
	wstring test = std::to_wstring(inttorender);
	const wchar_t* output = test.c_str();

	Vector2 origin;

	if (centerOrigin)
	{
		origin = m_font->MeasureString(output) / 2.f;
	}
	else
	{
		origin = Vector2::Zero;
	}

	m_font->DrawString(m_spriteBatch.get(), output,
		screenPos, Colors::White, 0.f, origin, 0.5f);

}
