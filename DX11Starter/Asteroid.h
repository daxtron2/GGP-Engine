#pragma once
#include "Entity.h"
class Asteroid :
    public Entity
{
private:
    float speed;
    Vector3 rotationDirection;

public:
    Vector3 movementDirection;
    bool IsAlive;
    bool DyingSoon;

    Asteroid(std::shared_ptr<Mesh> _mesh, std::shared_ptr<Material> _material);

    void Init();
    void Update(float deltaTime, float totalTime) override;
    void Kill();   
    void Die();


};

