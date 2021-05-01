#include "Transform.h"

using namespace DirectX;
Transform::Transform()
{
	transformDirty = true;
	position = Vector3(0, 0, 0);
	scale = Vector3(1, 1, 1);
	eulerRotation = Vector3(0, 0, 0);
	XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());
}

void Transform::SetPosition(float x, float y, float z)
{
	position.Set(x,y,z);
	transformDirty = true;
}

void Transform::SetRotation(float pitch, float yaw, float roll)
{
	eulerRotation.Set(pitch, yaw, roll);
	transformDirty = true;
}

void Transform::SetScale(float x, float y, float z)
{
	scale.Set(x, y, z);
	transformDirty = true;
}

void Transform::SetPosition(Vector3 pos)
{
	position = pos;
	transformDirty = true;
}

void Transform::SetRotation(Vector3 rot)
{
	eulerRotation = rot;
	transformDirty = true;
}

void Transform::SetScale(Vector3 sca)
{
	scale = sca;
	transformDirty = true;
}

Vector3 Transform::GetPosition()
{
	return position;
}

Vector3 Transform::GetEulerRotation()
{
	return eulerRotation;
}

Vector3 Transform::GetScale()
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

Vector3 Transform::GetForward()
{
	Vector3 forward(0, 0, 1);
	forward.Rotate(eulerRotation);
	forward.Normalize();
	return forward;
}

Vector3 Transform::GetXZForward()
{
	Vector3 forward(0, 0, 1);
	forward.Rotate(0, eulerRotation.Y(), eulerRotation.Z());
	forward.Normalize();
	return forward;
}

Vector3 Transform::GetRight()
{
	Vector3 right(1, 0, 0);
	right.Rotate(eulerRotation);
	right.Normalize();
	return right;
}

Vector3 Transform::GetXZRight()
{
	Vector3 right(1, 0, 0);
	right.Rotate(0, eulerRotation.Y(), eulerRotation.Z());
	right.Normalize();
	return right;
}

void Transform::MoveAbsolute(float x, float y, float z)
{
	position = position + Vector3(x, y, z);
	transformDirty = true;
}

void Transform::MoveRelative(float x, float y, float z)
{
	Vector3 moveVector(x, y, z);
	moveVector.Rotate(eulerRotation);

	position = position + moveVector;
	transformDirty = true;
}

void Transform::MoveAlong(Vector3 direction, float speed)
{
	XMVECTOR moveVector = XMVectorSet(direction.x * speed, direction.y * speed, direction.z * speed, 0);
	XMVECTOR pos = XMLoadFloat3(&position);
	pos += moveVector;
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
