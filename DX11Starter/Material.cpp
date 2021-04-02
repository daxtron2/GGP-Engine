#include "Material.h"
#include <memory>

Material::Material(std::shared_ptr<SimplePixelShader> _pixelShader, 
				   std::shared_ptr<SimpleVertexShader> _vertexShader,
				   ID3D11SamplerState* _samplerState,
				   ID3D11ShaderResourceView* _texture, ID3D11ShaderResourceView* _normalMap = nullptr)
{
	pixelShader = _pixelShader;
	vertexShader = _vertexShader;
	samplerState = _samplerState;
	texture = _texture;
	normalMap = _normalMap;
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

ID3D11ShaderResourceView* Material::GetTextureSRV()
{
	return texture;
}

ID3D11ShaderResourceView* Material::GetNormalMap()
{
	return normalMap;
}
