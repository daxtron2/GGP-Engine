#pragma once
#include "Entity.h"
class Asteroid :
    public Entity
{
private:
    float speed;
    Vector3 rotationDirection;
    Vector3 movementDirection;

public:
    Asteroid(std::shared_ptr<Mesh> _mesh, std::shared_ptr<Material> _material);

    void Init();
    void Update(float deltaTime, float totalTime) override;
    void Kill();
   

    bool IsAlive;

};

