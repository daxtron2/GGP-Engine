#include "AsteroidManager.h"
#include <memory>
#include "InputManager.h"
#include <iostream>

AsteroidManager::AsteroidManager(std::shared_ptr<Mesh> _astMesh, std::shared_ptr<Material> _astMat, Entity* _player)
{
	activeAsteroidCount = 50;
	//pool 100 asteroid entities
	for(int i = 0; i < 200; i++)
	{
		asteroids.push_back(std::make_unique<Asteroid>(_astMesh, _astMat, _player));
	}
}

void AsteroidManager::Update(float deltaTime, float totalTime)
{
	int aliveCount = 0;
	if(activeAsteroidCount >= asteroids.size())
	{
		activeAsteroidCount = (int)asteroids.size() - 1;
	}

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
			return !ast->IsAlive && !ast->DyingSoon;
		};
		std::vector<std::unique_ptr<Asteroid>>::iterator it = std::find_if(asteroids.begin(), asteroids.end(), filter);
		if(it != std::end(asteroids))
		{
			Asteroid* firstDead = it->get();
			firstDead->Init();
		}
	}

	static float difficultyIncreaseTimer = 0.0;

	if(difficultyIncreaseTimer > 2.f)
	{
		difficultyIncreaseTimer = 0.f;
		activeAsteroidCount++;
		//std::cout << "add asteroid" << std::endl;
	}

	difficultyIncreaseTimer += deltaTime;
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

int AsteroidManager::GetAsteroidCount()
{
	return activeAsteroidCount;
}
