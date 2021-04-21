#pragma once
#include <Windows.h>
#include "Transform.h"
#include <DirectXMath.h>
#include "InputManager.h"

class Camera
{
private:
	Transform transform;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	POINT lastMousePosition;
	float fieldOfView;
	float nearPlane, farPlane;
	float mouseLookSpeed;
	float orbitDistance;

	Transform* followTransform = nullptr;

	InputManager* Input = &InputManager::GetInstance();

public:
	Camera();
	Camera(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotation, float aspectRatio, float _fieldOfView, float _nearPlane, float _farPlane, float _mouseLookSpeed);

	DirectX::XMFLOAT4X4 GetViewMatrix();
	DirectX::XMFLOAT4X4 GetProjectionMatrix();
	Transform* GetTransform();

	void SetFollowTransform(Transform* transform);

	void UpdateViewMatrix();
	void UpdateProjectionMatrix(float aspectRatio);

	void Update(float deltaTime, HWND windowHandle);

};

