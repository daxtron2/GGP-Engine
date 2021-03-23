#pragma once
#include <DirectXMath.h>

struct Light 
{
	DirectX::XMFLOAT3 color;
	float intensity;
	DirectX::XMFLOAT3 direction;
	int type; //0 - directional, 1-point
	DirectX::XMFLOAT3 worldPos;
};
