#include "Asteroid.h"

Asteroid::Asteroid(std::shared_ptr<Mesh> _mesh, std::shared_ptr<Material> _material) : Entity(_mesh, _material)
{
	int x = rand() % 6 - 3;
	int y = rand() % 6 - 3;
	int zVariation = rand() % 5;
	transform.SetPosition((float)x, (float)y, 10.f + zVariation);
}

void Asteroid::Update(float deltaTime, float totalTime)
{
	transform.MoveAlong(Vector3(0, 0, -1), deltaTime);
}
