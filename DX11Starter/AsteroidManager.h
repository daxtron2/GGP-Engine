#pragma once
#include "Asteroid.h"
#include "Mesh.h"
#include "Entity.h"


class AsteroidManager
{
private:
	std::vector<std::unique_ptr<Asteroid>> asteroids;
	int activeAsteroidCount;

public:
	AsteroidManager(std::shared_ptr<Mesh> _astMesh, std::shared_ptr<Material> _astMat);

	void Update(float deltaTime, float totalTime);
	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Camera* camera);
	bool CheckOverlap(Entity* entity, std::vector<Asteroid*>* collidedAsteroids);
};

