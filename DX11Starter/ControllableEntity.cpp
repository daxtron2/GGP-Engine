#include "ControllableEntity.h"
#include <iostream>

ControllableEntity::ControllableEntity(std::shared_ptr<Mesh> _mesh, std::shared_ptr<Material> _material, Camera* _camera)
	: Entity(_mesh, _material)
{
	Input = &InputManager::GetInstance();
	camera = _camera;

	movementSpeed = 1.f;

}

void ControllableEntity::Update(float deltaTime, float totalTime)
{
	for (int i = 0; i < asteroids.size(); i++)
	{
		SphereCollider* astCol = asteroids[i]->GetCollider();
		if (collider->CheckOverlap(astCol)) 
		{
			asteroids[i]->GetTransform()->Rotate(0, 1.f, 0);
			std::cout << "Player overlaps with " << i << std::endl;
		}
	}
	float speed = (Input->GetKey(VK_SHIFT) ? movementSpeed * 2 : movementSpeed) * deltaTime;

	if (Input->GetKey('W'))
	{
		transform.MoveAlong(camera->GetTransform()->GetXZForward(), speed);
	}
	if (Input->GetKey('A'))
	{
		transform.MoveAlong(camera->GetTransform()->GetXZRight(), -speed);
	}
	if (Input->GetKey('S'))
	{
		transform.MoveAlong(camera->GetTransform()->GetXZForward(), -speed);
	}
	if (Input->GetKey('D'))
	{
		transform.MoveAlong(camera->GetTransform()->GetXZRight(), speed);
	}
	if (Input->GetKey(VK_SPACE))
	{
		transform.MoveRelative(0, speed, 0);
	}
	if (Input->GetKey(VK_LCONTROL))
	{
		transform.MoveRelative(0, -speed, 0);
	}

}

void ControllableEntity::AddAsteroid(Asteroid* asteroid)
{
	asteroids.push_back(asteroid);
}
