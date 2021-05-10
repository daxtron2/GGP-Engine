#pragma once

#include <DirectXMath.h>
#include <memory>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include <d3d11.h>

#include "DXCore.h"
#include "Mesh.h"
#include "SimpleShader.h"
#include "WICTextureLoader.h" // Load textures using the DirectX Tool Kit
#include "Camera.h"


class Skybox
{
private:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cubeMapTexture;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	std::shared_ptr<Mesh> cubeMesh;
	std::shared_ptr<SimplePixelShader> pixelShader;
	std::shared_ptr<SimpleVertexShader> vertexShader;

public:
	// Constructor
	Skybox(
		std::shared_ptr<Mesh> cubeMesh,
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cubeMapTexture,
		Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState,
		Microsoft::WRL::ComPtr<ID3D11Device> device,
		std::shared_ptr<SimpleVertexShader> vertexShader,
		std::shared_ptr<SimplePixelShader> pixelShader);

	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Camera* camera);
};

