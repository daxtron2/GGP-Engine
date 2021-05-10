#include "SphereCollider.h"

using namespace DirectX;

SphereCollider::SphereCollider(Transform* attachedTransform, float sphereRadius)
{
	center = attachedTransform;
	radius = sphereRadius;
}

bool SphereCollider::CheckOverlap(SphereCollider* otherCollider)
{
	Vector3 thisPos = center->GetPosition();
	Vector3 otherPos = otherCollider->center->GetPosition();

	float sqrDistance = thisPos.SqrDistance(otherPos);
	return sqrDistance <= powf(this->radius + otherCollider->radius, 2);
}
