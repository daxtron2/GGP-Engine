#pragma once
#include "Entity.h"
#include "Camera.h"
#include "Asteroid.h"
#include "AsteroidManager.h"


class ControllableEntity : public Entity
{
private:
	Camera* camera;
	float movementSpeed;
	InputManager* Input;
	std::shared_ptr<AsteroidManager> asteroidManager;
	std::vector<Asteroid*> currentCollisions;

public:
	ControllableEntity(std::shared_ptr<Mesh>, std::shared_ptr<Material>, Camera*);

	void Update(float deltaTime, float totalTime) override;

	void AddAsteroidManager(std::shared_ptr<AsteroidManager>);
};
