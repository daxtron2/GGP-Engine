#include "Material.h"
#include <memory>

Material::Material(DirectX::XMFLOAT4 _colorTint, std::shared_ptr<SimplePixelShader>
	_pixelShader, std::shared_ptr<SimpleVertexShader>
	_vertexShader)
{
	colorTint = _colorTint;
	pixelShader = _pixelShader;
	vertexShader = _vertexShader;
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

void Material::SetColorTint(DirectX::XMFLOAT4 tint)
{
	colorTint = tint;
}
