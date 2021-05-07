#include "AsteroidManager.h"
#include <memory>

AsteroidManager::AsteroidManager(std::shared_ptr<Mesh> _astMesh, std::shared_ptr<Material> _astMat)
{
	activeAsteroidCount = 10;
	//pool 100 asteroid entities
	for(int i = 0; i < 100; i++)
	{
		asteroids.push_back(std::make_unique<Asteroid>(_astMesh, _astMat));
	}
}

void AsteroidManager::Update(float deltaTime, float totalTime)
{
	int aliveCount = 0;
	for(int i = 0; i < asteroids.size(); i++)
	{
		asteroids[i]->Update(deltaTime, totalTime);
		if(asteroids[i]->IsAlive)
		{
			aliveCount++;
		}
	}

	if(aliveCount < activeAsteroidCount)
	{
		auto filter = [&](const std::unique_ptr<Asteroid>& ast)
		{
			return !ast->IsAlive;
		};
		std::vector<std::unique_ptr<Asteroid>>::iterator it = std::find_if(asteroids.begin(), asteroids.end(), filter);
		Asteroid* firstDead = it->get();
		firstDead->Init();
	}
}

void AsteroidManager::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Camera* camera)
{
	for(int i = 0; i < asteroids.size(); i++)
	{
		asteroids[i]->Draw(context, camera);
	}
}

bool AsteroidManager::CheckOverlap(Entity* entity, std::vector<Asteroid*>* collidedAsteroids)
{
	for(int i = 0; i < asteroids.size(); i++)
	{
		SphereCollider* astCol = asteroids[i]->GetCollider();
		if(entity->GetCollider()->CheckOverlap(astCol))
		{
			collidedAsteroids->push_back(asteroids[i].get());
		}
	}

	return collidedAsteroids->size() > 0;
}
