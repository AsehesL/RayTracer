#pragma once
#include "Quaternion.h"

typedef struct Matrix4x4
{
public:
	Matrix4x4(double m00, double m01, double m02, double m03, 
			double m10, double m11, double m12, double m13, 
			double m20, double m21, double m22, double m23, 
			double m30, double m31, double m32, double m33)
	{
		this->m00 = m00; this->m01 = m01; this->m02 = m02; this->m03 = m03;
		this->m10 = m10; this->m11 = m11; this->m12 = m12; this->m13 = m13;
		this->m20 = m20; this->m21 = m21; this->m22 = m22; this->m23 = m23;
		this->m30 = m30; this->m31 = m31; this->m32 = m32; this->m33 = m33;
	}

	Matrix4x4() : Matrix4x4(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0)
	{
	}

	Matrix4x4(const Matrix4x4& matrix) :
		Matrix4x4(matrix.m00, matrix.m01, matrix.m02, matrix.m03,
			matrix.m10, matrix.m11, matrix.m12, matrix.m13,
			matrix.m20, matrix.m21, matrix.m22, matrix.m23,
			matrix.m30, matrix.m31, matrix.m32, matrix.m33)
	{
	}

	bool operator == (const Matrix4x4& matrix) const
	{
		if (!IS_DOUBLE_EQUAL(m00, matrix.m00))
			return false;
		if (!IS_DOUBLE_EQUAL(m01, matrix.m01))
			return false;
		if (!IS_DOUBLE_EQUAL(m02, matrix.m02))
			return false;
		if (!IS_DOUBLE_EQUAL(m03, matrix.m03))
			return false;
		if (!IS_DOUBLE_EQUAL(m10, matrix.m10))
			return false;
		if (!IS_DOUBLE_EQUAL(m11, matrix.m11))
			return false;
		if (!IS_DOUBLE_EQUAL(m12, matrix.m12))
			return false;
		if (!IS_DOUBLE_EQUAL(m13, matrix.m13))
			return false;
		if (!IS_DOUBLE_EQUAL(m20, matrix.m20))
			return false;
		if (!IS_DOUBLE_EQUAL(m21, matrix.m21))
			return false;
		if (!IS_DOUBLE_EQUAL(m22, matrix.m22))
			return false;
		if (!IS_DOUBLE_EQUAL(m23, matrix.m23))
			return false;
		if (!IS_DOUBLE_EQUAL(m30, matrix.m30))
			return false;
		if (!IS_DOUBLE_EQUAL(m31, matrix.m31))
			return false;
		if (!IS_DOUBLE_EQUAL(m32, matrix.m32))
			return false;
		if (!IS_DOUBLE_EQUAL(m33, matrix.m33))
			return false;
		return true;
	}

	bool operator != (const Matrix4x4& matrix) const
	{
		if (!IS_DOUBLE_EQUAL(m00, matrix.m00))
			return true;
		if (!IS_DOUBLE_EQUAL(m01, matrix.m01))
			return true;
		if (!IS_DOUBLE_EQUAL(m02, matrix.m02))
			return true;
		if (!IS_DOUBLE_EQUAL(m03, matrix.m03))
			return true;
		if (!IS_DOUBLE_EQUAL(m10, matrix.m10))
			return true;
		if (!IS_DOUBLE_EQUAL(m11, matrix.m11))
			return true;
		if (!IS_DOUBLE_EQUAL(m12, matrix.m12))
			return true;
		if (!IS_DOUBLE_EQUAL(m13, matrix.m13))
			return true;
		if (!IS_DOUBLE_EQUAL(m20, matrix.m20))
			return true;
		if (!IS_DOUBLE_EQUAL(m21, matrix.m21))
			return true;
		if (!IS_DOUBLE_EQUAL(m22, matrix.m22))
			return true;
		if (!IS_DOUBLE_EQUAL(m23, matrix.m23))
			return true;
		if (!IS_DOUBLE_EQUAL(m30, matrix.m30))
			return true;
		if (!IS_DOUBLE_EQUAL(m31, matrix.m31))
			return true;
		if (!IS_DOUBLE_EQUAL(m32, matrix.m32))
			return true;
		if (!IS_DOUBLE_EQUAL(m33, matrix.m33))
			return true;
		return false;
	}

	Matrix4x4 operator * (const Matrix4x4& matrix) const
	{
		double rm00 = m00 * matrix.m00 + m01 * matrix.m10 + m02 * matrix.m20 + m03 * matrix.m30;
		double rm01 = m00 * matrix.m01 + m01 * matrix.m11 + m02 * matrix.m21 + m03 * matrix.m31;
		double rm02 = m00 * matrix.m02 + m01 * matrix.m12 + m02 * matrix.m22 + m03 * matrix.m32;
		double rm03 = m00 * matrix.m03 + m01 * matrix.m13 + m02 * matrix.m23 + m03 * matrix.m33;

		double rm10 = m10 * matrix.m00 + m11 * matrix.m10 + m12 * matrix.m20 + m13 * matrix.m30;
		double rm11 = m10 * matrix.m01 + m11 * matrix.m11 + m12 * matrix.m21 + m13 * matrix.m31;
		double rm12 = m10 * matrix.m02 + m11 * matrix.m12 + m12 * matrix.m22 + m13 * matrix.m32;
		double rm13 = m10 * matrix.m03 + m11 * matrix.m13 + m12 * matrix.m23 + m13 * matrix.m33;

		double rm20 = m20 * matrix.m00 + m21 * matrix.m10 + m22 * matrix.m20 + m23 * matrix.m30;
		double rm21 = m20 * matrix.m01 + m21 * matrix.m11 + m22 * matrix.m21 + m23 * matrix.m31;
		double rm22 = m20 * matrix.m02 + m21 * matrix.m12 + m22 * matrix.m22 + m23 * matrix.m32;
		double rm23 = m20 * matrix.m03 + m21 * matrix.m13 + m22 * matrix.m23 + m23 * matrix.m33;

		double rm30 = m30 * matrix.m00 + m31 * matrix.m10 + m32 * matrix.m20 + m33 * matrix.m30;
		double rm31 = m30 * matrix.m01 + m31 * matrix.m11 + m32 * matrix.m21 + m33 * matrix.m31;
		double rm32 = m30 * matrix.m02 + m31 * matrix.m12 + m32 * matrix.m22 + m33 * matrix.m32;
		double rm33 = m30 * matrix.m03 + m31 * matrix.m13 + m32 * matrix.m23 + m33 * matrix.m33;

		return Matrix4x4(rm00, rm01, rm02, rm03,
			rm10, rm11, rm12, rm13,
			rm20, rm21, rm22, rm23,
			rm30, rm31, rm32, rm33);
	}

	Vector4 operator * (const Vector4& vector) const
	{
		return Vector4(vector.x * m00 + vector.y * m01 + vector.z * m02 + vector.w * m03,
			vector.x * m10 + vector.y * m11 + vector.z * m12 + vector.w * m13,
			vector.x * m20 + vector.y * m21 + vector.z * m22 + vector.w * m23,
			vector.x * m30 + vector.y * m31 + vector.z * m32 + vector.w * m33);
	}

	Matrix4x4& operator *= (const Matrix4x4& matrix)
	{
		double rm00 = m00 * matrix.m00 + m01 * matrix.m10 + m02 * matrix.m20 + m03 * matrix.m30;
		double rm01 = m00 * matrix.m01 + m01 * matrix.m11 + m02 * matrix.m21 + m03 * matrix.m31;
		double rm02 = m00 * matrix.m02 + m01 * matrix.m12 + m02 * matrix.m22 + m03 * matrix.m32;
		double rm03 = m00 * matrix.m03 + m01 * matrix.m13 + m02 * matrix.m23 + m03 * matrix.m33;

		double rm10 = m10 * matrix.m00 + m11 * matrix.m10 + m12 * matrix.m20 + m13 * matrix.m30;
		double rm11 = m10 * matrix.m01 + m11 * matrix.m11 + m12 * matrix.m21 + m13 * matrix.m31;
		double rm12 = m10 * matrix.m02 + m11 * matrix.m12 + m12 * matrix.m22 + m13 * matrix.m32;
		double rm13 = m10 * matrix.m03 + m11 * matrix.m13 + m12 * matrix.m23 + m13 * matrix.m33;

		double rm20 = m20 * matrix.m00 + m21 * matrix.m10 + m22 * matrix.m20 + m23 * matrix.m30;
		double rm21 = m20 * matrix.m01 + m21 * matrix.m11 + m22 * matrix.m21 + m23 * matrix.m31;
		double rm22 = m20 * matrix.m02 + m21 * matrix.m12 + m22 * matrix.m22 + m23 * matrix.m32;
		double rm23 = m20 * matrix.m03 + m21 * matrix.m13 + m22 * matrix.m23 + m23 * matrix.m33;

		double rm30 = m30 * matrix.m00 + m31 * matrix.m10 + m32 * matrix.m20 + m33 * matrix.m30;
		double rm31 = m30 * matrix.m01 + m31 * matrix.m11 + m32 * matrix.m21 + m33 * matrix.m31;
		double rm32 = m30 * matrix.m02 + m31 * matrix.m12 + m32 * matrix.m22 + m33 * matrix.m32;
		double rm33 = m30 * matrix.m03 + m31 * matrix.m13 + m32 * matrix.m23 + m33 * matrix.m33;

		m00 = rm00;
		m01 = rm01;
		m02 = rm02;
		m03 = rm03;
		m10 = rm10;
		m11 = rm11;
		m12 = rm12;
		m13 = rm13;
		m20 = rm20;
		m21 = rm21;
		m22 = rm22;
		m23 = rm23;
		m30 = rm30;
		m31 = rm31;
		m32 = rm32;
		m33 = rm33;
		return *this;
	}

	double& operator [](int index)
	{
		if (index < 0 || index >= 16)
		{
			throw "invalid index!";
		}
		switch (index)
		{
		case 0:
			return m00;
		case 1:
			return m01;
		case 2:
			return m02;
		case 3:
			return m03;
		case 4:
			return m10;
		case 5:
			return m11;
		case 6:
			return m12;
		case 7:
			return m13;
		case 8:
			return m20;
		case 9:
			return m21;
		case 10:
			return m22;
		case 11:
			return m23;
		case 12:
			return m30;
		case 13:
			return m31;
		case 14:
			return m32;
		case 15:
			return m33;
		}
	}

	double operator [](int index) const
	{
		if (index < 0 || index >= 16)
		{
			throw "invalid index!";
		}
		switch (index)
		{
		case 0:
			return m00;
		case 1:
			return m01;
		case 2:
			return m02;
		case 3:
			return m03;
		case 4:
			return m10;
		case 5:
			return m11;
		case 6:
			return m12;
		case 7:
			return m13;
		case 8:
			return m20;
		case 9:
			return m21;
		case 10:
			return m22;
		case 11:
			return m23;
		case 12:
			return m30;
		case 13:
			return m31;
		case 14:
			return m32;
		case 15:
			return m33;
		}
	}

	friend Vector4 operator*(const Vector4& vector, const Matrix4x4& matrix)
	{
		return Vector4(vector.x * matrix.m00 + vector.y * matrix.m10 + vector.z * matrix.m20 + vector.w * matrix.m30,
			vector.x * matrix.m01 + vector.y * matrix.m11 + vector.z * matrix.m21 + vector.w * matrix.m31,
			vector.x * matrix.m02 + vector.y * matrix.m12 + vector.z * matrix.m22 + vector.w * matrix.m32,
			vector.x * matrix.m03 + vector.y * matrix.m13 + vector.z * matrix.m23 + vector.w * matrix.m33);
	}

	void TransformVector(const Vector3& vector, Vector3& out) const
	{
		double x = vector.x * m00 + vector.y * m01 + vector.z * m02;
		double y = vector.x * m10 + vector.y * m11 + vector.z * m12;
		double z = vector.x * m20 + vector.y * m21 + vector.z * m22;

		out.x = x;
		out.y = y;
		out.z = z;
	}

	Vector3 TransformVector(const Vector3& vector) const
	{
		Vector3 out;
		TransformVector(vector, out);
		return out;
	}

	void TransformPoint(const Vector3& vector, Vector3& out) const
	{
		double x = vector.x * m00 + vector.y * m01 + vector.z * m02 + m03;
		double y = vector.x * m10 + vector.y * m11 + vector.z * m12 + m13;
		double z = vector.x * m20 + vector.y * m21 + vector.z * m22 + m23;
		double w = vector.x * m30 + vector.y * m31 + vector.z * m32 + m33;

		if (IS_DOUBLE_EQUAL(w, 0.0)) {
			out.x = NAN;
			out.y = NAN;
			out.z = NAN;
			return;
		}
		out.x = x / w;
		out.y = y / w;
		out.z = z / w;
	}

	Vector3 TransformPoint(const Vector3& vector) const
	{
		Vector3 out;
		TransformPoint(vector, out);
		return out;
	}

	void GetTranspose(Matrix4x4& matrix) const
	{
		matrix.m00 = m00;
		matrix.m01 = m10;
		matrix.m02 = m20;
		matrix.m03 = m30;

		matrix.m10 = m01;
		matrix.m11 = m11;
		matrix.m12 = m21;
		matrix.m13 = m31;

		matrix.m20 = m02;
		matrix.m21 = m12;
		matrix.m22 = m22;
		matrix.m23 = m32;

		matrix.m30 = m03;
		matrix.m31 = m13;
		matrix.m32 = m23;
		matrix.m33 = m33;
	}

	Matrix4x4 GetTranspose() const
	{
		Matrix4x4 out;
		GetTranspose(out);
		return out;
	}

	void Transpose()
	{
		double t01 = m01;
		m01 = m10;
		m10 = t01;

		double t02 = m02;
		m02 = m20;
		m20 = t02;

		double t03 = m03;
		m03 = m30;
		m30 = t03;

		double t12 = m12;
		m12 = m21;
		m21 = t12;

		double t13 = m13;
		m13 = m31;
		m31 = t13;

		double t23 = m23;
		m23 = m32;
		m32 = t23;
	}

	double TransposeIndex(int index) const
	{
		if (index < 0 || index >= 16)
		{
			throw "invalid index!";
		}
		switch (index)
		{
		case 0:
			return m00;
		case 1:
			return m10;
		case 2:
			return m20;
		case 3:
			return m30;
		case 4:
			return m01;
		case 5:
			return m11;
		case 6:
			return m21;
		case 7:
			return m31;
		case 8:
			return m02;
		case 9:
			return m12;
		case 10:
			return m22;
		case 11:
			return m32;
		case 12:
			return m03;
		case 13:
			return m13;
		case 14:
			return m23;
		case 15:
			return m33;
		}
	}

	void Inverse()
	{
		Matrix4x4 tmp;

		tmp[0] = m11 * m22 * m33 -
			m11 * m23 * m32 -
			m21 * m12 * m33 +
			m21 * m13 * m32 +
			m31 * m12 * m23 -
			m31 * m13 * m22;

		tmp[4] = -m10 * m22 * m33 +
			m10 * m23 * m32 +
			m20 * m12 * m33 -
			m20 * m13 * m32 -
			m30 * m12 * m23 +
			m30 * m13 * m22;

		tmp[8] = m10 * m21 * m33 -
			m10 * m23 * m31 -
			m20 * m11 * m33 +
			m20 * m13 * m31 +
			m30 * m11 * m23 -
			m30 * m13 * m21;

		tmp[12] = -m10 * m21 * m32 +
			m10 * m22 * m31 +
			m20 * m11 * m32 -
			m20 * m12 * m31 -
			m30 * m11 * m22 +
			m30 * m12 * m21;

		tmp[1] = -m01 * m22 * m33 +
			m01 * m23 * m32 +
			m21 * m02 * m33 -
			m21 * m03 * m32 -
			m31 * m02 * m23 +
			m31 * m03 * m22;

		tmp[5] = m00 * m22 * m33 -
			m00 * m23 * m32 -
			m20 * m02 * m33 +
			m20 * m03 * m32 +
			m30 * m02 * m23 -
			m30 * m03 * m22;

		tmp[9] = -m00 * m21 * m33 +
			m00 * m23 * m31 +
			m20 * m01 * m33 -
			m20 * m03 * m31 -
			m30 * m01 * m23 +
			m30 * m03 * m21;

		tmp[13] = m00 * m21 * m32 -
			m00 * m22 * m31 -
			m20 * m01 * m32 +
			m20 * m02 * m31 +
			m30 * m01 * m22 -
			m30 * m02 * m21;

		tmp[2] = m01 * m12 * m33 -
			m01 * m13 * m32 -
			m11 * m02 * m33 +
			m11 * m03 * m32 +
			m31 * m02 * m13 -
			m31 * m03 * m12;

		tmp[6] = -m00 * m12 * m33 +
			m00 * m13 * m32 +
			m10 * m02 * m33 -
			m10 * m03 * m32 -
			m30 * m02 * m13 +
			m30 * m03 * m12;

		tmp[10] = m00 * m11 * m33 -
			m00 * m13 * m31 -
			m10 * m01 * m33 +
			m10 * m03 * m31 +
			m30 * m01 * m13 -
			m30 * m03 * m11;

		tmp[14] = -m00 * m11 * m32 +
			m00 * m12 * m31 +
			m10 * m01 * m32 -
			m10 * m02 * m31 -
			m30 * m01 * m12 +
			m30 * m02 * m11;

		tmp[3] = -m01 * m12 * m23 +
			m01 * m13 * m22 +
			m11 * m02 * m23 -
			m11 * m03 * m22 -
			m21 * m02 * m13 +
			m21 * m03 * m12;

		tmp[7] = m00 * m12 * m23 -
			m00 * m13 * m22 -
			m10 * m02 * m23 +
			m10 * m03 * m22 +
			m20 * m02 * m13 -
			m20 * m03 * m12;

		tmp[11] = -m00 * m11 * m23 +
			m00 * m13 * m21 +
			m10 * m01 * m23 -
			m10 * m03 * m21 -
			m20 * m01 * m13 +
			m20 * m03 * m11;

		tmp[15] = m00 * m11 * m22 -
			m00 * m12 * m21 -
			m10 * m01 * m22 +
			m10 * m02 * m21 +
			m20 * m01 * m12 -
			m20 * m02 * m11;

		double det = m00 * tmp[0] + m01 * tmp[4] + m02 * tmp[8] + m03 * tmp[12];

		if (det == 0)
			throw "irreversible matrix!";

		det = 1.0f / det;

		for (int i = 0; i < 16; i++)
			this->operator[](i) = tmp[i] * det;
	}

	static void Perspective(Matrix4x4* matrix, double fov, double aspect, double nearplane, double farplane)
	{
		double cotfov = 1.0 / tanf(fov * Math::DegToRad * 0.5);
		double delta = 1.0 / (farplane - nearplane);

		matrix->m00 = cotfov / aspect;
		matrix->m01 = 0.0;
		matrix->m02 = 0.0;
		matrix->m03 = 0.0;

		matrix->m10 = 0.0;
		matrix->m11 = cotfov;
		matrix->m12 = 0.0;
		matrix->m13 = 0.0;

		matrix->m20 = 0.0;
		matrix->m21 = 0.0;
		matrix->m22 = farplane * delta;
		matrix->m23 = -nearplane * farplane * delta;

		matrix->m30 = 0.0;
		matrix->m31 = 0.0;
		matrix->m32 = 1.0;
		matrix->m33 = 0.0;
	}

	static void Ortho(Matrix4x4* matrix, float width, float height, double nearplane, double farplane)
	{
		double fn = 1.0 / (farplane - nearplane);

		matrix->m00 = 2.0 / width;
		matrix->m01 = 0.0;
		matrix->m02 = 0.0;
		matrix->m03 = 0.0;
		matrix->m10 = 0.0;
		matrix->m11 = 2.0 / height;
		matrix->m12 = 0.0;
		matrix->m13 = 0.0;
		matrix->m20 = 0.0;
		matrix->m21 = 0.0;
		matrix->m22 = fn;
		matrix->m23 = -nearplane * fn;
		matrix->m30 = 0.0;
		matrix->m31 = 0.0;
		matrix->m32 = 0.0;
		matrix->m33 = 1.0;
	}

	static void Identity(Matrix4x4* matrix)
	{
		matrix->m00 = 1;
		matrix->m01 = 0;
		matrix->m02 = 0;
		matrix->m03 = 0;
		matrix->m10 = 0;
		matrix->m11 = 1;
		matrix->m12 = 0;
		matrix->m13 = 0;
		matrix->m20 = 0;
		matrix->m21 = 0;
		matrix->m22 = 1;
		matrix->m23 = 0;
		matrix->m30 = 0;
		matrix->m31 = 0;
		matrix->m32 = 0;
		matrix->m33 = 1;
	}

	static void Scale(Matrix4x4* matrix, double x, double y, double z)
	{
		matrix->m00 = x;
		matrix->m01 = 0;
		matrix->m02 = 0;
		matrix->m03 = 0;

		matrix->m10 = 0;
		matrix->m11 = y;
		matrix->m12 = 0;
		matrix->m13 = 0;

		matrix->m20 = 0;
		matrix->m21 = 0;
		matrix->m22 = z;
		matrix->m23 = 0;

		matrix->m30 = 0;
		matrix->m31 = 0;
		matrix->m32 = 0;
		matrix->m33 = 1;
	}

	static void Translate(Matrix4x4* matrix, double x, double y, double z)
	{
		matrix->m00 = 1;
		matrix->m01 = 0;
		matrix->m02 = 0;
		matrix->m03 = x;

		matrix->m10 = 0;
		matrix->m11 = 1;
		matrix->m12 = 0;
		matrix->m13 = y;

		matrix->m20 = 0;
		matrix->m21 = 0;
		matrix->m22 = 1;
		matrix->m23 = z;

		matrix->m30 = 0;
		matrix->m31 = 0;
		matrix->m32 = 0;
		matrix->m33 = 1;
	}

	static void RotateX(Matrix4x4* matrix, double angle)
	{
		double cosag = cos(angle);
		double sinag = sin(angle);
		matrix->m00 = 1.0;
		matrix->m01 = 0.0;
		matrix->m02 = 0.0;
		matrix->m03 = 0.0;
		matrix->m10 = 0.0;
		matrix->m11 = cosag;
		matrix->m12 = -sinag;
		matrix->m13 = 0.0;
		matrix->m20 = 0.0;
		matrix->m21 = sinag;
		matrix->m22 = cosag;
		matrix->m23 = 0.0;
		matrix->m30 = 0.0;
		matrix->m31 = 0.0;
		matrix->m32 = 0.0;
		matrix->m33 = 1.0;
	}

	static void RotateY(Matrix4x4* matrix, double angle)
	{
		double cosag = cos(angle);
		double sinag = sin(angle);
		matrix->m00 = cosag;
		matrix->m01 = 0.0;
		matrix->m02 = sinag;
		matrix->m03 = 0.0;
		matrix->m10 = 0.0;
		matrix->m11 = 1.0;
		matrix->m12 = 0.0;
		matrix->m13 = 0.0;
		matrix->m20 = -sinag;
		matrix->m21 = 0.0;
		matrix->m22 = cosag;
		matrix->m23 = 0.0;
		matrix->m30 = 0.0;
		matrix->m31 = 0.0;
		matrix->m32 = 0.0;
		matrix->m33 = 1.0;
	}

	static void RotateZ(Matrix4x4* matrix, double angle)
	{
		double cosag = cos(angle);
		double sinag = sin(angle);
		matrix->m00 = cosag;
		matrix->m01 = -sinag;
		matrix->m02 = 0.0;
		matrix->m03 = 0.0;
		matrix->m10 = sinag;
		matrix->m11 = cosag;
		matrix->m12 = 0.0;
		matrix->m13 = 0.0;
		matrix->m20 = 0.0;
		matrix->m21 = 0.0;
		matrix->m22 = 1.0;
		matrix->m23 = 0.0;
		matrix->m30 = 0.0;
		matrix->m31 = 0.0;
		matrix->m32 = 0.0;
		matrix->m33 = 1.0;
	}

	static void Rotate(Matrix4x4* matrix, const Quaternion& rotation)
	{
		double x2 = 2.0 * rotation.x * rotation.x;
		double y2 = 2.0 * rotation.y * rotation.y;
		double z2 = 2.0 * rotation.z * rotation.z;
		double xy = 2.0 * rotation.x * rotation.y;
		double xz = 2.0 * rotation.x * rotation.z;
		double xw = 2.0 * rotation.x * rotation.w;
		double yz = 2.0 * rotation.y * rotation.z;
		double yw = 2.0 * rotation.y * rotation.w;
		double zw = 2.0 * rotation.z * rotation.w;
		matrix->m00 = 1.0 - y2 - z2;
		matrix->m01 = xy - zw;
		matrix->m02 = xz + yw;
		matrix->m03 = 0.0;
		matrix->m10 = xy + zw;
		matrix->m11 = 1.0 - x2 - z2;
		matrix->m12 = yz - xw;
		matrix->m13 = 0.0;
		matrix->m20 = xz - yw;
		matrix->m21 = yz + xw;
		matrix->m22 = 1.0 - x2 - y2;
		matrix->m23 = 0.0;
		matrix->m30 = 0.0;
		matrix->m31 = 0.0;
		matrix->m32 = 0.0;
		matrix->m33 = 1.0;
	}

	static void TRS(Matrix4x4* matrix, const Vector3& position, const Quaternion& rotation, const Vector3& scale)
	{
		double x2 = 2.0 * rotation.x * rotation.x;
		double y2 = 2.0 * rotation.y * rotation.y;
		double z2 = 2.0 * rotation.z * rotation.z;
		double xy = 2.0 * rotation.x * rotation.y;
		double xz = 2.0 * rotation.x * rotation.z;
		double xw = 2.0 * rotation.x * rotation.w;
		double yz = 2.0 * rotation.y * rotation.z;
		double yw = 2.0 * rotation.y * rotation.w;
		double zw = 2.0 * rotation.z * rotation.w;
		double ra = 1.0 - y2 - z2;
		double rb = xy + zw;
		double rc = xz - yw;
		double rd = xy - zw;
		double re = 1.0 - x2 - z2;
		double rf = yz + xw;
		double rg = xz + yw;
		double rh = yz - xw;
		double ri = 1.0 - x2 - y2;

		matrix->m00 = scale.x * ra;
		matrix->m01 = scale.y * rd;
		matrix->m02 = scale.z * rg;
		matrix->m03 = position.x;
		matrix->m10 = scale.x * rb;
		matrix->m11 = scale.y * re;
		matrix->m12 = scale.z * rh;
		matrix->m13 = position.y;
		matrix->m20 = scale.x * rc;
		matrix->m21 = scale.y * rf;
		matrix->m22 = scale.z * ri;
		matrix->m23 = position.z;
		matrix->m30 = 0.0;
		matrix->m31 = 0.0;
		matrix->m32 = 0.0;
		matrix->m33 = 1.0;
	}

	static void LookAt(Matrix4x4* matrix, const Vector3& eye, const Vector3& lookat, const Vector3& up)
	{
		Vector3 x, y, z;
		Vector3(lookat - eye).GetNormalized(z);
		Vector3::Cross(up, z, x);
		x.Normalize();
		Vector3::Cross(z, x, y);

		matrix->m00 = x.x;
		matrix->m01 = x.y;
		matrix->m02 = x.z;

		matrix->m10 = y.x;
		matrix->m11 = y.y;
		matrix->m12 = y.z;

		matrix->m20 = z.x;
		matrix->m21 = z.y;
		matrix->m22 = z.z;

		matrix->m03 = -Vector3::Dot(x, eye);
		matrix->m13 = -Vector3::Dot(y, eye);
		matrix->m23 = -Vector3::Dot(z, eye);

		matrix->m30 = 0.0;
		matrix->m31 = 0.0;
		matrix->m32 = 0.0;
		matrix->m33 = 1.0;
	}

	static void Transpose(Matrix4x4* out, const Matrix4x4& target)
	{
		out->m00 = target.m00;
		out->m11 = target.m11;
		out->m22 = target.m22;
		out->m33 = target.m33;

		double t01, t02, t03, t10, t12, t13, t20, t21, t23, t30, t31, t32;
		t01 = target.m01;
		t02 = target.m02;
		t03 = target.m03;
		t10 = target.m10;
		t12 = target.m12;
		t13 = target.m13;
		t20 = target.m20;
		t21 = target.m21;
		t23 = target.m23;
		t30 = target.m30;
		t31 = target.m31;
		t32 = target.m32;

		out->m01 = t10;
		out->m02 = t20;
		out->m03 = t30;

		out->m10 = t01;
		out->m12 = t21;
		out->m13 = t31;

		out->m20 = t02;
		out->m21 = t12;
		out->m23 = t32;

		out->m30 = t03;
		out->m31 = t13;
		out->m32 = t23;
	}

	static void Inverse(Matrix4x4* out, const Matrix4x4& target)
	{
		Matrix4x4 tmp;

		tmp[0] = target.m11 * target.m22 * target.m33 -
			target.m11 * target.m23 * target.m32 -
			target.m21 * target.m12 * target.m33 +
			target.m21 * target.m13 * target.m32 +
			target.m31 * target.m12 * target.m23 -
			target.m31 * target.m13 * target.m22;

		tmp[4] = -target.m10 * target.m22 * target.m33 +
			target.m10 * target.m23 * target.m32 +
			target.m20 * target.m12 * target.m33 -
			target.m20 * target.m13 * target.m32 -
			target.m30 * target.m12 * target.m23 +
			target.m30 * target.m13 * target.m22;

		tmp[8] = target.m10 * target.m21 * target.m33 -
			target.m10 * target.m23 * target.m31 -
			target.m20 * target.m11 * target.m33 +
			target.m20 * target.m13 * target.m31 +
			target.m30 * target.m11 * target.m23 -
			target.m30 * target.m13 * target.m21;

		tmp[12] = -target.m10 * target.m21 * target.m32 +
			target.m10 * target.m22 * target.m31 +
			target.m20 * target.m11 * target.m32 -
			target.m20 * target.m12 * target.m31 -
			target.m30 * target.m11 * target.m22 +
			target.m30 * target.m12 * target.m21;

		tmp[1] = -target.m01 * target.m22 * target.m33 +
			target.m01 * target.m23 * target.m32 +
			target.m21 * target.m02 * target.m33 -
			target.m21 * target.m03 * target.m32 -
			target.m31 * target.m02 * target.m23 +
			target.m31 * target.m03 * target.m22;

		tmp[5] = target.m00 * target.m22 * target.m33 -
			target.m00 * target.m23 * target.m32 -
			target.m20 * target.m02 * target.m33 +
			target.m20 * target.m03 * target.m32 +
			target.m30 * target.m02 * target.m23 -
			target.m30 * target.m03 * target.m22;

		tmp[9] = -target.m00 * target.m21 * target.m33 +
			target.m00 * target.m23 * target.m31 +
			target.m20 * target.m01 * target.m33 -
			target.m20 * target.m03 * target.m31 -
			target.m30 * target.m01 * target.m23 +
			target.m30 * target.m03 * target.m21;

		tmp[13] = target.m00 * target.m21 * target.m32 -
			target.m00 * target.m22 * target.m31 -
			target.m20 * target.m01 * target.m32 +
			target.m20 * target.m02 * target.m31 +
			target.m30 * target.m01 * target.m22 -
			target.m30 * target.m02 * target.m21;

		tmp[2] = target.m01 * target.m12 * target.m33 -
			target.m01 * target.m13 * target.m32 -
			target.m11 * target.m02 * target.m33 +
			target.m11 * target.m03 * target.m32 +
			target.m31 * target.m02 * target.m13 -
			target.m31 * target.m03 * target.m12;

		tmp[6] = -target.m00 * target.m12 * target.m33 +
			target.m00 * target.m13 * target.m32 +
			target.m10 * target.m02 * target.m33 -
			target.m10 * target.m03 * target.m32 -
			target.m30 * target.m02 * target.m13 +
			target.m30 * target.m03 * target.m12;

		tmp[10] = target.m00 * target.m11 * target.m33 -
			target.m00 * target.m13 * target.m31 -
			target.m10 * target.m01 * target.m33 +
			target.m10 * target.m03 * target.m31 +
			target.m30 * target.m01 * target.m13 -
			target.m30 * target.m03 * target.m11;

		tmp[14] = -target.m00 * target.m11 * target.m32 +
			target.m00 * target.m12 * target.m31 +
			target.m10 * target.m01 * target.m32 -
			target.m10 * target.m02 * target.m31 -
			target.m30 * target.m01 * target.m12 +
			target.m30 * target.m02 * target.m11;

		tmp[3] = -target.m01 * target.m12 * target.m23 +
			target.m01 * target.m13 * target.m22 +
			target.m11 * target.m02 * target.m23 -
			target.m11 * target.m03 * target.m22 -
			target.m21 * target.m02 * target.m13 +
			target.m21 * target.m03 * target.m12;

		tmp[7] = target.m00 * target.m12 * target.m23 -
			target.m00 * target.m13 * target.m22 -
			target.m10 * target.m02 * target.m23 +
			target.m10 * target.m03 * target.m22 +
			target.m20 * target.m02 * target.m13 -
			target.m20 * target.m03 * target.m12;

		tmp[11] = -target.m00 * target.m11 * target.m23 +
			target.m00 * target.m13 * target.m21 +
			target.m10 * target.m01 * target.m23 -
			target.m10 * target.m03 * target.m21 -
			target.m20 * target.m01 * target.m13 +
			target.m20 * target.m03 * target.m11;

		tmp[15] = target.m00 * target.m11 * target.m22 -
			target.m00 * target.m12 * target.m21 -
			target.m10 * target.m01 * target.m22 +
			target.m10 * target.m02 * target.m21 +
			target.m20 * target.m01 * target.m12 -
			target.m20 * target.m02 * target.m11;

		double det = target.m00 * tmp[0] + target.m01 * tmp[4] + target.m02 * tmp[8] + target.m03 * tmp[12];

		if (det == 0)
			throw "irreversible matrix!";

		det = 1.0f / det;

		for (int i = 0; i < 16; i++)
			out->operator[](i) = tmp[i] * det;
	}

public:
	double m00, m01, m02, m03;
	double m10, m11, m12, m13;
	double m20, m21, m22, m23;
	double m30, m31, m32, m33;
} Matrix4x4;

