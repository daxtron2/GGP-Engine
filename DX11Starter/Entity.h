#pragma once
#include "Transform.h"
#include "Mesh.h"
#include "Camera.h"
#include <memory>
#include "Material.h"
#include "SphereCollider.h"

class Entity
{
protected:
	Transform transform;
	std::shared_ptr<Mesh> mesh;
	std::shared_ptr<Material> material;
	SphereCollider* collider;

public:
	Entity();
	Entity(std::shared_ptr<Mesh> _mesh, std::shared_ptr<Material> _material);
	~Entity();

	std::shared_ptr<Mesh> GetMesh();
	Transform* GetTransform();
	SphereCollider* GetCollider();
	Material* GetMaterial();

	void SetMesh(std::shared_ptr<Mesh> _mesh);
	void SetMaterial(std::shared_ptr<Material> _material);

	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Camera* camera);
	virtual void Update(float deltaTime, float totalTime);
};

