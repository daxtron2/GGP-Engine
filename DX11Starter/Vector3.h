#pragma once
#include <DirectXMath.h>
#include <cstdlib>
class Vector3
{
private:
	DirectX::XMFLOAT3 float3;

public:	
	//ctors
	Vector3();
	Vector3(float x, float y, float z);
	Vector3(DirectX::XMFLOAT3 vector);

	//getters
	float X();
	float Y();
	float Z();
	DirectX::XMFLOAT3 GetRaw();
	float GetSqrMagnitude();
	float GetMagnitude();

	//setters
	void Set(float x, float y, float z);
	void Set(DirectX::XMFLOAT3 vector);
	void X(float x);
	void Y(float y);
	void Z(float z);

	//Rotate by vector with euler(x,y,z/pitch,yaw,roll)
	void Rotate(Vector3 b);
	void Rotate(float x, float y, float z);
	Vector3 Normalize();
	float SqrDistance(Vector3 other);
	float Distance(Vector3 other);

	//op overloards
	Vector3 operator+(const Vector3& b);
	Vector3 operator-(const Vector3& b);
	Vector3 operator*(float b);
	Vector3 operator*=(float b);
};

#ifndef DAX2_GLOBAL_MATH
#define DAX2_GLOBAL_MATH
inline float GetRandomFloat(float min, float max)
{
	return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
}

inline Vector3 GetRandomUnitVector()
{
	return (Vector3(GetRandomFloat(-1, 1), GetRandomFloat(-1, 1), GetRandomFloat(-1, 1))).Normalize();
}
#endif