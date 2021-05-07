#include "Game.h"
#include "Vertex.h"
#include "Material.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"


// Needed for a helper function to read compiled shader files from the hard drive
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>
// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif

}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Note: Since we're using smart pointers (ComPtr),
	// we don't need to explicitly clean up those DirectX objects
	// - If we weren't using smart pointers, we'd need
	//   to call Release() on each DirectX object created in Game

}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	//CreateBasicGeometry();
	LoadModels();
	LoadTextures();
	CreateEntities();
	CreateLights();
	CreateSkyBox();
	ResizePostProcessResources();

	// Set up the post process sampler state
	D3D11_SAMPLER_DESC samplerStatePostProcessDesc = {};
	samplerStatePostProcessDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerStatePostProcessDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerStatePostProcessDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerStatePostProcessDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerStatePostProcessDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&samplerStatePostProcessDesc, samplerStatePostProcess.GetAddressOf());

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	camera = std::make_shared<Camera>(XMFLOAT3(0, 0, -10), XMFLOAT3(0, 0, 0), (float)width / (float)height, 60.f, 0.01f, 1000.f, 1.f, 1.f);
	

}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// and also created the Input Layout that describes our 
// vertex data to the rendering pipeline. 
// - Input Layout creation is done here because it must 
//    be verified against vertex shader byte code
// - We'll have that byte code already loaded below
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = std::make_shared<SimpleVertexShader>(device.Get(), context.Get(), GetFullPathTo_Wide(L"VertexShader.cso").c_str());
	pixelShader = std::make_shared<SimplePixelShader>(device.Get(), context.Get(), GetFullPathTo_Wide(L"PixelShader.cso").c_str());

	vertexShaderNormal = std::make_shared<SimpleVertexShader>(device.Get(), context.Get(), GetFullPathTo_Wide(L"VertexShaderNormal.cso").c_str());
	pixelShaderNormal = std::make_shared<SimplePixelShader>(device.Get(), context.Get(), GetFullPathTo_Wide(L"PixelShaderNormal.cso").c_str());

	vertexShaderSky = std::make_shared<SimpleVertexShader>(device.Get(), context.Get(), GetFullPathTo_Wide(L"VertexShaderSky.cso").c_str());
	pixelShaderSky = std::make_shared<SimplePixelShader>(device.Get(), context.Get(), GetFullPathTo_Wide(L"PixelShaderSky.cso").c_str());

	vertexShaderPostProcess = std::make_shared<SimpleVertexShader>(device.Get(), context.Get(), GetFullPathTo_Wide(L"VertexShaderPostProcess.cso").c_str());
	pixelShaderBloomExtract = std::make_shared<SimplePixelShader>(device.Get(), context.Get(), GetFullPathTo_Wide(L"PixelShaderBloomExtract.cso").c_str());
	pixelShaderBloomCombine = std::make_shared<SimplePixelShader>(device.Get(), context.Get(), GetFullPathTo_Wide(L"PixelShaderBloomCombine.cso").c_str());
	pixelShaderWeightedBlur = std::make_shared<SimplePixelShader>(device.Get(), context.Get(), GetFullPathTo_Wide(L"PixelShaderWeightedBlur.cso").c_str());
}

void Game::LoadModels() 
{
	meshes.push_back(std::make_shared<Mesh>(GetFullPathTo("../../Assets/Models/sphere.obj").c_str(), device));
	meshes.push_back(std::make_shared<Mesh>(GetFullPathTo("../../Assets/Models/cube.obj").c_str(), device));
	meshes.push_back(std::make_shared<Mesh>(GetFullPathTo("../../Assets/Models/torus.obj").c_str(), device));
	meshes.push_back(std::make_shared<Mesh>(GetFullPathTo("../../Assets/Models/cylinder.obj").c_str(), device));
	meshes.push_back(std::make_shared<Mesh>(GetFullPathTo("../../Assets/Models/helix.obj").c_str(), device));
}

