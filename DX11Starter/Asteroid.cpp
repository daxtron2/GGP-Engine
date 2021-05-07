#include "Asteroid.h"

Asteroid::Asteroid(std::shared_ptr<Mesh> _mesh, std::shared_ptr<Material> _material) : Entity(_mesh, _material)
{
	DyingSoon = false;
	IsAlive = false;
	speed = 0.f;
	rotationDirection = Vector3(0, 0, 0);
	movementDirection = Vector3(0, 0, 0);
	transform.SetPosition(0, -1000, 0);
}

void Asteroid::Init()
{
	IsAlive = true;
	DyingSoon = false;

	//random position
	float randLength = Vector3::GetRandomFloat(0.f, 5.f);
	Vector3 randPosInCircle = Vector3::GetRandomUnitVector() * randLength;
	int zVariation = rand() % 10;
	transform.SetPosition(randPosInCircle.X(), randPosInCircle.Y(), 10.f + zVariation);

	//random speed
	speed = Vector3::GetRandomFloat(1.f, 3.f);

	//random rotation
	rotationDirection = Vector3::GetRandomUnitVector();

	movementDirection = Vector3(0, 0, -1);
}

void Asteroid::Update(float deltaTime, float totalTime)
{
	transform.MoveAlong(movementDirection, deltaTime * speed);

	transform.Rotate(rotationDirection * deltaTime);

	if(IsAlive || DyingSoon)
	{
		Vector3 cPos = transform.GetPosition();
		if(cPos.GetSqrMagnitude() > 625.f)
		{
			Die();
		}
	}
}

void Asteroid::Kill()
{
	DyingSoon = true;
}

void Asteroid::Die()
{
	IsAlive = false;
	transform.SetPosition(0, -1000, 0);
	movementDirection = Vector3(0, 0, 0);
	rotationDirection = Vector3(0, 0, 0);
	speed = 0.f;
}
