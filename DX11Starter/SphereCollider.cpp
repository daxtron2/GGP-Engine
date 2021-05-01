#include "SphereCollider.h"

using namespace DirectX;
SphereCollider::SphereCollider(Transform* attachedTransform, float sphereRadius)
{
	center = attachedTransform;
	radius = sphereRadius;
}

bool SphereCollider::CheckOverlap(SphereCollider* otherCollider)
{
	XMFLOAT3 thisPos = center->GetPosition();
	XMFLOAT3 otherPos = otherCollider->center->GetPosition();


}
