#include "Camera.h"
#include <math.h>
#include <iostream>
#include "Vector3.h"

using namespace DirectX;

Camera::Camera()
{
	transform = Transform();
	transform.SetRotation(0, 0, 0);
	transform.SetPosition(0, 0, -10);

	fieldOfView = 60.f;
	nearPlane = 0.01f;
	farPlane = 1000.f;
	mouseLookSpeed = 1.f;
	orbitDistance = 3.f;

	UpdateViewMatrix();
	UpdateProjectionMatrix(1.f);
}

Camera::Camera(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation,
			   float aspectRatio, float _fieldOfView, float _nearPlane, 
			   float _farPlane, float _mouseLookSpeed)
{
	transform = Transform();
	transform.SetRotation(rotation);
	transform.SetPosition(position);

	fieldOfView = _fieldOfView;
	nearPlane = _nearPlane;
	farPlane = _farPlane;
	mouseLookSpeed = _mouseLookSpeed;
	orbitDistance = 3.f;


	UpdateViewMatrix();
	UpdateProjectionMatrix(aspectRatio);
}

XMFLOAT4X4 Camera::GetViewMatrix()
{
	return viewMatrix;
}

XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

Transform* Camera::GetTransform()
{
	return &transform;
}

void Camera::SetFollowTransform(Transform* _transform)
{
	followTransform = _transform;
}

void Camera::UpdateViewMatrix()
{
	XMFLOAT3 tPos = transform.GetPosition().GetRaw();
	XMFLOAT3 tFwd = transform.GetForward().GetRaw();

	XMVECTOR pos = XMLoadFloat3(&tPos);
	XMVECTOR fwd = XMLoadFloat3(&tFwd);

	XMStoreFloat4x4(&viewMatrix, XMMatrixLookToLH(pos, fwd, XMVectorSet(0, 1, 0, 0)));
}

void Camera::UpdateProjectionMatrix(float aspectRatio)
{
	float fovRad = (fieldOfView * 3.1415926535897f) / 180.f;
	XMStoreFloat4x4(&projectionMatrix, XMMatrixPerspectiveFovLH(fovRad, aspectRatio, nearPlane, farPlane));
}

void Camera::Update(float deltaTime, HWND windowHandle)
{	
	POINT mousePos = {};
	GetCursorPos(&mousePos);
	ScreenToClient(windowHandle, &mousePos);
	if(Input->GetKey(VK_LBUTTON))
	{
		float xDiff = (float)mousePos.x - lastMousePosition.x;
		float yDiff = (float)mousePos.y - lastMousePosition.y;
		xDiff *= mouseLookSpeed * deltaTime;
		yDiff *= mouseLookSpeed * deltaTime;

		float currentPitch = transform.GetEulerRotation().X();
		if (currentPitch + yDiff > 1.5f || currentPitch + yDiff < -1.5f) 
		{
			yDiff = 0.f;
		}
		transform.Rotate(yDiff, xDiff, 0);
		
	}
	if (Input->GetKeyDown(VK_ADD) && orbitDistance > 0.0f) 
	{
		orbitDistance--;
	}
	if (Input->GetKeyDown(VK_SUBTRACT) && orbitDistance < 10.0f)
	{
		orbitDistance++;
	}

	if (followTransform != nullptr)
	{
		Vector3 ftPos = followTransform->GetPosition();
		Vector3 camFwd = transform.GetForward();

		camFwd = camFwd * orbitDistance;

		Vector3 newPos = ftPos - camFwd;
		transform.SetPosition(newPos);
	}

	lastMousePosition = mousePos;
	UpdateViewMatrix();
}
