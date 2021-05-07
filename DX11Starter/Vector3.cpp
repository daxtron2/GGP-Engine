#include "Vector3.h"

using namespace DirectX;

//ctors
Vector3::Vector3()
{
    float3 = XMFLOAT3(0, 0, 0);
}

Vector3::Vector3(float x, float y, float z)
{
    float3 = XMFLOAT3(x, y, z);
}

Vector3::Vector3(DirectX::XMFLOAT3 vector)
{
    float3 = vector;
}

//getters
float Vector3::X()
{
    return float3.x;
}

float Vector3::Y()
{
    return float3.y;
}

float Vector3::Z()
{
    return float3.z;
}

DirectX::XMFLOAT3 Vector3::GetRaw()
{
    return float3;
}

float Vector3::GetSqrMagnitude()
{
    return powf(float3.x, 2) + powf(float3.y, 2) + powf(float3.z, 2);
}

float Vector3::GetMagnitude()
{
    return sqrtf(GetSqrMagnitude());
}

//setters
void Vector3::Set(float x, float y, float z)
{
    float3.x = x;
    float3.y = y;
    float3.z = z;
}

void Vector3::Set(DirectX::XMFLOAT3 vector)
{
    float3 = vector;
}

void Vector3::X(float x)
{
    float3.x = x;
}

void Vector3::Y(float y)
{
    float3.y = y;
}

void Vector3::Z(float z)
{
    float3.z = z;
}

void Vector3::Rotate(Vector3 eulerVector)
{
    XMVECTOR rotQuat = XMQuaternionRotationRollPitchYaw(eulerVector.X(), eulerVector.Y(), eulerVector.Z());

    XMStoreFloat3(&float3, XMVector3Rotate(XMLoadFloat3(&float3), rotQuat));
}

void Vector3::Rotate(float x, float y, float z)
{
    XMVECTOR rotQuat = XMQuaternionRotationRollPitchYaw(x, y, z);

    XMStoreFloat3(&float3, XMVector3Rotate(XMLoadFloat3(&float3), rotQuat));
}

Vector3 Vector3::Normalize()
{
    XMStoreFloat3(&float3, XMVector3Normalize(XMLoadFloat3(&float3)));
    return *this;
}

float Vector3::SqrDistance(Vector3 other)
{
    return powf(other.X() - X(), 2) + powf(other.Y() - Y(), 2) + powf(other.Z() - Z(), 2);
}

float Vector3::Distance(Vector3 other)
{
    return sqrtf(SqrDistance(other));
}

//ops
Vector3 Vector3::operator+(const Vector3& b)
{
    XMVECTOR thisPos = XMLoadFloat3(&float3);
    XMVECTOR bPos = XMLoadFloat3(&b.float3);

    XMVECTOR newPos = XMVectorAdd(thisPos, bPos);

    Vector3 returned;
    XMStoreFloat3(&returned.float3, newPos);
    return returned;
}

Vector3 Vector3::operator-(const Vector3& b)
{
    XMVECTOR thisPos = XMLoadFloat3(&float3);
    XMVECTOR bPos = XMLoadFloat3(&b.float3);

    XMVECTOR newPos = XMVectorSubtract(thisPos, bPos);

    Vector3 returned;
    XMStoreFloat3(&returned.float3, newPos);
    return returned;
}

Vector3 Vector3::operator*(float b)
{
    XMVECTOR thisVec = XMLoadFloat3(&float3);
    Vector3 returned;
    XMStoreFloat3(&returned.float3, XMVectorScale(thisVec, b));
    return returned;
}

Vector3 Vector3::operator*=(float b)
{
    return *this * b;
}
