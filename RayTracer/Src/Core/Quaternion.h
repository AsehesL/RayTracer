#pragma once
#include "Vector.h"

typedef struct Quaternion
{
public:
	Quaternion(double x, double y, double z, double w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	Quaternion() : Quaternion(0.0, 0.0, 0.0, 0.0)
	{
	}

	Quaternion(const Quaternion& quaternion) : Quaternion(quaternion.x, quaternion.y, quaternion.z, quaternion.w)
	{
	}
	
	void EulerAngle(double& eulerX, double& eulerY, double& eulerZ) const
	{
		long sp = -2.0 * (y * z - w * x);

		if (abs(sp) > 0.9999)
		{
			eulerX = 1.570796 * sp;
			eulerY = atan2(-x * z + w * y, 0.5 - y * y - z * z);
			eulerZ = 0.0;
		}
		else
		{
			eulerX = asin(sp);
			eulerY = atan2(x * z + w * y, 0.5 - x * x - y * y);
			eulerZ = atan2(x * y + w * z, 0.5 - x * x - z * z);
		}
		eulerX *= Math::RadToDeg;
		eulerY *= Math::RadToDeg;
		eulerZ *= Math::RadToDeg;
	}

	void EulerAngle(Vector3& euler) const
	{
		double sp = -2.0 * (y * z - w * x);

		if (abs(sp) > 0.9999)
		{
			euler.x = 1.570796 * sp;
			euler.y = atan2(-x * z + w * y, 0.5 - y * y - z * z);
			euler.z = 0.0;
		}
		else
		{
			euler.x = asin(sp);
			euler.y = atan2(x * z + w * y, 0.5 - x * x - y * y);
			euler.z = atan2(x * y + w * z, 0.5 - x * x - z * z);
		}
		euler.x *= Math::RadToDeg;
		euler.y *= Math::RadToDeg;
		euler.z *= Math::RadToDeg;
	}

	Vector3 EulerAngle() const
	{
		Vector3 out;
		EulerAngle(out);
		return out;
	}

	double Magnitude() const
	{
		return sqrt(x * x + y * y + z * z + w * w);
	}

	double& operator [](int index)
	{
		if (index < 0 || index >= 4)
		{
			throw "invalid index!";
		}
		switch (index)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
		default:
			break;
		}
		return x;
	}

	double operator [](int index) const
	{
		if (index < 0 || index >= 4)
		{
			throw "invalid index!";
		}
		switch (index)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
		default:
			break;
		}
	}

	bool operator ==(const Quaternion& quaternion) const
	{
		return IS_DOUBLE_EQUAL(x, quaternion.x) && IS_DOUBLE_EQUAL(y, quaternion.y) && IS_DOUBLE_EQUAL(z, quaternion.z) && IS_DOUBLE_EQUAL(w, quaternion.w);
	}

	bool operator !=(const Quaternion& quaternion) const
	{
		return !IS_DOUBLE_EQUAL(x, quaternion.x) || !IS_DOUBLE_EQUAL(y, quaternion.y) || !IS_DOUBLE_EQUAL(z, quaternion.z) || !IS_DOUBLE_EQUAL(w, quaternion.w);
	}

	Quaternion operator * (const Quaternion& b) const
	{
		Quaternion result;
		result.x = b.w * x + b.x * w + b.z * y - b.y * z;
		result.y = b.w * y + b.y * w + b.x * z - b.z * x;
		result.z = b.w * z + b.z * w + b.y * x - b.x * y;
		result.w = b.w * w - b.x * x - b.y * y - b.z * z;
		return result;
	}
	//Quaternion& operator *= (const Quaternion&);

	static void Lerp(const Quaternion& a, const Quaternion& b, double t, Quaternion& out)
	{
		double t_ = 1 - t;
		out.x = t_ * a.x + t * b.x;
		out.y = t_ * a.y + t * b.y;
		out.z = t_ * a.z + t * b.z;
		out.w = t_ * a.w + t * b.w;
	}

	static Quaternion Lerp(const Quaternion& a, const Quaternion& b, double t)
	{
		Quaternion out;
		Lerp(a, b, t, out);
		return out;
	}

	static void Slerp(const Quaternion& a, const Quaternion& b, double t, Quaternion& out)
	{
		double dot = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;

		Quaternion temp = b;

		if (dot < 0.0) {
			temp = Quaternion(b.x * -1.0, b.y * -1.0, b.z * -1.0, b.w * -1.0);
			dot = -dot;
		}

		const double DOT_THRESHOLD = 0.9995;

		if (dot > DOT_THRESHOLD) {

			out = Quaternion(a.x + t * (temp.x - a.x), a.y + t * (temp.y - a.y), a.z + t * (temp.z - a.z), a.w + t * (temp.w - a.w));
			return;
		}

		double theta_0 = acos(dot);
		double theta = theta_0 * t;
		double sin_theta = sin(theta);
		double sin_theta_0 = sin(theta_0);

		double s0 = cos(theta) - dot * sin_theta / sin_theta_0;
		double s1 = sin_theta / sin_theta_0;

		out = Quaternion(s0 * a.x + s1 * temp.x, s0 * a.y + s1 * temp.y, s0 * a.z + s1 * temp.z, s0 * a.w + s1 * temp.w);
	}

	static Quaternion Slerp(const Quaternion& a, const Quaternion& b, double t)
	{
		Quaternion out;
		Slerp(a, b, t, out);
		return out;
	}

	static double Dot(const Quaternion a, const Quaternion b)
	{
		return a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
	}

	static void Euler(Quaternion* rotation, double eulerX, double eulerY, double eulerZ)
	{
		double cosx = cos(eulerX * Math::DegToRad * 0.5);
		double cosy = cos(eulerY * Math::DegToRad * 0.5);
		double cosz = cos(eulerZ * Math::DegToRad * 0.5);

		double sinx = sin(eulerX * Math::DegToRad * 0.5);
		double siny = sin(eulerY * Math::DegToRad * 0.5);
		double sinz = sin(eulerZ * Math::DegToRad * 0.5);

		rotation->x = cosy * sinx * cosz + siny * cosx * sinz;
		rotation->y = siny * cosx * cosz - cosy * sinx * sinz;
		rotation->z = cosy * cosx * sinz - siny * sinx * cosz;
		rotation->w = cosy * cosx * cosz + siny * sinx * sinz;
	}

	static void Euler(Quaternion* rotation, const Vector3& euler)
	{
		double cosx = cos(euler.x * Math::DegToRad * 0.5);
		double cosy = cos(euler.y * Math::DegToRad * 0.5);
		double cosz = cos(euler.z * Math::DegToRad * 0.5);

		double sinx = sin(euler.x * Math::DegToRad * 0.5);
		double siny = sin(euler.y * Math::DegToRad * 0.5);
		double sinz = sin(euler.z * Math::DegToRad * 0.5);

		rotation->x = cosy * sinx * cosz + siny * cosx * sinz;
		rotation->y = siny * cosx * cosz - cosy * sinx * sinz;
		rotation->z = cosy * cosx * sinz - siny * sinx * cosz;
		rotation->w = cosy * cosx * cosz + siny * sinx * sinz;
	}

	static Quaternion Inverse(const Quaternion& in)
	{
		double im = 1.0 / in.Magnitude();
		return Quaternion(-in.x * im, -in.y * im, -in.z * im, in.w * im);
	}

public:
	double x, y, z, w;
} Quaternion;

#define QUATERNION_IDENTITY Quaternion(0, 0, 0, 1)