typedef struct Matrix4x4f
{
public:
	Matrix4x4f()
	{
		m00 = 0;
		m01 = 0;
		m02 = 0;
		m03 = 0;

		m10 = 0;
		m11 = 0;
		m12 = 0;
		m13 = 0;

		m20 = 0;
		m21 = 0;
		m22 = 0;
		m23 = 0;

		m30 = 0;
		m31 = 0;
		m32 = 0;
		m33 = 0;
	}
	Matrix4x4f(const Matrix4x4& matrix)
	{
		m00 = matrix.m00;
		m01 = matrix.m01;
		m02 = matrix.m02;
		m03 = matrix.m03;

		m10 = matrix.m10;
		m11 = matrix.m11;
		m12 = matrix.m12;
		m13 = matrix.m13;

		m20 = matrix.m20;
		m21 = matrix.m21;
		m22 = matrix.m22;
		m23 = matrix.m23;

		m30 = matrix.m30;
		m31 = matrix.m31;
		m32 = matrix.m32;
		m33 = matrix.m33;
	}

public:
	float m00, m01, m02, m03;
	float m10, m11, m12, m13;
	float m20, m21, m22, m23;
	float m30, m31, m32, m33;
} Matrix4x4f;

#define DMATRIX_IDENTITY Matrix4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)