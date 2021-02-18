#pragma once
#include "../Core/Matrix.h"
#include "../Core/Ray.h"

namespace RayTracer
{
	class SamplerBase;
}

class Camera
{
public:
	Camera(unsigned int width, unsigned int height);
	~Camera();

	Matrix4x4 GetWorldToViewMatrix();
	Matrix4x4 GetProjectionMatrix();

	unsigned int GetRenderWidth() const { return m_width; }
	unsigned int GetRenderHeight() const { return m_height; }
	void SetRenderWidth(unsigned int width) 
	{ 
		m_width = width; 
		m_projectionMatrixDirty = true;
	}
	void SetRenderHeight(unsigned int height) 
	{ 
		m_height = height; 
		m_projectionMatrixDirty = true;
	}

	void ForceRefreshTransform();

	void GetRayFromPixel(int pixelX, int pixelY, RayTracer::SamplerBase* sampler, Ray& outRay);

	void GetRayFromPixel(int pixelX, int pixelY, Ray& outRay);

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

	Vector3 GetPosition() const
	{
		return m_position;
	}

	void SetPosition(const Vector3& position)
	{
		m_position = position;
		m_viewMatrixDirty = true;
	}

	Vector3 GetEuler() const
	{
		return m_euler;
	}

	void SetEuler(const Vector3& euler)
	{
		m_euler = euler;
		m_directionDirty = true;
		m_viewMatrixDirty = true;
	}

	double GetFOV() const { return m_fieldOfView; }

	void SetFOV(double fov)
	{
		m_fieldOfView = fov;
		m_projectionMatrixDirty = true;
	}

	double GetNear() { return m_nearClipPlane; }
	double GetFar() { return 1000.0; }
	float GetAspect() 
	{
		return float(m_width) / float(m_height);
	}

private:
	void RefreshDirection();

public:
	float radius;
	float focal;
	bool useThinLens;

private:
	Vector3 m_position;
	Vector3 m_euler;
	Vector3 m_right;
	Vector3 m_up;
	Vector3 m_forward;
	double m_fieldOfView;
	double m_nearClipPlane;

	unsigned int m_width;
	unsigned int m_height;

	bool m_viewMatrixDirty;
	bool m_directionDirty;
	bool m_projectionMatrixDirty;

	Matrix4x4 m_viewMatrix;
	Matrix4x4 m_projectionMatrix;

	float m_halfScreenHeight;
	float m_halfScreenWidth;
};