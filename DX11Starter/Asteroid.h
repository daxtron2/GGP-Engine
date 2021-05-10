#pragma once
#include "Entity.h"

class Asteroid :
    public Entity
{
private:
    float speed;
    Vector3 rotationDirection;

    Vector3 tweenScale;
    bool isTweening;
    Entity* player;

public:
    Vector3 movementDirection;
    bool IsAlive;
    bool DyingSoon;
    bool ScoreCollected;

    Asteroid(std::shared_ptr<Mesh> _mesh, std::shared_ptr<Material> _material, Entity* _player);

    void Init();
    void Update(float deltaTime, float totalTime) override;
    void Kill();   
    void Die();


};

