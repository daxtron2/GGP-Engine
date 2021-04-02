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
	void CreateBasicGeometry();
	void CreateEntities();
	void CreateLights();

	
	// Note the usage of ComPtr below
	//  - This is a smart pointer for objects that abide by the
	//    Component Object Model, which DirectX objects do
	//  - More info here: https://github.com/Microsoft/DirectXTK/wiki/ComPtr
	
	// Shaders and shader-related constructs
	std::shared_ptr<SimplePixelShader> pixelShader;
	std::shared_ptr<SimpleVertexShader> vertexShader;

	std::shared_ptr<SimplePixelShader> pixelShaderNormal;
	std::shared_ptr<SimpleVertexShader> vertexShaderNormal;

	std::vector<std::shared_ptr<Mesh>> meshes;
	std::vector<std::unique_ptr<Entity>> entities;

	std::shared_ptr<Camera> camera;

	Light directionalLight1 = Light();
	Light directionalLight2 = Light();
	Light pointLight1 = Light();

	XMFLOAT3 ambientColor = XMFLOAT3(0.2f, 0.2f, 0.2f);

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> diffuseTexture1;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> diffuseTexture2;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> diffuseTexture3;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normalTexture1;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normalTexture2;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normalTexture3;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;

};

