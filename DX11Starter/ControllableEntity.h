#pragma once
#include "Entity.h"
#include "Camera.h"
#include "Asteroid.h"

class ControllableEntity : public Entity
{
private:
	Camera* camera;
	float movementSpeed;
	InputManager* Input;
	std::vector<Asteroid*> asteroids;

public:
	ControllableEntity(std::shared_ptr<Mesh>, std::shared_ptr<Material>, Camera*);
	void Update(float deltaTime, float totalTime) override;

	void AddAsteroid(Asteroid*);
};
