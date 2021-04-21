#pragma once
#include "Entity.h"
#include "Camera.h"

class ControllableEntity : public Entity
{
private:
	Camera* camera;
	float movementSpeed;
	InputManager* Input;
public:
	ControllableEntity(std::shared_ptr<Mesh> _mesh, std::shared_ptr<Material> _material, Camera* _camera);
	void Update(float deltaTime, float totalTime) override;
};
