#pragma once
#include <wrl/client.h>
#include "DXCore.h"
#include <memory>
#include "Mesh.h"
#include "SimpleShader.h"
#include "Camera.h"

class Sky
{

	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerOptions;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cubeMapTexture;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilOptions;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerOptions;

	std::shared_ptr<Mesh> skyboxMesh;
		 
	std::shared_ptr<SimpleVertexShader> skyVertexShader;
	std::shared_ptr<SimplePixelShader> skyPixelShader;

public:
	Sky(std::shared_ptr<Mesh> skyMesh, 
		Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState,
		std::shared_ptr<SimplePixelShader> pixelShader,
		std::shared_ptr<SimpleVertexShader> vertexShader,
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> skyTexture,
		Microsoft::WRL::ComPtr < ID3D11Device> device);

	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Camera* camera);

};

