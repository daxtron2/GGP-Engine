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
	XMStoreFloat3(&position, XMVectorSet(x, y, z, 0));
	transformDirty = true;
}

void Transform::SetRotation(float pitch, float yaw, float roll)
{
	XMStoreFloat3(&eulerRotation, XMVectorSet(pitch, yaw, roll, 0));
	transformDirty = true;
}

void Transform::SetScale(float x, float y, float z)
{
	XMStoreFloat3(&scale, XMVectorSet(x, y, z, 0));
	transformDirty = true;
}

void Transform::SetPosition(XMFLOAT3 pos)
{
	
	XMStoreFloat3(&position, XMLoadFloat3(&pos));
	transformDirty = true;
}

void Transform::SetRotation(XMFLOAT3 rot)
{
	XMStoreFloat3(&eulerRotation, XMLoadFloat3(&rot));
	transformDirty = true;
}

void Transform::SetScale(XMFLOAT3 sca)
{
	XMStoreFloat3(&scale, XMLoadFloat3(&sca));
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

DirectX::XMFLOAT3 Transform::GetForward()
{
	XMVECTOR absoluteRotation = XMQuaternionRotationRollPitchYaw(eulerRotation.x, eulerRotation.y, eulerRotation.z);

	XMFLOAT3 forward;
	XMStoreFloat3(&forward, XMVector3Rotate(XMVectorSet(0, 0, 1, 0), absoluteRotation));
	return forward;
}

void Transform::MoveAbsolute(float x, float y, float z)
{
	XMVECTOR pos = XMLoadFloat3(&position);
	pos += XMVectorSet(x, y, z, 0);
	XMStoreFloat3(&position, pos);
	transformDirty = true;
}

void Transform::MoveRelative(float x, float y, float z)
{
	XMVECTOR moveVector = XMVectorSet(x, y, z, 0);
	XMVECTOR absoluteRotation = XMQuaternionRotationRollPitchYaw(eulerRotation.x, eulerRotation.y, eulerRotation.z);
	XMVECTOR rotatedMove = XMVector3Rotate(moveVector, absoluteRotation);

	XMVECTOR pos = XMLoadFloat3(&position);
	pos += rotatedMove;
	XMStoreFloat3(&position, pos);
	transformDirty = true;
}

void Transform::Rotate(float pitch, float yaw, float roll)
{
	XMVECTOR rot = XMLoadFloat3(&eulerRotation);
	rot += XMVectorSet(pitch, yaw, roll, 0);
	XMStoreFloat3(&eulerRotation, rot);
	transformDirty = true;
}

void Transform::Scale(float x, float y, float z)
{
	XMVECTOR scl = XMLoadFloat3(&scale);
	scl += XMVectorSet(x, y, z, 0);
	XMStoreFloat3(&scale, scl);
	transformDirty = true;
}
