#pragma once

#include "DXCore.h"
#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include <vector>
#include "Mesh.h"
#include "Entity.h"
#include <memory>
#include "Camera.h"
#include "Lights.h"
#include "Skybox.h"
#include "AsteroidManager.h"
#include "ControllableEntity.h"
#include "Asteroid.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>



class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

private:

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 
	void LoadModels();
	void LoadTextures(); 
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateCubemap(
		const wchar_t* right,
		const wchar_t* left,
		const wchar_t* up,
		const wchar_t* down,
		const wchar_t* front,
		const wchar_t* back);
	void CreateBasicGeometry();
	void CreateEntities();
	void CreateLights();
	void CreateSkyBox();
	void ResizePostProcessResources();
	void ResizePostProcessRTVAndSRVPair(Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& RTV, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& SRV, float renderTargetScale);
	void BloomExtract();
	void BloomCombine();
	void WeightedBlur(Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& RTV, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& SRV, float renderTargetScale);
	
	// Note the usage of ComPtr below
	//  - This is a smart pointer for objects that abide by the
	//    Component Object Model, which DirectX objects do
	//  - More info here: https://github.com/Microsoft/DirectXTK/wiki/ComPtr
	
	// Shaders and shader-related constructs
	std::shared_ptr<SimplePixelShader> pixelShader;
	std::shared_ptr<SimpleVertexShader> vertexShader;

	std::shared_ptr<SimplePixelShader> pixelShaderSky;
	std::shared_ptr<SimpleVertexShader> vertexShaderSky;

	// POST PROCESS
	std::shared_ptr<SimplePixelShader> pixelShaderBloomExtract;
	std::shared_ptr<SimplePixelShader> pixelShaderBloomCombine;
	std::shared_ptr<SimplePixelShader> pixelShaderWeightedBlur;
	std::shared_ptr<SimpleVertexShader> vertexShaderPostProcess;

	std::vector<std::shared_ptr<Mesh>> meshes;
	//std::vector<std::unique_ptr<Entity>> entities;
	std::unique_ptr<ControllableEntity> player;

	std::shared_ptr<Camera> camera;
	std::shared_ptr<Skybox> skybox;

	std::shared_ptr<AsteroidManager> asteroidManager;

	Light light1 = Light();
	Light light2 = Light();
	Light light3 = Light();

	XMFLOAT3 ambientColor = XMFLOAT3(0.2f, 0.2f, 0.2f);


	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> skyTexture;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStatePostProcess;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> postProcessRTV; // Draw the "normal" data to this texture instead of the screen
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> postProcessSRV; // Use to sample the texture drawn by postProcessRTV
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> bloomExtractRTV;	// Extract only pixels above a minimum lightness threshold
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> bloomExtractSRV;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> bloomLayersRTV[3]; // Apply sequential layers of blurring
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> bloomLayersSRV[3];

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;

};