void Game::LoadTextures()
{
	//CreateWICTextureFromFile(device.Get(), context.Get(), L".\\Assets\\Textures\\A8\\cushion.png", nullptr, &diffuseTexture1);
	//CreateWICTextureFromFile(device.Get(), context.Get(), L".\\Assets\\Textures\\A8\\metal.png", nullptr, &diffuseTexture2);
	//CreateWICTextureFromFile(device.Get(), context.Get(), L".\\Assets\\Textures\\A8\\rock.png", nullptr, &diffuseTexture3);

	//CreateWICTextureFromFile(device.Get(), context.Get(), L".\\Assets\\Textures\\A8\\cushion_normals.png", nullptr, &normalTexture1);
	//CreateWICTextureFromFile(device.Get(), context.Get(), L".\\Assets\\Textures\\A8\\metal_normals.png", nullptr, &normalTexture2);
	//CreateWICTextureFromFile(device.Get(), context.Get(), L".\\Assets\\Textures\\A8\\rock_normals.png", nullptr, &normalTexture3);

	CreateDDSTextureFromFile(device.Get(), L".\\Assets\\Textures\\Skies\\SunnyCubeMap.dds", nullptr, &skyTexture);
}

// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{

	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in memory
	//    over to a DirectX-controlled data structure (the vertex buffer)
	// - Note: Since we don't have a camera or really any concept of
	//    a "3d world" yet, we're simply describing positions within the
	//    bounds of how the rasterizer sees our screen: [-1 to +1] on X and Y
	// - This means (0,0) is at the very center of the screen.
	// - These are known as "Normalized Device Coordinates" or "Homogeneous 
	//    Screen Coords", which are ways to describe a position without
	//    knowing the exact size (in pixels) of the image/window/etc.  
	// - Long story short: Resizing the window also resizes the triangle,
	//    since we're describing the triangle in terms of the window itself
	Vertex triangleVerts[] =
	{
		{ XMFLOAT3(+0.0f, +0.25f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(+0.25f, -0.25f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(-0.25f, -0.25f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
	};

	// Set up the indices, which tell us which vertices to use and in which order
	// - This is somewhat redundant for just 3 vertices (it's a simple example)
	// - Indices are technically not required if the vertices are in the buffer 
	//    in the correct order and each one will be used exactly once
	// - But just to see how it's done...
	unsigned int triangleIndices[] = { 0, 1, 2 };

	//create and store the triangle mesh
	meshes.push_back(std::make_shared<Mesh>(
		triangleVerts, 3,
		triangleIndices, 3,
		device));

	Vertex squareVerts[] =
	{
		{ XMFLOAT3(-0.25f, +0.25f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) }, //top left
		{ XMFLOAT3(+0.25f, +0.25f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) }, //top right
		{ XMFLOAT3(+0.25f, -0.25f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },//bottom right
		{ XMFLOAT3(-0.25f, -0.25f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) }, //bottom left
	};
	unsigned int squareIndices[] = { 0, 1, 2, 0, 2, 3 };

	meshes.push_back(std::make_shared<Mesh>(
		squareVerts, 4,
		squareIndices, 6,
		device));

	Vertex pentagonVerts[] =
	{
		{ XMFLOAT3(-0.00f, +0.25f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(+0.10f, -0.25f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(-0.10f, -0.25f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(-0.15f, +0.05f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },
		{ XMFLOAT3(+0.15f, +0.05f, +0.0f), XMFLOAT3(0,0,-1), XMFLOAT2(0,0) },

	};
	unsigned int pentagonIndices[] = { 0, 1, 2, 2, 3, 0, 0, 4, 1 };

	meshes.push_back(std::make_shared<Mesh>(
		pentagonVerts, 5,
		pentagonIndices, 9,
		device));
}
void Game::CreateEntities() {



	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerDesc, &samplerState);


	
	std::shared_ptr<Material> mat1 = std::make_shared<Material>(pixelShaderNormal, vertexShaderNormal, samplerState.Get(), 
																L".\\Assets\\Textures\\PBR\\floor", device, context);
	mat1->SetSpecularIntensity(0.0f);

	std::shared_ptr<Material> mat2 = std::make_shared<Material>(pixelShaderNormal, vertexShaderNormal, samplerState.Get(), 
																L".\\Assets\\Textures\\PBR\\cobblestone", device, context);
	mat2->SetSpecularIntensity(0.5f);

	std::shared_ptr<Material> mat3 = std::make_shared<Material>(pixelShader, vertexShader, samplerState.Get(), 
																L".\\Assets\\Textures\\PBR\\scratched", device, context);
	mat3->SetSpecularIntensity(1.0f);


	entities.push_back(std::make_unique<Entity>(Entity(meshes[0], mat1)));
	entities.push_back(std::make_unique<Entity>(Entity(meshes[1], mat2)));
	entities.push_back(std::make_unique<Entity>(Entity(meshes[2], mat3)));
	entities.push_back(std::make_unique<Entity>(Entity(meshes[3], mat1)));
	entities.push_back(std::make_unique<Entity>(Entity(meshes[4], mat3)));

	//follows point light
	entities.push_back(std::make_unique<Entity>(Entity(meshes[0], mat3)));
	entities[5]->GetTransform()->SetScale(0.2f, 0.2f, 0.2f);

}
void Game::CreateLights()
{
	directionalLight1.color = XMFLOAT3(1.0f, 1.0f, 1.0f);
	directionalLight1.intensity = 1.0f;
	directionalLight1.direction = XMFLOAT3(1, -1, 1);
	directionalLight1.type = 0;

	directionalLight2.color = XMFLOAT3(0.1f, 1.0f, 0.1f);
	directionalLight2.intensity = 1.0f;
	directionalLight2.direction = XMFLOAT3(-1, -1, 1);
	directionalLight2.type = 0;

	pointLight1.color = XMFLOAT3(1, 0, 0);
	pointLight1.intensity = 1.0f;
	pointLight1.worldPos = XMFLOAT3(0.f, 5.f, 0.f);
	pointLight1.type = 1;
}
void Game::CreateSkyBox()
{
	skybox = std::make_shared<Sky>(meshes[1],
		samplerState,
		pixelShaderSky,
		vertexShaderSky,
		skyTexture,
		device);
}

// For each post-process resource, create an associated texture/RTV/SRV within the device
void Game::ResizePostProcessResources()
{
	ResizePostProcessRTVAndSRVPair(postProcessRTV, postProcessSRV, 1.0f);
	ResizePostProcessRTVAndSRVPair(bloomExtractRTV, bloomExtractSRV, 0.5f);

	float renderTargetScale = 0.5f;
	for (int i = 0; i < 3; i++) {
		ResizePostProcessRTVAndSRVPair(bloomLayersRTV[i], bloomLayersSRV[i], renderTargetScale);
		renderTargetScale *= 0.5f;
	}
}

void Game::ResizePostProcessRTVAndSRVPair(Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& RTV, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& SRV, float renderTargetScale) {
	// Create a texture description associated with the RTV/SRV
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = (unsigned int)(width * renderTargetScale);
	textureDesc.Height = (unsigned int)(height * renderTargetScale);
	textureDesc.ArraySize = 1;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.MipLevels = 1;
	textureDesc.MiscFlags = 0;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;

	// Create the texture
	ID3D11Texture2D* postProcessTexture;
	device->CreateTexture2D(&textureDesc, 0, &postProcessTexture);

	// Create the description for the Render Target View
	D3D11_RENDER_TARGET_VIEW_DESC RTVDesc = {};
	RTVDesc.Format = textureDesc.Format;
	RTVDesc.Texture2D.MipSlice = 0;
	RTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	// Create the RTV
	device->CreateRenderTargetView(postProcessTexture, &RTVDesc, RTV.ReleaseAndGetAddressOf());

	// Create the description for the Shader Resource View
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
	SRVDesc.Format = textureDesc.Format;
	SRVDesc.Texture2D.MipLevels = 1;
	SRVDesc.Texture2D.MostDetailedMip = 0;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

	// Create the SRV
	device->CreateShaderResourceView(postProcessTexture, &SRVDesc, SRV.ReleaseAndGetAddressOf());

	// The pointer is no longer needed, release the memory
	postProcessTexture->Release();
}

// Create the extracted bloom texture from the drawn screen
void Game::BloomExtract()
{
	// Use a texture that's half of the viewport size for a cheap blur effect
	D3D11_VIEWPORT viewport = {};
	viewport.Width = width * 0.5f;
	viewport.Height = height * 0.5f;
	viewport.MaxDepth = 1.0f;
	context->RSSetViewports(1, &viewport);

	// Draw the result to a texture rather than the screen
	context->OMSetRenderTargets(1, bloomExtractRTV.GetAddressOf(), 0);
	
	// Use the bloom-specific shader with the scene objects drawn to postProcessSRV
	pixelShaderBloomExtract->SetShader();
	pixelShaderBloomExtract->SetShaderResourceView("drawTexture", postProcessSRV.Get());
	pixelShaderBloomExtract->SetSamplerState("samplerOptions", samplerStatePostProcess.Get()); // TODO: Move to setting in post processing block

	// Set the cutoff brightness for pixels to influence bloom
	pixelShaderBloomExtract->SetFloat("bloomThreshold", 0.5f);
	pixelShaderBloomExtract->CopyAllBufferData();

	// Draw the dynamically rendered triangle
	context->Draw(3, 0);
}

void Game::BloomCombine()
{
	// Reset the viewport to the original size
	D3D11_VIEWPORT viewport = {};
	viewport.Width = width;
	viewport.Height = height;
	viewport.MaxDepth = 1.0f;
	context->RSSetViewports(1, &viewport);

	// Prepare for drawing to the screen since this is the last step in the render pipeline
	context->OMSetRenderTargets(1, backBufferRTV.GetAddressOf(), 0);

	// Set up the BloomCombine pixel shader with the layered textures
	pixelShaderBloomCombine->SetShader();
	pixelShaderBloomCombine->SetShaderResourceView("preBloomPixels", postProcessSRV.Get());
	pixelShaderBloomCombine->SetShaderResourceView("bloomLayer0", bloomExtractSRV.Get());
	pixelShaderBloomCombine->SetShaderResourceView("bloomLayer1", bloomExtractSRV.Get());
	pixelShaderBloomCombine->SetShaderResourceView("bloomLayer2", bloomExtractSRV.Get());
	pixelShaderBloomCombine->SetSamplerState("samplerOptions", samplerStatePostProcess.Get()); // TODO: Move to setting in post processing block

	// Apply constants for each layer's intensity
	pixelShaderBloomCombine->SetFloat("intensityBloomLayer0", 1.0f);
	pixelShaderBloomCombine->SetFloat("intensityBloomLayer1", 1.0f);
	pixelShaderBloomCombine->SetFloat("intensityBloomLayer2", 1.0f);
	pixelShaderBloomCombine->CopyAllBufferData();

	// Draw the dynamically rendered triangle
	context->Draw(3, 0);
}

void Game::WeightedBlur(Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& RTV, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& SRV, float renderTargetScale)
{
	// Resize the viewport to match the render target scale
	D3D11_VIEWPORT viewport = {};
	viewport.Width = width * renderTargetScale;
	viewport.Height = height * renderTargetScale;
	viewport.MaxDepth = 1.0f;
	context->RSSetViewports(1, &viewport);

	// Draw to the specified RTV
	context->OMSetRenderTargets(1, RTV.GetAddressOf(), 0);

	// Change the pixel shader to Weighted Blur
	pixelShaderWeightedBlur->SetShader();
	pixelShaderWeightedBlur->SetShaderResourceView("inputLayer", SRV.Get());
	pixelShaderWeightedBlur->SetSamplerState("samplerOptions", samplerStatePostProcess.Get()); // TODO: Move to setting in post processing block

	// Calculate the pixel->UV width/height based on the screen's width/height and the render target scale
	XMFLOAT2 pixelToUVSize = XMFLOAT2(1.0f / width / renderTargetScale, 1.0f / height / renderTargetScale);
	pixelShaderWeightedBlur->SetFloat2("pixelToUVSize", pixelToUVSize);
	pixelShaderWeightedBlur->CopyAllBufferData();

	// it's high noon
	context->Draw(3, 0);
}

// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	ResizePostProcessResources();

	// Handle base-level DX resize stuff
	DXCore::OnResize();
	if (camera != nullptr)
	{
		camera->UpdateProjectionMatrix((float)this->width / this->height);
	}
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	for (size_t i = 0; i < entities.size(); i++)
	{
		entities[i]->Update(deltaTime, totalTime);
	}
	camera->Update(deltaTime, this->hWnd);
	pointLight1.worldPos = XMFLOAT3(sin(totalTime) * 7, cos(totalTime) * 7, 0);
	entities[5]->GetTransform()->SetPosition(pointLight1.worldPos);
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV.Get(), color);
	context->ClearDepthStencilView(
		depthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	// Clear the post-process textures as well
	context->ClearRenderTargetView(postProcessRTV.Get(), color);
	context->ClearRenderTargetView(bloomExtractRTV.Get(), color);
	
	// Change the render target to be the post process RTV instead of the screen
	context->OMSetRenderTargets(1, postProcessRTV.GetAddressOf(), depthStencilView.Get());

	// TODO: Determine if shaders should be set here
	vertexShader->SetShader();
	pixelShader->SetShader();

	// Render the scene to the post process RTV
	pixelShader->SetFloat3("cameraPosition", camera->GetTransform()->GetPosition());

	pixelShader->SetData(
		"light1", 
		&pointLight1, 
		sizeof(Light)
	);


	pixelShader->SetData(
		"light2",
		&directionalLight1,
		sizeof(Light)
	);



	pixelShader->SetFloat3("ambientColor", ambientColor);

	pixelShader->CopyAllBufferData();

	pixelShaderNormal->SetFloat3("cameraPosition", camera->GetTransform()->GetPosition());

	pixelShaderNormal->SetData(
		"light1",
		&pointLight1,
		sizeof(Light)
	);


	pixelShaderNormal->SetData(
		"light2",
		&directionalLight1,
		sizeof(Light)
	);



	pixelShaderNormal->SetFloat3("ambientColor", ambientColor);

	pixelShaderNormal->CopyAllBufferData();

	for (int i = 0; i < entities.size(); i++)
	{
		entities[i]->Draw(context, camera.get());		
	}

	skybox->Draw(context, camera.get());

#pragma region Post Processing
	// Unbind the existing vertex and index buffers to prepare for drawing
	// the post-process texture onto a dynamically generated triangle
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ID3D11Buffer* nullBuffer = 0;
	context->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);
	context->IASetVertexBuffers(0, 1, &nullBuffer, &stride, &offset);

	// Resume drawing to the back buffer
	// context->OMSetRenderTargets(1, backBufferRTV.GetAddressOf(), 0);

	// Set the vertex shader used by all post processing
	vertexShaderPostProcess->SetShader();

	// Set the new sampler 
	// Assumes that all post-process pixel shaders have a resource 
	// context->PSSetSamplers(0, 1, samplerStatePostProcess.GetAddressOf());

	// Generate a texture of the screen with all pixels under a brightness treshold set to black
	BloomExtract();

	// After the bloom pixels have been extracted, blur the pixels to generate bloom
	// For the first layer, use bloomExtractSRV as the input
	float renderTargetScale = 0.5f;
	WeightedBlur(bloomLayersRTV[0], bloomExtractSRV, renderTargetScale);

	// For the next layers, use bloomLayer[n-1] as the input
	for (int i = 1; i < 3; i++) {
		renderTargetScale *= 0.5f;
		WeightedBlur(bloomLayersRTV[i], bloomLayersSRV[i-1], renderTargetScale);
	}

	// Apply bloom blur/iteration
	BloomCombine();

	// Unbind all SRVs so they can be drawn to in the next frame
	// Less than 32 SRVs currently exist, but a high number is used as a buffer for future development
	ID3D11ShaderResourceView* nullSRVs[32] = {};
	context->PSSetShaderResources(0, 32, nullSRVs);

	// Draw the dynamically rendered triangle
	// context->Draw(3, 0);
#pragma endregion


	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);

	// Due to the usage of a more sophisticated swap chain,
	// the render target must be re-bound after every call to Present()
	context->OMSetRenderTargets(1, backBufferRTV.GetAddressOf(), depthStencilView.Get());
}