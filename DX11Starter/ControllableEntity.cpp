#include "ControllableEntity.h"
#include <iostream>

ControllableEntity::ControllableEntity(std::shared_ptr<Mesh> _mesh, std::shared_ptr<Material> _material, Camera* _camera)
	: Entity(_mesh, _material)
{
	Input = &InputManager::GetInstance();
	camera = _camera;

	movementSpeed = 2.5f;
	currScore = 0;
}

void ControllableEntity::Update(float deltaTime, float totalTime)
{
	float speed = (Input->GetKey(VK_SHIFT) ? movementSpeed * 2 : movementSpeed) * deltaTime;

	if (Input->GetKey('W'))
	{
		transform.MoveAlong(Vector3(0, 1, 0), speed);
	}
	if (Input->GetKey('A'))
	{
		transform.MoveAlong(Vector3(-1, 0, 0), speed);
	}
	if (Input->GetKey('S'))
	{
		transform.MoveAlong(Vector3(0, -1, 0), speed);
	}
	if (Input->GetKey('D'))
	{
		transform.MoveAlong(Vector3(1, 0, 0), speed);
	}
	if (Input->GetKey(VK_SPACE))
	{

	}

	currentCollisions.clear();
	if (asteroidManager->CheckOverlap(this, &currentCollisions))
	{
		for (int i = 0; i < currentCollisions.size(); i++)
		{
			Vector3 myPos = transform.GetPosition();
			Vector3 otherPos = currentCollisions[i]->GetTransform()->GetPosition();

			Vector3 surfNormal = (otherPos - myPos).Normalize();

			Vector3 moveDir = currentCollisions[i]->movementDirection;
			Vector3 newDir = surfNormal + moveDir;

			currentCollisions[i]->movementDirection = newDir;
			if(!currentCollisions[i]->ScoreCollected)
			{
				currScore++;
			}
			currentCollisions[i]->Kill();
		}
	}

	//keep in bounding circle
	//if (transform.GetPosition().GetSqrMagnitude() >= 9.f)
	//{
	//	transform.SetPosition(transform.GetPosition().Normalize() * 3.f);
	//}

	transform.Rotate(0, deltaTime *.5f, 0);
}

void ControllableEntity::AddAsteroidManager(std::shared_ptr<AsteroidManager> _asteroidManager)
{
	asteroidManager = _asteroidManager;
}

int ControllableEntity::GetScore()
{
	return currScore;
}
