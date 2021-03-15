#pragma once
#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include <d3d11.h>
#include "SimpleShader.h"
#include <memory>

class Material
{
private:
	DirectX::XMFLOAT4 colorTint;
	std::shared_ptr<SimplePixelShader> pixelShader;
	std::shared_ptr<SimpleVertexShader> vertexShader;

public:
	Material(DirectX::XMFLOAT4 _colorTint, 
		std::shared_ptr<SimplePixelShader> _pixelShader,
		std::shared_ptr<SimpleVertexShader> _vertexShader);

	DirectX::XMFLOAT4 GetColorTint();
	std::shared_ptr<SimplePixelShader> GetPixelShader();
	std::shared_ptr<SimpleVertexShader> GetVertexShader();

	void SetColorTint(DirectX::XMFLOAT4 tint);
};

