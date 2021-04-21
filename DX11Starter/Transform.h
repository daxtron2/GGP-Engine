#pragma once
#include <DirectXMath.h>
class Transform
{
private:
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT3 eulerRotation;
	bool transformDirty;

public:
	Transform();
	void SetPosition(float x, float y, float z);
	void SetRotation(float pitch, float yaw, float roll);
	void SetScale(float x, float y, float z);

	void SetPosition(DirectX::XMFLOAT3 pos);
	void SetRotation(DirectX::XMFLOAT3 rot);
	void SetScale(DirectX::XMFLOAT3 sca);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetEulerRotation();
	DirectX::XMFLOAT3 GetScale();
	DirectX::XMFLOAT4X4 GetWorldMatrix();
	DirectX::XMFLOAT3 GetForward();
	DirectX::XMFLOAT3 GetXZForward();
	DirectX::XMFLOAT3 GetRight();
	DirectX::XMFLOAT3 GetXZRight();


	void MoveAbsolute(float x, float y, float z);
	void MoveRelative(float x, float y, float z);
	void MoveAlong(DirectX::XMFLOAT3 direction, float speed);
	void Rotate(float pitch, float yaw, float roll);
	void Scale(float x, float y, float z);

};

