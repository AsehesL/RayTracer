#pragma once
#include "Math.h"

typedef struct Vector2
{
public:
	Vector2(double x, double y)
	{
		this->x = x;
		this->y = y;
	}

	Vector2() : Vector2(0, 0)
	{
	}

	Vector2(const Vector2& vector) : Vector2(vector.x, vector.y)
	{
	}

	double Magnitude() const
	{
		return sqrt(x * x + y * y);
	}

	double SqrMagnitude() const
	{
		return x * x + y * y;
	}

	void Normalize()
	{
		double m = Magnitude();
		if (m > 0.0)
		{
			double im = 1.0 / m;
			x *= im;
			y *= im;
		}
		else {
			x = 0.0;
			y = 0.0;
		}
	}

	void GetNormalized(Vector2& out) const
	{
		out.x = x;
		out.y = y;
		out.Normalize();
	}

	Vector2 GetNormalized() const
	{
		Vector2 out;
		GetNormalized(out);
		return out;
	}

	void Scale(const Vector2& scale)
	{
		x *= scale.x;
		y *= scale.y;
	}

	double& operator [](int index)
	{
		if (index < 0 || index >= 2)
		{
			throw "invalid index!";
		}
		switch (index)
		{
		case 0:
			return x;
		case 1:
			return y;
		}
	}

	double operator [](int index) const
	{
		if (index < 0 || index >= 2)
		{
			throw "invalid index!";
		}
		switch (index)
		{
		case 0:
			return x;
		case 1:
			return y;
		default:
			return x;
		}
	}

	bool operator ==(const Vector2& vector) const
	{
		return IS_DOUBLE_EQUAL(x, vector.x) && IS_DOUBLE_EQUAL(y, vector.y);
	}

	bool operator !=(const Vector2& vector) const
	{
		return !IS_DOUBLE_EQUAL(x, vector.x) || !IS_DOUBLE_EQUAL(y, vector.y);
	}

	Vector2 operator +(const Vector2& vector) const
	{
		return Vector2(x + vector.x, y + vector.y);
	}

	Vector2 operator-(const Vector2& vector) const
	{
		return Vector2(x - vector.x, y - vector.y);
	}

	Vector2 operator*(double scale) const
	{
		return Vector2(x * scale, y * scale);
	}

	Vector2 operator/(double scale) const
	{
		double in = 1.0 / scale;
		return Vector2(x * in, y * in);
	}

	friend Vector2 operator * (double scale, const Vector2& vector)
	{
		return Vector2(scale * vector.x, scale * vector.y);
	}

	Vector2& operator += (const Vector2& vector)
	{
		x += vector.x;
		y += vector.y;
		return *this;
	}

	Vector2& operator -= (const Vector2& vector)
	{
		x -= vector.x;
		y -= vector.y;
		return *this;
	}

	Vector2& operator *= (double scale)
	{
		x *= scale;
		y *= scale;
		return *this;
	}

	Vector2& operator /= (double scale)
	{
		double in = 1.0 / scale;
		x *= in;
		y *= in;
		return *this;
	}

	static Vector2 Lerp(const Vector2& a, const Vector2& b, double t)
	{
		return Vector2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t);
	}

	static void Lerp(const Vector2& a, const Vector2& b, double t, Vector2& out)
	{
		out = Vector2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t);
	}

	static Vector2 Lerp01(const Vector2& a, const Vector2& b, double t)
	{
		if (t < 0.0)
			t = 0.0;
		else if (t > 1.0)
			t = 1.0;
		return Lerp(a, b, t);
	}

	static void Lerp01(const Vector2& a, const Vector2& b, double t, Vector2& out)
	{
		if (t < 0.0)
			t = 0.0;
		else if (t > 1.0)
			t = 1.0;
		Lerp(a, b, t, out);
	}

	static double Dot(const Vector2& lhs, const Vector2& rhs)
	{
		return lhs.x * rhs.x + lhs.y + rhs.y;
	}

	static Vector2 Reflect(const Vector2& direction, const Vector2& normal)
	{
		return 2.0 * Vector2::Dot(normal, direction) * normal - direction;
	}

	static void Reflect(const Vector2& direction, const Vector2& normal, Vector2& out)
	{
		out = 2.0 * Vector2::Dot(normal, direction) * normal - direction;
	}

	static Vector2 Refract(const Vector2& direction, const Vector2& normal, double eta)
	{
		double cosi = Vector2::Dot(-1.0 * direction, normal);
		double cost2 = 1.0 - eta * eta * (1.0 - cosi * cosi);
		Vector2 t = eta * direction + ((eta * cosi - sqrt(abs(cost2))) * normal);
		double v = cost2 > 0 ? 1.0 : 0.0;
		return Vector2(v * t.x, v * t.y);
	}

	static void Refract(const Vector2& direction, const Vector2& normal, double eta, Vector2& out)
	{
		double cosi = Vector2::Dot(-1.0 * direction, normal);
		double cost2 = 1.0 - eta * eta * (1.0 - cosi * cosi);
		Vector2 t = eta * direction + ((eta * cosi - sqrt(abs(cost2))) * normal);
		double v = cost2 > 0 ? 1.0 : 0.0;
		out = Vector2(v * t.x, v * t.y);
	}

	static double Angle(const Vector2& fromvec, const Vector2& tovec)
	{
		Vector2 fn = fromvec.GetNormalized();
		Vector2 tn = tovec.GetNormalized();
		double v = Vector2::Dot(fn, tn);
		if (v < -1.0)
			v = -1.0;
		else if (v > 1.0)
			v = 1.0;
		return acos(v) * 57.29578;
	}
	
	static double Distance(const Vector2& lhs, const Vector2& rhs)
	{
		return (lhs - rhs).Magnitude();
	}

	static double Distance2(const Vector2& lhs, const Vector2& rhs)
	{
		return (lhs - rhs).SqrMagnitude();
	}

	static Vector2 Max(const Vector2& lhs, const Vector2& rhs)
	{
		return Vector2(Math::Max(lhs.x, rhs.x), Math::Max(lhs.y, rhs.y));
	}

	static void Max(const Vector2& lhs, const Vector2& rhs, Vector2& out)
	{
		out = Vector2(Math::Max(lhs.x, rhs.x), Math::Max(lhs.y, rhs.y));
	}

	static Vector2 Min(const Vector2& lhs, const Vector2& rhs)
	{
		return Vector2(Math::Min(lhs.x, rhs.x), Math::Min(lhs.y, rhs.y));
	}

	static void Min(const Vector2& lhs, const Vector2& rhs, Vector2& out)
	{
		out = Vector2(Math::Min(lhs.x, rhs.x), Math::Min(lhs.y, rhs.y));
	}

	static Vector2 Scale(const Vector2& a, const Vector2& b)
	{
		return Vector2(a.x * b.x, a.y * b.y);
	}

	static void Scale(const Vector2& a, const Vector2& b, Vector2& out)
	{
		out = Vector2(a.x * b.x, a.y * b.y);
	}

