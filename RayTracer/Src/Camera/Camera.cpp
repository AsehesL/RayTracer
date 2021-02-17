#include "Camera.h"
#include "../RayTracer/Sampler/Sampler.h"

Camera::Camera(unsigned int width, unsigned int height)
{
	m_width = width;
	m_height = height;
	m_fieldOfView = 60.0;
	useThinLens = false;
	radius = 0;
	focal = 0;
	m_nearClipPlane = 0.01f;
	m_halfScreenWidth = 0;
	m_halfScreenHeight = 0;
	m_viewMatrixDirty = true;
	m_directionDirty = true;
	m_projectionMatrixDirty = true;
}

Camera::~Camera()
{
}

Matrix4x4 Camera::GetWorldToViewMatrix()
{
	//RefreshTransform();
	RefreshDirection();

	if (m_viewMatrixDirty)
	{ 
		m_viewMatrix.m00 = m_right.x;
		m_viewMatrix.m01 = m_right.y;
		m_viewMatrix.m02 = m_right.z;

		m_viewMatrix.m10 = m_up.x;
		m_viewMatrix.m11 = m_up.y;
		m_viewMatrix.m12 = m_up.z;

		m_viewMatrix.m20 = m_forward.x;
		m_viewMatrix.m21 = m_forward.y;
		m_viewMatrix.m22 = m_forward.z;

		m_viewMatrix.m03 = -Vector3::Dot(m_right, m_position);
		m_viewMatrix.m13 = -Vector3::Dot(m_up, m_position);
		m_viewMatrix.m23 = -Vector3::Dot(m_forward, m_position);

		m_viewMatrix.m30 = 0.0f;
		m_viewMatrix.m31 = 0.0f;
		m_viewMatrix.m32 = 0.0f;
		m_viewMatrix.m33 = 1.0f;

		m_viewMatrixDirty = false;
	}

	return m_viewMatrix;
}

Matrix4x4 Camera::GetProjectionMatrix()
{
	float aspect = float(m_width) / float(m_height);
	m_halfScreenHeight = m_nearClipPlane * tan(m_fieldOfView * 0.5 * 0.01745329252);
	m_halfScreenWidth = aspect * m_halfScreenHeight;
	if (m_projectionMatrixDirty)
	{
		Matrix4x4::Perspective(&m_projectionMatrix, m_fieldOfView, aspect, m_nearClipPlane, 1000.0f);
		m_projectionMatrixDirty = false;
	}
	return m_projectionMatrix;
}

void Camera::ForceRefreshTransform()
{
	float aspect = float(m_width) / float(m_height);
	m_halfScreenHeight = m_nearClipPlane * tan(m_fieldOfView * 0.5 * 0.01745329252);
	m_halfScreenWidth = aspect * m_halfScreenHeight;

	m_directionDirty = true;
	RefreshDirection();
}

void Camera::GetRayFromPixel(int pixelX, int pixelY, RayTracer::SamplerBase* sampler, Ray& outRay)
{
	Vector2 sample;
	sampler->Sample(sample);
	double x = pixelX + sample.x;
	double y = pixelY + sample.y;

	x = (x / m_width) * 2.0 - 1.0;
	y = (1.0 - y / m_height) * 2.0 - 1.0;

	x = x * m_halfScreenWidth;
	y = y * m_halfScreenHeight;

	if (useThinLens)
	{
		double per = focal / m_nearClipPlane;
		x = x * per;
		y = y * per;

		Vector3 p = m_position + m_right * x + m_up * y + m_forward * focal;

		Vector2 disk;
		sampler->SampleUnitDisk(disk);

		Vector3 ori = m_position + m_right * disk.x * radius + m_up * disk.y * radius;
		Vector3 dir = p - ori;
		dir.Normalize();
		outRay = Ray(ori, dir);
	}
	else
	{ 
		Vector3 dir = m_right * x + m_up * y + m_forward * m_nearClipPlane;
		Vector3 ori = m_position + dir;
		dir.Normalize();
		outRay = Ray(ori, dir);
	}
}

//void Camera::GetRayFromPixel(double x, double y, Ray& outRay)
//{
//	x = (x / m_width) * 2.0 - 1.0;
//	y = (1.0-y / m_height) * 2.0 - 1.0;
//
//	x = x * m_halfScreenWidth;
//	y = y * m_halfScreenHeight;
//
//	if (useThinLens)
//	{
//
//	}
//	Vector3 dir = m_right * x + m_up * y + m_forward * m_nearClipPlane;
//	Vector3 ori = m_position + dir;
//	dir.Normalize();
//	outRay = Ray(ori, dir);
//}

void Camera::RefreshDirection()
{
	if (m_directionDirty)
	{
		double cosx = cos(m_euler.x * 0.01745329252 * 0.5);
		double cosy = cos(m_euler.y * 0.01745329252 * 0.5);
		double cosz = cos(m_euler.z * 0.01745329252 * 0.5);

		double sinx = sin(m_euler.x * 0.01745329252 * 0.5);
		double siny = sin(m_euler.y * 0.01745329252 * 0.5);
		double sinz = sin(m_euler.z * 0.01745329252 * 0.5);

		double rx = cosy * sinx * cosz + siny * cosx * sinz;
		double ry = siny * cosx * cosz - cosy * sinx * sinz;
		double rz = cosy * cosx * sinz - siny * sinx * cosz;
		double rw = cosy * cosx * cosz + siny * sinx * sinz;

		double x2 = 2.0 * rx * rx;
		double y2 = 2.0 * ry * ry;
		double z2 = 2.0 * rz * rz;
		double xy = 2.0 * rx * ry;
		double xz = 2.0 * rx * rz;
		double xw = 2.0 * rx * rw;
		double yz = 2.0 * ry * rz;
		double yw = 2.0 * ry * rw;
		double zw = 2.0 * rz * rw;

		double ra = 1.0 - y2 - z2;
		double rb = xy + zw;
		double rc = xz - yw;
		double rd = xy - zw;
		double re = 1.0 - x2 - z2;
		double rf = yz + xw;
		double rg = xz + yw;
		double rh = yz - xw;
		double ri = 1.0 - x2 - y2;

		this->m_right = Vector3(ra, rb, rc);
		this->m_up = Vector3(rd, re, rf);
		this->m_forward = Vector3(rg, rh, ri);

		m_directionDirty = false;
	}
}
