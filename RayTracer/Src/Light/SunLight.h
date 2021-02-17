#pragma once
#include "../Core/Matrix.h"
#include "../Core/Color.h"

class SunLight
{
public:
	SunLight(class GLContext* glContext);
	~SunLight();

	void RefreshSunLightMatrix(class Camera* camera);

	class RenderTexture* GetShadowMap();

	Vector3 GetEuler() const
	{
		return m_euler;
	}

	void SetEuler(const Vector3& euler)
	{
		m_euler = euler;
		m_directionDirty = true;
	}

	Vector3 GetForward()
	{
		RefreshDirection();
		return m_forward;
	}

	Vector3 GetUp()
	{
		RefreshDirection();
		return m_up;
	}

	Vector3 GetRight()
	{
		RefreshDirection();
		return m_right;
	}

	void GetLightSpaceMatrix(Matrix4x4& lightSpaceMatrix)
	{
		lightSpaceMatrix = m_lightSpaceMatrix;
	}

	void GetShadowProjectionMatrix(Matrix4x4& shadowProjectionMatrix)
	{
		shadowProjectionMatrix = m_shadowProjectionMatrix;
	}

private:
	void RefreshDirection();

public:
	Color sunColor;
	float sunIntensity;
	float shadowBias;

private:
	Vector3 m_euler;
	bool m_directionDirty;
	Vector3 m_right;
	Vector3 m_up;
	Vector3 m_forward;
	Matrix4x4 m_lightSpaceMatrix;
	Matrix4x4 m_shadowProjectionMatrix;
	class GLContext* m_glContext;
	class RenderTexture* m_shadowMap;
};