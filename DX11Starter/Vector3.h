#pragma once
#include <DirectXMath.h>
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

	//setters
	void Set(float x, float y, float z);
	void Set(DirectX::XMFLOAT3 vector);
	void X(float x);
	void Y(float y);
	void Z(float z);

	//ops
	//Rotate by vector with euler(x,y,z/pitch,yaw,roll)
	void Rotate(Vector3 b);
	void Rotate(float x, float y, float z);
	void Normalize();
	Vector3 operator+(const Vector3& b);
	Vector3 operator-(const Vector3& b);
	Vector3 operator*(float b);
};

