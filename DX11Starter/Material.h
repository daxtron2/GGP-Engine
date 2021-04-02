#pragma once
#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include <d3d11.h>
#include "SimpleShader.h"
#include <memory>

class Material
{
private:
	DirectX::XMFLOAT4 colorTint = DirectX::XMFLOAT4(1.f, 1.f, 1.f, 1.f);
	float specularIntensity;
	std::shared_ptr<SimplePixelShader> pixelShader;
	std::shared_ptr<SimpleVertexShader> vertexShader;
	ID3D11SamplerState* samplerState;
	ID3D11ShaderResourceView* texture;
	ID3D11ShaderResourceView* normalMap;

public:
	Material(std::shared_ptr<SimplePixelShader> _pixelShader,
			 std::shared_ptr<SimpleVertexShader> _vertexShader,
			 ID3D11SamplerState* _samplerState,
			 ID3D11ShaderResourceView* _srv,
			 ID3D11ShaderResourceView* _normalMap);

	DirectX::XMFLOAT4 GetColorTint();
	std::shared_ptr<SimplePixelShader> GetPixelShader();
	std::shared_ptr<SimpleVertexShader> GetVertexShader();

	bool HasNormalMap();
	void SetColorTint(DirectX::XMFLOAT4 tint);
	void SetSpecularIntensity(float spec);
	float GetSpecularIntensity();
	ID3D11SamplerState* GetSamplerState();
	ID3D11ShaderResourceView* GetTextureSRV();
	ID3D11ShaderResourceView* GetNormalMap();
};

