#pragma once
#include "Entity.h"
class Asteroid :
    public Entity
{
public:
    Asteroid(std::shared_ptr<Mesh> _mesh, std::shared_ptr<Material> _material);
    void Update(float deltaTime, float totalTime) override;

private:
};

