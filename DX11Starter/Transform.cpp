#include "Transform.h"

using namespace DirectX;
Transform::Transform()
{
	transformDirty = true;
	position = XMFLOAT3(0, 0, 0);
	scale = XMFLOAT3(1, 1, 1);
	eulerRotation = XMFLOAT3(0, 0, 0);
	XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());
}

void Transform::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	transformDirty = true;

}

void Transform::SetRotation(float pitch, float yaw, float roll)
{
	eulerRotation.x = pitch;
	eulerRotation.y = yaw;
	eulerRotation.z = roll;
	transformDirty = true;

}

void Transform::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
	transformDirty = true;

}

DirectX::XMFLOAT3 Transform::GetPosition()
{
	return position;
}

DirectX::XMFLOAT3 Transform::GetEulerRotation()
{
	return eulerRotation;
}

DirectX::XMFLOAT3 Transform::GetScale()
{
	return scale;
}

DirectX::XMFLOAT4X4 Transform::GetWorldMatrix()
{
	if (transformDirty) {
		XMMATRIX translationMatrix = XMMatrixTranslation(position.x, position.y, position.z);

		XMMATRIX scaleMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);
		XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(eulerRotation.x, eulerRotation.y, eulerRotation.z);

		XMMATRIX world = scaleMatrix * rotationMatrix * translationMatrix;

		XMStoreFloat4x4(&worldMatrix, world);
		transformDirty = false;
	}
	return worldMatrix;
}

void Transform::MoveAbsolute(float x, float y, float z)
{
	position.x += x;
	position.y += y;
	position.z += z;
	transformDirty = true;

}

void Transform::Rotate(float pitch, float yaw, float roll)
{
	eulerRotation.x += pitch;
	eulerRotation.y += yaw;
	eulerRotation.z += roll;
	transformDirty = true;

}

void Transform::Scale(float x, float y, float z)
{
	scale.x += x;
	scale.y += y;
	scale.z += z;
	transformDirty = true;

}
