#pragma once
#include <DirectXMath.h>

struct DirectionalLight 
{
	DirectX::XMFLOAT3 color;
	float intensity;
	DirectX::XMFLOAT3 direction;
};