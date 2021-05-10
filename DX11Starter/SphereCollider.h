#pragma once
#include <DirectXMath.h>
#include "Transform.h"

class SphereCollider
{
private:
	Transform* center;
	float radius;

public:
	SphereCollider(Transform* attachedTransform, float sphereRadius);
	bool CheckOverlap(SphereCollider* otherCollider);
};

