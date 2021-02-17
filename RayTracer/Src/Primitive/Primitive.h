#pragma once
#include "../Core/Vector.h"
#include "../Core/Matrix.h"
#include "../Core/Bounds.h"

namespace RayTracer
{
	class PrimitiveBase;
}

class PrimitiveBase 
{
public:
	PrimitiveBase(class GLContext* glContext);
	virtual ~PrimitiveBase();

	void Render(const Matrix4x4& worldToViewMatrix, const Matrix4x4& projectionMatrix);
	bool Culled(const Matrix4x4& worldToViewMatrix, const Matrix4x4& projectionMatrix);
	void RenderShadow(const Matrix4x4& lightSpaceMatrix, const Matrix4x4& shadowProjectionMatrix);
	void RenderGizmos(class GizmosRenderer* gizmosRenderer);
	RayTracer::PrimitiveBase* GetRayTracerPrimitive();
	virtual void GetBounds(Bounds& OutBounds) = 0;
	Bounds GetBounds();

protected:
	virtual void GetRenderMatrix(Matrix4x4& renderMatrix) = 0;
	virtual void UpdateRayTracerPrimitive() = 0;

public:
	class MaterialShader* material;

public:
	Vector3 position;
	Vector3 euler;
	Vector3 scale;
	bool displayBounds;

protected:
	class GLContext* m_glContext;
	RayTracer::PrimitiveBase* m_rayTracerPrimitive;
	class Mesh* m_realTimeRenderMesh;
};

class CubePrimitive : public PrimitiveBase
{
public:
	CubePrimitive(class GLContext* glContext);
	virtual ~CubePrimitive() override;
	virtual void GetBounds(Bounds& OutBounds) override;

protected:
	virtual void GetRenderMatrix(Matrix4x4& renderMatrix) override;
	virtual void UpdateRayTracerPrimitive() override;
};

class SpherePrimitive : public PrimitiveBase
{
public:
	SpherePrimitive(class GLContext* glContext);
	virtual ~SpherePrimitive() override;
	virtual void GetBounds(Bounds& OutBounds) override;

protected:
	virtual void GetRenderMatrix(Matrix4x4& renderMatrix) override;
	virtual void UpdateRayTracerPrimitive() override;
};

class PlanePrimitive : public PrimitiveBase
{
public:
	PlanePrimitive(class GLContext* glContext);
	virtual ~PlanePrimitive() override;
	virtual void GetBounds(Bounds& OutBounds) override;

protected:
	virtual void GetRenderMatrix(Matrix4x4& renderMatrix) override;
	virtual void UpdateRayTracerPrimitive() override;
};

class MeshPrimitive : public PrimitiveBase
{
public:
	MeshPrimitive(class GLContext* glContext);
	virtual ~MeshPrimitive() override;
	void SetMesh(class Mesh* mesh);
	virtual void GetBounds(Bounds& OutBounds) override;

protected:
	virtual void GetRenderMatrix(Matrix4x4& renderMatrix) override;
	virtual void UpdateRayTracerPrimitive() override;
};