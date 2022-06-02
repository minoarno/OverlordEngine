#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

namespace MathHelper
{
	inline XMFLOAT3 QuaternionToEuler(XMFLOAT4 q)
	{
		XMFLOAT3 euler{};

		euler.y = atan2f(2.f * q.x * q.w + 2.f * q.y * q.z, 1.f - 2.f * (q.z * q.z + q.w * q.w));     // Yaw 
		euler.x = asinf(2.f * (q.x * q.z - q.w * q.y));                             // Pitch 
		euler.z = atan2f(2.f * q.x * q.y + 2.f * q.z * q.w, 1.f - 2.f * (q.y * q.y + q.z * q.z));      // Roll 

		return euler;
	}

	inline bool XMFloat4Equals(const XMFLOAT4& a, const XMFLOAT4& b)
	{
		return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
	}

	inline bool XMFloat3Equals(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}

	inline float SquaredDistance(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		return pow(b.x - a.x,2.f) + pow(b.y - a.y, 2.f) + pow(b.z - a.z, 2.f);
	}

	inline bool XMFloat2Equals(const XMFLOAT2& a, const XMFLOAT2& b)
	{
		return a.x == b.x && a.y == b.y;
	}

	inline float randF(float min, float max)
	{
		const float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		const float diff = max - min;
		const float r = random * diff;
		return min + r;
	}

	inline LONG BinaryClamp(LONG x)
	{
		if (x > 0) return 1;
		if (x < 0) return -1;
		return 0;
	}

	template<typename T>
	void Clamp(T& value, T hi, T lo)
	{
		if (value > hi)
			value = hi;

		if (value < lo)
			value = lo;
	}

	//https://stackoverflow.com/questions/12435671/quaternion-lookat-function
	// just in case you need that function also
	inline XMFLOAT4 CreateFromAxisAngle(const XMFLOAT3& axis, float angle)
	{
		float halfAngle = angle * .5f;
		float s = float{ sin(halfAngle) };
		XMFLOAT4 q;
		q.x = axis.x * s;
		q.y = axis.y * s;
		q.z = axis.z * s;
		q.w = float{ cos(halfAngle) };
		return q;
	}

	inline XMFLOAT4 LookAt(const XMFLOAT3& sourcePoint, const XMFLOAT3& destPoint)
	{
		float epsilon{ 0.000001f };
		XMVECTOR forwardVector = XMVector3Normalize(XMVectorSubtract(XMLoadFloat3(&destPoint), XMLoadFloat3(&sourcePoint)));

		float dot = XMVector3Dot({0,0,1}, forwardVector).m128_f32[0];

		if (abs(dot - (-1.0f)) < epsilon)
		{
			return XMFLOAT4(0, 1, 0, float{M_PI});
		}
		if (abs(dot - (1.0f)) < epsilon)
		{
			return XMFLOAT4{0,0,0,1};
		}

		float rotAngle = float{ acos(dot) };
		XMFLOAT3 rotAxis{};
		XMStoreFloat3(&rotAxis, XMVector3Normalize(XMVector3Cross({ 0,0,1 }, forwardVector)));
		return CreateFromAxisAngle(rotAxis, rotAngle);
	}
}