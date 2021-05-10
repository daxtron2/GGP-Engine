#include "Asteroid.h"

Asteroid::Asteroid(std::shared_ptr<Mesh> _mesh, std::shared_ptr<Material> _material, Entity* _player) : Entity(_mesh, _material)
{
	player = _player;
	Die();
}

void Asteroid::Init()
{
	IsAlive = true;
	DyingSoon = false;
	ScoreCollected = false;
	transform.SetScale(0, 0, 0);
	isTweening = true;
	tweenScale = Vector3(1, 1, 1);

	//random position
	float randLength = Vector3::GetRandomFloat(0.f, 5.f);
	Vector3 randPosInCircle = (Vector3::GetRandomUnitVector() * randLength) ;
	int zVariation = rand() % 10;
	randPosInCircle.Z(10.f + zVariation);
	transform.SetPosition(randPosInCircle + player->GetTransform()->GetPosition());

	//random speed
	speed = Vector3::GetRandomFloat(1.f, 3.f);

	//random rotation
	rotationDirection = Vector3::GetRandomUnitVector();

	movementDirection = Vector3(0, 0, -1);
}

void Asteroid::Update(float deltaTime, float totalTime)
{
	transform.MoveAlong(movementDirection, deltaTime * speed);

	transform.Rotate(rotationDirection * deltaTime);
	
	if(isTweening)
	{
		Vector3 currScale = transform.GetScale();
		if(currScale != tweenScale)
		{
			if(currScale.SqrDistance(tweenScale) < .001f)
			{
				currScale = tweenScale;
			}
			else
			{
				Vector3 tweenThisFrame = tweenScale - currScale;
				tweenThisFrame.Normalize();
				tweenThisFrame *= (deltaTime * speed * .25f);
				currScale += tweenThisFrame;
			}
		}
		else
		{
			isTweening = false;
		}

		transform.SetScale(currScale);
	}

	Vector3 currPos = transform.GetPosition();

	if(currPos.Z() < -3.f)
	{
		Kill();
	}

	if(IsAlive || DyingSoon)
	{
		if(transform.GetScale() == Vector3(0, 0, 0))
		{
			Die();
		}
	}
}

void Asteroid::Kill()
{
	DyingSoon = true;
	tweenScale = Vector3(0, 0, 0);
	isTweening = true;
	ScoreCollected = true;
}

void Asteroid::Die()
{
	IsAlive = false;
	DyingSoon = false;
	isTweening = false;
	ScoreCollected = true;
	transform.SetPosition(0, -1000, 0);
	movementDirection = Vector3(0, 0, 0);
	rotationDirection = Vector3(0, 0, 0);
	speed = 0.f;
}