public:
	double x, y;
} Vector2;

typedef struct Vector3
{
public:
	Vector3(double x, double y, double z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	Vector3() : Vector3(0.0, 0.0, 0.0)
	{
	}

	Vector3(const Vector3& vector) : Vector3(vector.x, vector.y, vector.z)
	{
	}

	double Magnitude() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	double SqrMagnitude() const
	{
		return x * x + y * y + z * z;
	}

	void Normalize()
	{
		double m = Magnitude();
		if (m > 0.0)
		{
			double im = 1.0 / m;
			x *= im;
			y *= im;
			z *= im;
		}
		else {
			x = 0.0;
			y = 0.0;
			z = 0.0;
		}
	}

	void GetNormalized(Vector3& out) const
	{
		out.x = x;
		out.y = y;
		out.z = z;
		out.Normalize();
	}

	Vector3 GetNormalized() const
	{
		Vector3 out;
		GetNormalized(out);
		return out;
	}

	void Scale(const Vector3& scale)
	{
		x *= scale.x;
		y *= scale.y;
		z *= scale.z;
	}

	double& operator [](int index)
	{
		if (index < 0 || index >= 3)
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
		}
	}

	double operator [](int index) const
	{
		if (index < 0 || index >= 3)
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
		default:
			return x;
		}
	}

	bool operator ==(const Vector3& vector) const
	{
		return IS_DOUBLE_EQUAL(x, vector.x) && IS_DOUBLE_EQUAL(y, vector.y) && IS_DOUBLE_EQUAL(z, vector.z);
	}

	bool operator !=(const Vector3& vector) const
	{
		return !IS_DOUBLE_EQUAL(x, vector.x) || !IS_DOUBLE_EQUAL(y, vector.y) || !IS_DOUBLE_EQUAL(z, vector.z);
	}

	Vector3 operator +(const Vector3& vector) const
	{
		return Vector3(x + vector.x, y + vector.y, z + vector.z);
	}

	Vector3 operator-(const Vector3& vector) const
	{
		return Vector3(x - vector.x, y - vector.y, z - vector.z);
	}

	Vector3 operator*(double scale) const
	{
		return Vector3(x * scale, y * scale, z * scale);
	}

	Vector3 operator/(double scale) const
	{
		double in = 1.0 / scale;
		return Vector3(x * in, y * in, z * in);
	}

	friend Vector3 operator * (double scale, const Vector3& vector)
	{
		return Vector3(scale * vector.x, scale * vector.y, scale * vector.z);
	}

	Vector3& operator += (const Vector3& vector)
	{
		x += vector.x;
		y += vector.y;
		z += vector.z;
		return *this;
	}

	Vector3& operator -= (const Vector3& vector)
	{
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;
		return *this;
	}

	Vector3& operator *= (double scale)
	{
		x *= scale;
		y *= scale;
		z *= scale;
		return *this;
	}

	Vector3& operator /= (double scale)
	{
		double in = 1.0 / scale;
		x *= in;
		y *= in;
		z *= in;
		return *this;
	}

	static Vector3 Lerp(const Vector3& a, const Vector3& b, double t)
	{
		return Vector3(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t);
	}

	static void Lerp(const Vector3& a, const Vector3& b, double t, Vector3& out)
	{
		out = Vector3(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t);
	}

	static Vector3 Lerp01(const Vector3& a, const Vector3& b, double t)
	{
		if (t < 0.0)
			t = 0.0;
		else if (t > 1.0)
			t = 1.0;
		return Lerp(a, b, t);
	}

	static void Lerp01(const Vector3& a, const Vector3& b, double t, Vector3& out)
	{
		if (t < 0.0)
			t = 0.0;
		else if (t > 1.0)
			t = 1.0;
		Lerp(a, b, t, out);
	}

	static double Dot(const Vector3& lhs, const Vector3& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}

	static Vector3 Reflect(const Vector3& direction, const Vector3& normal)
	{
		return 2.0 * Dot(normal, direction) * normal - direction;
	}

	static void Reflect(const Vector3& direction, const Vector3& normal, Vector3& out)
	{
		out = 2.0 * Dot(normal, direction) * normal - direction;
	}

	static bool Refract(const Vector3& direction, const Vector3& normal, double eta, Vector3& out)
	{
		double cosi = Dot(-1.0 * direction, normal);
		double cost2 = 1.0 - eta * eta * (1.0 - cosi * cosi);
		if (cost2 > 0.0)
		{
			out = eta * direction + ((eta * cosi - sqrt(abs(cost2))) * normal);
			return true;
		}
		return false;
	}

	static double Angle(const Vector3& fromvec, const Vector3& tovec)
	{
		Vector3 fn = fromvec.GetNormalized();
		Vector3 tn = tovec.GetNormalized();
		double v = Dot(fn, tn);
		if (v < -1.0)
			v = -1.0;
		if (v > 1.0)
			v = 1.0;
		return acos(v) * 57.29578;
	}

	static double Distance(const Vector3& lhs, const Vector3& rhs)
	{
		Vector3 v = lhs - rhs;
		return v.Magnitude();
	}

	static double Distance2(const Vector3& lhs, const Vector3& rhs)
	{
		Vector3 v = lhs - rhs;
		return v.SqrMagnitude();
	}

	static Vector3 Max(const Vector3& lhs, const Vector3& rhs)
	{
		return Vector3(Math::Max(lhs.x, rhs.x), Math::Max(lhs.y, rhs.y), Math::Max(lhs.z, rhs.z));
	}

	static void Max(const Vector3& lhs, const Vector3& rhs, Vector3& out)
	{
		out = Vector3(Math::Max(lhs.x, rhs.x), Math::Max(lhs.y, rhs.y), Math::Max(lhs.z, rhs.z));
	}

	static Vector3 Min(const Vector3& lhs, const Vector3& rhs)
	{
		return Vector3(Math::Min(lhs.x, rhs.x), Math::Min(lhs.y, rhs.y), Math::Min(lhs.z, rhs.z));
	}

	static void Min(const Vector3& lhs, const Vector3& rhs, Vector3& out)
	{
		out = Vector3(Math::Min(lhs.x, rhs.x), Math::Min(lhs.y, rhs.y), Math::Min(lhs.z, rhs.z));
	}

	static Vector3 Scale(const Vector3& a, const Vector3& b)
	{
		return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
	}

	static void Scale(const Vector3& a, const Vector3& b, Vector3& out)
	{
		out = Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
	}

	static Vector3 Cross(const Vector3& lhs, const Vector3& rhs)
	{
		return Vector3(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
	}

	static void Cross(const Vector3& lhs, const Vector3& rhs, Vector3& out)
	{
		out = Vector3(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
	}

	static void Project(const Vector3& vector, const Vector3& normal, Vector3& out)
	{
		double n = Vector3::Dot(normal, normal);
		if (n >= FLT_EPSILON)
			out = normal * Dot(vector, normal) / n;
	}

	static Vector3 Project(const Vector3& vector, const Vector3& normal)
	{
		Vector3 out;
		Project(vector, normal, out);
		return out;
	}

	static void ProjectOnPlane(const Vector3& vector, const Vector3& planeNormal, Vector3& out)
	{
		Vector3 pj;
		Project(vector, planeNormal, pj);
		out = vector - pj;
	}

	static Vector3 ProjectOnPlane(const Vector3& vector, const Vector3& planeNormal)
	{
		Vector3 out;
		ProjectOnPlane(vector, planeNormal, out);
		return out;
	}

public:
	double x, y, z;
} Vector3;

typedef struct Vector4
{
public:
	Vector4(double x, double y, double z, double w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	Vector4() : Vector4(0.0, 0.0, 0.0, 0.0)
	{
	}

	Vector4(const Vector4& vector) : Vector4(vector.x, vector.y, vector.z, vector.w)
	{
	}

	double Magnitude() const
	{
		return sqrt(x * x + y * y + z * z + w * w);
	}

	double SqrMagnitude() const
	{
		return x * x + y * y + z * z + w * w;
	}

	Vector3 xyz() const
	{
		return Vector3(x, y, z);
	}

	void Normalize()
	{
		double m = Magnitude();
		if (m > 0.0)
		{
			double im = 1.0 / m;
			x *= im;
			y *= im;
			z *= im;
			w *= im;
		}
		else {
			x = 0.0;
			y = 0.0;
			z = 0.0;
			w = 0.0;
		}
	}

	void GetNormalized(Vector4& out) const
	{
		out.x = x;
		out.y = y;
		out.z = z;
		out.w = w;
		out.Normalize();
	}

	Vector4 GetNormalized() const
	{
		Vector4 out;
		GetNormalized(out);
		return out;
	}

	void Scale(const Vector4& scale)
	{
		x *= scale.x;
		y *= scale.y;
		z *= scale.z;
		w *= scale.w;
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
		}
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
		}
	}

	bool operator ==(const Vector4& vector) const
	{
		return IS_DOUBLE_EQUAL(x, vector.x) && IS_DOUBLE_EQUAL(y, vector.y) && IS_DOUBLE_EQUAL(z, vector.z) && IS_DOUBLE_EQUAL(w, vector.w);
	}

	bool operator !=(const Vector4& vector) const
	{
		return !IS_DOUBLE_EQUAL(x, vector.x) || !IS_DOUBLE_EQUAL(y, vector.y) || !IS_DOUBLE_EQUAL(z, vector.z) || !IS_DOUBLE_EQUAL(w, vector.w);
	}

	Vector4 operator +(const Vector4& vector) const
	{
		return Vector4(x + vector.x, y + vector.y, z + vector.z, w + vector.w);
	}

	Vector4 operator-(const Vector4& vector) const
	{
		return Vector4(x - vector.x, y - vector.y, z - vector.z, w - vector.w);
	}

	Vector4 operator*(double scale) const
	{
		return Vector4(x * scale, y * scale, z * scale, w * scale);
	}

	Vector4 operator/(double scale) const
	{
		double in = 1.0 / scale;
		return Vector4(x * in, y * in, z * in, w * in);
	}

	friend Vector4 operator * (double scale, const Vector4& vector)
	{
		return Vector4(scale * vector.x, scale * vector.y, scale * vector.z, scale * vector.w);
	}

	Vector4& operator += (const Vector4& vector)
	{
		x += vector.x;
		y += vector.y;
		z += vector.z;
		w += vector.w;
		return *this;
	}

	Vector4& operator -= (const Vector4& vector)
	{
		x -= vector.x;
		y -= vector.y;
		z -= vector.z;
		w -= vector.w;
		return *this;
	}

	Vector4& operator *= (double scale)
	{
		x *= scale;
		y *= scale;
		z *= scale;
		w *= scale;
		return *this;
	}

	Vector4& operator /= (double scale)
	{
		double in = 1.0 / scale;
		x *= in;
		y *= in;
		z *= in;
		w *= in;
		return *this;
	}

	static Vector4 Lerp(const Vector4& a, const Vector4& b, double t)
	{
		return Vector4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t);
	}

	static void Lerp(const Vector4& a, const Vector4& b, double t, Vector4& out)
	{
		out = Vector4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t);
	}

	static Vector4 Lerp01(const Vector4& a, const Vector4& b, double t)
	{
		if (t < 0.0)
			t = 0.0;
		else if (t > 1.0)
			t = 1.0;
		return Lerp(a, b, t);
	}

	static void Lerp01(const Vector4& a, const Vector4& b, double t, Vector4& out)
	{
		if (t < 0.0)
			t = 0.0;
		else if (t > 1.0)
			t = 1.0;
		Lerp(a, b, t, out);
	}

	static double Dot(const Vector4& lhs, const Vector4& rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w;
	}

	static double Distance(const Vector4& lhs, const Vector4& rhs)
	{
		return (lhs - rhs).Magnitude();
	}

	static double Distance2(const Vector4& lhs, const Vector4& rhs)
	{
		return (lhs - rhs).SqrMagnitude();
	}

	static Vector4 Max(const Vector4& lhs, const Vector4& rhs)
	{
		return Vector4(Math::Max(lhs.x, rhs.x), Math::Max(lhs.y, rhs.y), Math::Max(lhs.z, rhs.z), Math::Max(lhs.w, rhs.w));
	}

	static void Max(const Vector4& lhs, const Vector4& rhs, Vector4& out)
	{
		out = Vector4(Math::Max(lhs.x, rhs.x), Math::Max(lhs.y, rhs.y), Math::Max(lhs.z, rhs.z), Math::Max(lhs.w, rhs.w));
	}

	static Vector4 Min(const Vector4& lhs, const Vector4& rhs)
	{
		return Vector4(Math::Min(lhs.x, rhs.x), Math::Min(lhs.y, rhs.y), Math::Min(lhs.z, rhs.z), Math::Min(lhs.w, rhs.w));
	}

	static void Min(const Vector4& lhs, const Vector4& rhs, Vector4& out)
	{
		out = Vector4(Math::Min(lhs.x, rhs.x), Math::Min(lhs.y, rhs.y), Math::Min(lhs.z, rhs.z), Math::Min(lhs.w, rhs.w));
	}

	static Vector4 Scale(const Vector4& a, const Vector4& b)
	{
		return Vector4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
	}

	static void Scale(const Vector4& a, const Vector4& b, Vector4& out)
	{
		out = Vector4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
	}

	static Vector4 Project(const Vector4& a, const Vector4& b)
	{
		return b * Dot(a, b) / Dot(b, b);
	}

	static void Project(const Vector4& a, const Vector4& b, Vector4& out)
	{
		out = b * Dot(a, b) / Dot(b, b);
	}

