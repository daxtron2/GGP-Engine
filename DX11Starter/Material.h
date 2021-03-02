#pragma once
#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include <d3d11.h>

class Material
{
private:
	DirectX::XMFLOAT4 colorTint;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;

public:
	Material(DirectX::XMFLOAT4 _colorTint, 
			 Microsoft::WRL::ComPtr<ID3D11PixelShader> _pixelShader, 
			 Microsoft::WRL::ComPtr<ID3D11VertexShader> _vertexShader);

	DirectX::XMFLOAT4 GetColorTint();
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetPixelShader();
	Microsoft::WRL::ComPtr<ID3D11VertexShader> GetVertexShader();

	void SetColorTint(DirectX::XMFLOAT4 tint);
};

