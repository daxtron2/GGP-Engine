#pragma once
#include <DirectXMath.h>
#include "Vector3.h"

class Transform
{
private:
	DirectX::XMFLOAT4X4 worldMatrix;
	Vector3 position;
	Vector3 scale;
	Vector3 eulerRotation;
	bool transformDirty;

public:
	Transform();
	void SetPosition(float x, float y, float z);
	void SetRotation(float pitch, float yaw, float roll);
	void SetScale(float x, float y, float z);

	void SetPosition(Vector3 pos);
	void SetRotation(Vector3 rot);
	void SetScale(Vector3 sca);

	Vector3 GetPosition();
	Vector3 GetEulerRotation();
	Vector3 GetScale();
	DirectX::XMFLOAT4X4 GetWorldMatrix();
	Vector3 GetForward();
	Vector3 GetXZForward();
	Vector3 GetRight();
	Vector3 GetXZRight();


	void MoveAbsolute(float x, float y, float z);
	void MoveRelative(float x, float y, float z);
	void MoveAlong(Vector3 direction, float speed);
	void Rotate(float pitch, float yaw, float roll);
	void Scale(float x, float y, float z);

};