public:
	double x, y, z, w;
} Vector4;

typedef struct Vector2f
{
public:
	Vector2f(const Vector2 vector)
	{
		x = vector.x;
		y = vector.y;
	}

public:
	float x, y;
} Vector2f;

typedef struct Vector3f
{
public:
	Vector3f(const Vector3 vector)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
	}

public:
	float x, y, z;
} Vector3f;

typedef struct Vector4f
{
public:
	Vector4f()
	{
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}

	Vector4f(const Vector4 vector)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
		w = vector.w;
	}

	Vector4f(const Vector3 vector)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
		w = 0.0f;
	}

	Vector4f(const Vector2 vector)
	{
		x = vector.x;
		y = vector.y;
		z = 0;
		w = 0.0f;
	}

	Vector4f(float x, float y, float z, float w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	Vector4f(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = 0;
	}

	Vector4f(float x, float y)
	{
		this->x = x;
		this->y = y;
		this->z = 0;
		this->w = 0;
	}

	Vector4f(float x)
	{
		this->x = x;
		this->y = 0;
		this->z = 0;
		this->w = 0;
	}

public:
	float x, y, z, w;
} Vector4f;

#define DVEC2_ZERO Vector2(0.0, 0.0)
#define DVEC2_ONE Vector2(1.0, 1.0)
#define DVEC2_UP Vector2(0.0, 1.0)
#define DVEC2_DOWN Vector2(0.0, -1.0)
#define DVEC2_LEFT Vector2(1.0, 0.0)
#define DVEC2_RIGHT Vector2(-1.0, 0.0)

#define DVEC3_ZERO Vector3(0.0, 0.0, 0.0)
#define DVEC3_ONE Vector3(1.0, 1.0, 1.0)
#define DVEC3_FORWARD Vector3(0.0, 0.0, 1.0)
#define DVEC3_BACK Vector3(0.0, 0.0, -1.0)
#define DVEC3_UP Vector3(0.0, 1.0, 0.0)
#define DVEC3_DOWN Vector3(0.0, -1.0, 0.0)
#define DVEC3_LEFT Vector3(-1.0, 0.0, 0.0)
#define DVEC3_RIGHT Vector3(1.0, 0.0, 0.0)

#define DVEC4_ZERO Vector4(0.0, 0.0, 0.0, 0.0)
#define DVEC4_ONE Vector4(1.0, 1.0, 1.0, 1.0)