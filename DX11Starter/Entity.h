#pragma once
#include "Transform.h"
#include "Mesh.h"
#include <memory>

class Entity
{
private:
	Transform transform;
	std::shared_ptr<Mesh> mesh;

public:
	Entity(std::shared_ptr<Mesh> _mesh);
	~Entity();

	std::shared_ptr<Mesh> GetMesh();
	Transform* GetTransform();

	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Microsoft::WRL::ComPtr<ID3D11Buffer> vsConstantBuffer);
	void Update(float deltaTime, float totalTime);
};

