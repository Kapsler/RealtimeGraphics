#include "GraphicsClass.h"
#include <memory>
#include <SpriteFont.h>
#include <SimpleMath.inl>
#include <string>

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

	//Scene
	models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/brickwall.dds", L"./Model/brickbump.dds", XMFLOAT3(-30, 0.0f, 50.0f), 4.0f));

	models.push_back(InitializeModel(hwnd, "./Model/Cube.txt", L"./Model/brickwall.dds", L"./Model/brickbump.dds", XMFLOAT3(30, 0.0f, 50.0f), 4.0f));
	models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/icetex.dds", L"./Model/icenormal.dds", XMFLOAT3(20.0f, 0.0f, 150.0f), 4.0f));
	models.push_back(InitializeModel(hwnd, "./Model/Cube.txt", L"./Model/crate.dds", L"./Model/cratebump.dds", XMFLOAT3(-20.0f, 0.0f, 150.0f), 4.0f));
	models.push_back(InitializeModel(hwnd, "./Model/Sphere.txt", L"./Model/stone01.dds", L"./Model/bump01.dds", XMFLOAT3(-10, 0.0f, 250.0f), 4.0f));
	models.push_back(InitializeModel(hwnd, "./Model/Cube.txt", L"./Model/stone01.dds", L"./Model/bump01.dds", XMFLOAT3(10, 0.0f, 250.0f), 4.0f));

	models.push_back(InitializeModel(hwnd, "./Model/Plane.txt", L"./Model/ground.dds", L"./Model/groundbump.dds", XMFLOAT3(0.0f, -10.0f, 0.0f), 100.0f));
	models.push_back(InitializeModel(hwnd, "./Model/Plane.txt", L"./Model/ground.dds", L"./Model/groundbump.dds", XMFLOAT3(0.0f, -10.0f, 200.0f), 100.0f));
	models.push_back(InitializeModel(hwnd, "./Model/Plane.txt", L"./Model/ground.dds", L"./Model/groundbump.dds", XMFLOAT3(0.0f, -10.0f, 400.0f), 100.0f));
	models.push_back(InitializeModel(hwnd, "./Model/Plane.txt", L"./Model/ground.dds", L"./Model/groundbump.dds", XMFLOAT3(200.0f, -10.0f, 0.0f), 100.0f));
	models.push_back(InitializeModel(hwnd, "./Model/Plane.txt", L"./Model/ground.dds", L"./Model/groundbump.dds", XMFLOAT3(200.0f, -10.0f, 200.0f), 100.0f));
	models.push_back(InitializeModel(hwnd, "./Model/Plane.txt", L"./Model/ground.dds", L"./Model/groundbump.dds", XMFLOAT3(200.0f, -10.0f, 400.0f), 100.0f));
	models.push_back(InitializeModel(hwnd, "./Model/Plane.txt", L"./Model/ground.dds", L"./Model/groundbump.dds", XMFLOAT3(-200.0f, -10.0f, 0.0f), 100.0f));
	models.push_back(InitializeModel(hwnd, "./Model/Plane.txt", L"./Model/ground.dds", L"./Model/groundbump.dds", XMFLOAT3(-200.0f, -10.0f, 200.0f), 100.0f));
	models.push_back(InitializeModel(hwnd, "./Model/Plane.txt", L"./Model/ground.dds", L"./Model/groundbump.dds", XMFLOAT3(-200.0f, -10.0f, 400.0f), 100.0f));

	//Lights
	light = new LightClass();
	if (!light)
	{
		return false;
	}
	light->SetPosition(-30.0f, 100.0f, -100.0f);
	light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
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
		fps = 1000.0 / deltaTime;
		totaltime = 0.0f;
	}

	CheckWireframe(input); 
	CheckMSKeys(input);

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
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
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
	bool result;

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

bool GraphicsClass::Render(float rotation, InputClass* input)
{
	XMMATRIX viewMatrix, projectionMatrix, translateMatrix;
	XMMATRIX lightViewMatrix, lightProjectionMatrix;
	XMMATRIX lightViewMatrix2, lightProjectionMatrix2;
	bool result;

	//Render scene to texture
	result = RenderSceneToTexture();
	if (!result)
	{
		return false;
	}

	//Render scene to texture 2
	result = RenderSceneToTexture2();
	if (!result)
	{
		return false;
	}


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

	//Lighting
	light->GenerateViewMatrix();
	light->GetViewMatrix(lightViewMatrix);
	light->GetProjectionMatrix(lightProjectionMatrix);
	//Lighting2
	light2->GenerateViewMatrix();
	light2->GetViewMatrix(lightViewMatrix2);
	light2->GetProjectionMatrix(lightProjectionMatrix2);

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

	RenderText("FPS: " + std::to_string(static_cast<int>(fps)), fpsPos);
	RenderText("SampleCount: " + std::to_string(direct3D->GetCurrentSampleCount()) + " of " + std::to_string(direct3D->GetMaxSampleCount()), scPos);
	RenderText("QualityLevel: " + std::to_string(direct3D->GetCurrentQualityLevel()) + " of " + std::to_string(direct3D->GetMaxQualityLevels()), qlPos);

	m_spriteBatch->End();

	ID3D11Texture2D* backBuffer;
	direct3D->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
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

	if (!wireframeMode & !wireframeKeyToggle)
	{
		if (input->IsKeyDown(zkey))
		{
			wireframeMode = true;
			wireframeKeyToggle = true;
			ChangeFillmode(D3D11_FILL_WIREFRAME);
		}
	}
	else
	{
		if (input->IsKeyDown(zkey))
		{
			wireframeMode = false;
			wireframeKeyToggle = true;
			ChangeFillmode(D3D11_FILL_SOLID);
		}
	}

	if (input->IsKeyUp(zkey))
	{
		wireframeKeyToggle = false;
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

	result = model->Initialize(direct3D->GetDevice(), modelFilename, textureFilename1, textureFilename2);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize model", L"Error", MB_OK);
		return nullptr;
	}

	model->worldMatrix *= XMMatrixScaling(scale, scale, scale);
	model->worldMatrix *= XMMatrixTranslation(position.x, position.y, position.z);

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

void GraphicsClass::RenderText(string texttorender, Vector2 screenPos)
{
	wstring test(texttorender.begin(), texttorender.end());
	const wchar_t* output = test.c_str();

	//Vector2 origin = m_font->MeasureString(output) / 2.f;
	Vector2 origin = Vector2::Zero;

	m_font->DrawString(m_spriteBatch.get(), output,
		screenPos, Colors::White, 0.f, origin);
}

void GraphicsClass::RenderText(int inttorender, Vector2 screenPos)
{
	wstring test = std::to_wstring(inttorender);
	const wchar_t* output = test.c_str();

	//Vector2 origin = m_font->MeasureString(output) / 2.f;
	Vector2 origin = Vector2::Zero;

	m_font->DrawString(m_spriteBatch.get(), output,
		screenPos, Colors::White, 0.f, origin);
}
