#include "Game.h"
#include "Vertex.h"
#include "BufferStructs.h"
#include "Material.h"


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
	CreateBasicGeometry();
	CreateEntities();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Get size as the next multiple of 16(don�t hardcode a numberhere!)
	unsigned int size = sizeof(VertexShaderExternalData);
	size = (size + 15) / 16 * 16;// This will work even if your struct size changes

	// Describe the constant buffer
	D3D11_BUFFER_DESC cbDesc = {};	// Sets struct to all zeros
	cbDesc.BindFlags         = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.ByteWidth		 = size;	// Must be a multiple of 16
	cbDesc.CPUAccessFlags	 = D3D11_CPU_ACCESS_WRITE;
	cbDesc.Usage			 = D3D11_USAGE_DYNAMIC;

	device->CreateBuffer(&cbDesc, 0, vsConstantBuffer.GetAddressOf());
	
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
	// Blob for reading raw data
	// - This is a simplified way of handling raw data
	ID3DBlob* shaderBlob;

	// Read our compiled vertex shader code into a blob
	// - Essentially just "open the file and plop its contents here"
	D3DReadFileToBlob(
		GetFullPathTo_Wide(L"VertexShader.cso").c_str(), // Using a custom helper for file paths
		&shaderBlob);

	// Create a vertex shader from the information we
	// have read into the blob above
	// - A blob can give a pointer to its contents, and knows its own size
	device->CreateVertexShader(
		shaderBlob->GetBufferPointer(), // Get a pointer to the blob's contents
		shaderBlob->GetBufferSize(),	// How big is that data?
		0,								// No classes in this shader
		vertexShader.GetAddressOf());	// The address of the ID3D11VertexShader*


	// Create an input layout that describes the vertex format
	// used by the vertex shader we're using
	//  - This is used by the pipeline to know how to interpret the raw data
	//     sitting inside a vertex buffer
	//  - Doing this NOW because it requires a vertex shader's byte code to verify against!
	//  - Luckily, we already have that loaded (the blob above)
	D3D11_INPUT_ELEMENT_DESC inputElements[2] = {};

	// Set up the first element - a position, which is 3 float values
	inputElements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;				// Most formats are described as color channels; really it just means "Three 32-bit floats"
	inputElements[0].SemanticName = "POSITION";							// This is "POSITION" - needs to match the semantics in our vertex shader input!
	inputElements[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// How far into the vertex is this?  Assume it's after the previous element

	// Set up the second element - a color, which is 4 more float values
	inputElements[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;			// 4x 32-bit floats
	inputElements[1].SemanticName = "COLOR";							// Match our vertex shader input!
	inputElements[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;	// After the previous element

	// Create the input layout, verifying our description against actual shader code
	device->CreateInputLayout(
		inputElements,					// An array of descriptions
		2,								// How many elements in that array
		shaderBlob->GetBufferPointer(),	// Pointer to the code of a shader that uses this layout
		shaderBlob->GetBufferSize(),	// Size of the shader code that uses this layout
		inputLayout.GetAddressOf());	// Address of the resulting ID3D11InputLayout*



	// Read and create the pixel shader
	//  - Reusing the same blob here, since we're done with the vert shader code
	D3DReadFileToBlob(
		GetFullPathTo_Wide(L"PixelShader.cso").c_str(), // Using a custom helper for file paths
		&shaderBlob);

	device->CreatePixelShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		0,
		pixelShader.GetAddressOf());
}



// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	XMFLOAT4 purple = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
	XMFLOAT4 orange = XMFLOAT4(1.0f, 0.647f, 0.0f, 1.0f);


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
		{ XMFLOAT3(+0.0f, +0.25f, +0.0f), red },
		{ XMFLOAT3(+0.25f, -0.25f, +0.0f), blue },
		{ XMFLOAT3(-0.25f, -0.25f, +0.0f), green },
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
		{ XMFLOAT3(-0.25f, +0.25f, +0.0f), red }, //top left
		{ XMFLOAT3(+0.25f, +0.25f, +0.0f), blue }, //top right
		{ XMFLOAT3(+0.25f, -0.25f, +0.0f), green },//bottom right
		{ XMFLOAT3(-0.25f, -0.25f, +0.0f), purple }, //bottom left
	};
	unsigned int squareIndices[] = { 0, 1, 2, 0, 2, 3 };

	meshes.push_back(std::make_shared<Mesh>(
		squareVerts, 4,
		squareIndices, 6,
		device));

	Vertex pentagonVerts[] =
	{
		{ XMFLOAT3(-0.00f, +0.25f, +0.0f), red },
		{ XMFLOAT3(+0.10f, -0.25f, +0.0f), blue },
		{ XMFLOAT3(-0.10f, -0.25f, +0.0f), green },
		{ XMFLOAT3(-0.15f, +0.05f, +0.0f), purple },
		{ XMFLOAT3(+0.15f, +0.05f, +0.0f), orange },

	};
	unsigned int pentagonIndices[] = { 0, 1, 2, 2, 3, 0, 0, 4, 1 };

	meshes.push_back(std::make_shared<Mesh>(
		pentagonVerts, 5,
		pentagonIndices, 9,
		device));
}
void Game::CreateEntities() {
	std::shared_ptr<Material> mat1 = std::make_shared<Material>(XMFLOAT4(1.0f, 0.5f, 0.5f, 1.f), pixelShader, vertexShader);
	std::shared_ptr<Material> mat2 = std::make_shared<Material>(XMFLOAT4(0.5f, 1.0f, 0.5f, 1.f), pixelShader, vertexShader);
	std::shared_ptr<Material> mat3 = std::make_shared<Material>(XMFLOAT4(0.5f, 0.5f, 1.0f, 1.f), pixelShader, vertexShader);

	entities.push_back(std::make_unique<Entity>(Entity(meshes[0], mat1)));
	entities.push_back(std::make_unique<Entity>(Entity(meshes[1], mat2)));
	entities.push_back(std::make_unique<Entity>(Entity(meshes[2], mat3)));
	entities.push_back(std::make_unique<Entity>(Entity(meshes[2], mat1)));
	entities.push_back(std::make_unique<Entity>(Entity(meshes[1], mat3)));
	entities.push_back(std::make_unique<Entity>(Entity(meshes[0], mat2)));
}

// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
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


	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	context->VSSetShader(vertexShader.Get(), 0, 0);
	context->PSSetShader(pixelShader.Get(), 0, 0);


	// Ensure the pipeline knows how to interpret the data (numbers)
	// from the vertex buffer.  
	// - If all of your 3D models use the exact same vertex layout,
	//    this could simply be done once in Init()
	// - However, this isn't always the case (but might be for this course)
	context->IASetInputLayout(inputLayout.Get());

	for (int i = 0; i < entities.size(); i++)
	{
		entities[i]->Draw(context, vsConstantBuffer, camera.get());		
	}



	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);

	// Due to the usage of a more sophisticated swap chain,
	// the render target must be re-bound after every call to Present()
	context->OMSetRenderTargets(1, backBufferRTV.GetAddressOf(), depthStencilView.Get());
}