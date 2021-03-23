#include "Camera.h"
#include <math.h>
using namespace DirectX;

Camera::Camera()
{
	transform = Transform();
	transform.SetRotation(0, 0, 0);
	transform.SetPosition(0, 0, -10);

	fieldOfView = 60.f;
	nearPlane = 0.01f;
	farPlane = 1000.f;
	movementSpeed = 1.f;
	mouseLookSpeed = 1.f;

	UpdateViewMatrix();
	UpdateProjectionMatrix(1.f);
}

Camera::Camera(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation,
			   float aspectRatio, float _fieldOfView, float _nearPlane, 
			   float _farPlane, float _movementSpeed, float _mouseLookSpeed)
{
	transform = Transform();
	transform.SetRotation(rotation);
	transform.SetPosition(position);

	fieldOfView = _fieldOfView;
	nearPlane = _nearPlane;
	farPlane = _farPlane;
	movementSpeed = _movementSpeed;
	mouseLookSpeed = _mouseLookSpeed;

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

void Camera::UpdateViewMatrix()
{
	XMFLOAT3 tPos = transform.GetPosition();
	XMFLOAT3 tFwd = transform.GetForward();

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
	float speed = (GetAsyncKeyState(VK_SHIFT) & 0x8000 ? movementSpeed * 2 : movementSpeed) * deltaTime;
	
	if (GetAsyncKeyState('W') & 0x8000) 
	{
		transform.MoveRelative(0, 0, speed);
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		transform.MoveRelative(-speed, 0, 0);
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		transform.MoveRelative(0, 0, -speed);
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		transform.MoveRelative(speed, 0, 0);
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		transform.MoveRelative(0, speed, 0);
	}
	if (GetAsyncKeyState(VK_LCONTROL) & 0x8000)
	{
		transform.MoveRelative(0, -speed, 0);
	}
	

	POINT mousePos = {};
	GetCursorPos(&mousePos);
	ScreenToClient(windowHandle, &mousePos);

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) 
	{
		float xDiff = (float)mousePos.x - lastMousePosition.x;
		float yDiff = (float)mousePos.y - lastMousePosition.y;
		xDiff *= mouseLookSpeed * deltaTime;
		yDiff *= mouseLookSpeed * deltaTime;

		transform.Rotate(yDiff, xDiff, 0);
	}

	UpdateViewMatrix();
	lastMousePosition = mousePos;
}
