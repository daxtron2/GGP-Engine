#include "Asteroid.h"

Asteroid::Asteroid(std::shared_ptr<Mesh> _mesh, std::shared_ptr<Material> _material) : Entity(_mesh, _material)
{
	IsAlive = false;
	speed = 0.f;
	rotationDirection = Vector3(0, 0, 0);
	movementDirection = Vector3(0, 0, 0);
	transform.SetPosition(0, -1000, 0);
}

void Asteroid::Init()
{
	IsAlive = true;

	//random position
	float randLength = GetRandomFloat(0.f, 5.f);
	Vector3 randPosInCircle = GetRandomUnitVector() * randLength;
	int zVariation = rand() % 10;
	transform.SetPosition(randPosInCircle.X(), randPosInCircle.Y(), 10.f + zVariation);

	//random speed
	speed = GetRandomFloat(1.f, 3.f);

	//random rotation
	rotationDirection = GetRandomUnitVector();

	movementDirection = Vector3(0, 0, -1);
}

void Asteroid::Update(float deltaTime, float totalTime)
{
	if(IsAlive)
	{
		transform.MoveAlong(movementDirection, deltaTime * speed);

		transform.Rotate(rotationDirection * deltaTime);

		if(transform.GetPosition().Z() < -10)
		{
			Init();
		}
	}
}

void Asteroid::Kill()
{
	IsAlive = false;
	transform.SetPosition(0, -1000, 0);
}


