#pragma once
#include <Windows.h>
#include "Transform.h"
#include <DirectXMath.h>

class Camera
{
private:
	Transform transform;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	POINT lastMousePosition;
	float fieldOfView;
	float nearPlane, farPlane;
	float movementSpeed;
	float mouseLookSpeed;

public:
	Camera();
	Camera(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, float aspectRatio, float _fieldOfView, float _nearPlane, float _farPlane, float _movementSpeed, float _mouseLookSpeed);

	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjectionMatrix();

	void UpdateViewMatrix();
	void UpdateProjectionMatrix(float aspectRatio);

	void Update(float deltaTime, HWND windowHandle);

};

