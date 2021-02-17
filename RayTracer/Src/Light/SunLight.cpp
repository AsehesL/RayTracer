#include "SunLight.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../Camera/Camera.h"
#include "../Texture/RenderTexture.h"
#include "../RayTracer/Light/RayTracerSunLight.h"

SunLight::SunLight(GLContext* glContext)
{
	m_glContext = glContext;
	m_directionDirty = true;
	m_shadowMap = nullptr;
	sunIntensity = 3.14f;
	shadowBias = 0.01f;
}

SunLight::~SunLight()
{
	if (m_shadowMap)
		delete m_shadowMap;
	m_shadowMap = nullptr;
}

void SunLight::RefreshSunLightMatrix(Camera* camera)
{
	double csm0 = 0.0f;
	double csm1 = 0.01f;

	double camNear = camera->GetNear();
	double camFar = camera->GetFar();
	double fov = camera->GetFOV();
	float aspect = camera->GetAspect();

	double tnear = Math::Lerp(camNear, camFar, csm0);
	double tfar = Math::Lerp(camNear, camFar, csm1);

	double tanfov = tan(Math::DegToRad * fov * 0.5f);
	double tnearh = tnear * tanfov;
	double tfarh = tfar * tanfov;
	double tnearw = aspect * tnearh;
	double tfarw = aspect * tfarh;

	double z = tfarh * tfarh + tfarw * tfarw - tnearh * tnearh - tnearw * tnearw + tfar * tfar - tnear * tnear;
	z /= 2.0 * (tfar - tnear);


	double radius = (z - tnear) * (z - tnear) + tnearh * tnearh + tnearw * tnearw;
	radius = sqrt(radius);

	double z2 = tfar;
	double radius2 = sqrt(tfarw * tfarw + tfarh * tfarh);
	if (z > tfar)
	{
		radius = radius2;
		z = z2;
	}

	Matrix4x4 matrix, projMatrix;
	Vector3 lightx, lighty, lightz, eye, camPos, camZ;
	camPos = camera->GetPosition();
	camZ = camera->GetForward();
	eye = camPos + camZ * z;
	lightx = this->GetRight();
	lighty = this->GetUp();
	lightz = this->GetForward();

	matrix.m00 = lightx.x;
	matrix.m01 = lightx.y;
	matrix.m02 = lightx.z;

	matrix.m10 = lighty.x;
	matrix.m11 = lighty.y;
	matrix.m12 = lighty.z;

	matrix.m20 = lightz.x;
	matrix.m21 = lightz.y;
	matrix.m22 = lightz.z;

	matrix.m03 = -Vector3::Dot(lightx, eye);
	matrix.m13 = -Vector3::Dot(lighty, eye);
	matrix.m23 = -Vector3::Dot(lightz, eye);

	matrix.m30 = 0.0;
	matrix.m31 = 0.0;
	matrix.m32 = 0.0;
	matrix.m33 = 1.0;

	Matrix4x4::Ortho(&projMatrix, radius * 2.0, radius * 2.0, -radius, radius);

	m_lightSpaceMatrix = matrix;
	m_shadowProjectionMatrix = projMatrix;
}

RenderTexture* SunLight::GetShadowMap()
{
	if (m_shadowMap == nullptr)
	{ 
		m_shadowMap = new RenderTexture(1024,1024,true,m_glContext);
		m_shadowMap->SetWrapMode(WrapMode::Clamp);
	}
	return m_shadowMap;
}

void SunLight::RefreshDirection()
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
