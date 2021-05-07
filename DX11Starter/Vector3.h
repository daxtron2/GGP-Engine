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

	
	static float GetRandomFloat(float min, float max)
	{
		return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
	}

	static Vector3 GetRandomUnitVector()
	{
		return (Vector3(GetRandomFloat(-1, 1), GetRandomFloat(-1, 1), GetRandomFloat(-1, 1))).Normalize();
	}

	static Vector3 DotProduct(Vector3 a, Vector3 b)
	{		
		DirectX::XMFLOAT3 aRaw = a.GetRaw();
		DirectX::XMFLOAT3 bRaw = b.GetRaw();

		DirectX::XMVECTOR aVec = XMLoadFloat3(&aRaw);
		DirectX::XMVECTOR bVec = XMLoadFloat3(&bRaw);

		DirectX::XMVECTOR dotResult = DirectX::XMVector3Dot(aVec, bVec);

		Vector3 result;
		XMStoreFloat3(&result.float3, dotResult);
		return result;
	}
};

