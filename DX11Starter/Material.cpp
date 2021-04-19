#include "Material.h"
#include <memory>
#include "WICTextureLoader.h"

Material::Material(std::shared_ptr<SimplePixelShader> _pixelShader, 
				   std::shared_ptr<SimpleVertexShader> _vertexShader,
				   ID3D11SamplerState* _samplerState,
				   std::wstring texturePath,
				   Microsoft::WRL::ComPtr < ID3D11Device> device,
				   Microsoft::WRL::ComPtr < ID3D11DeviceContext> context)
{
	pixelShader = _pixelShader;
	vertexShader = _vertexShader;
	samplerState = _samplerState;


	DirectX::CreateWICTextureFromFile(device.Get(), context.Get(), (texturePath + L"_albedo.png").c_str(), nullptr, &texture);

	DirectX::CreateWICTextureFromFile(device.Get(), context.Get(), (texturePath + L"_normals.png").c_str(), nullptr, &normalMap);

	DirectX::CreateWICTextureFromFile(device.Get(), context.Get(), (texturePath + L"_roughness.png").c_str(), nullptr, &roughnessMap);

	DirectX::CreateWICTextureFromFile(device.Get(), context.Get(), (texturePath + L"_metalness.png").c_str(), nullptr, &metalnessMap);
}

DirectX::XMFLOAT4 Material::GetColorTint()
{
	return colorTint;
}

std::shared_ptr<SimplePixelShader> Material::GetPixelShader()
{
	return pixelShader;
}

std::shared_ptr<SimpleVertexShader> Material::GetVertexShader()
{
	return vertexShader;
}

bool Material::HasNormalMap()
{
	return normalMap != nullptr;
}

void Material::SetColorTint(DirectX::XMFLOAT4 tint)
{
	colorTint = tint;
}

void Material::SetSpecularIntensity(float spec)
{
	specularIntensity = spec;
}

float Material::GetSpecularIntensity() 
{
	return specularIntensity;
}

ID3D11SamplerState* Material::GetSamplerState()
{
	return samplerState;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Material::GetTextureSRV()
{
	return texture;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Material::GetNormalMap()
{
	return normalMap;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Material::GetRoughnessMap()
{
	return roughnessMap;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Material::GetMetalnessMap()
{
	return metalnessMap;
}
