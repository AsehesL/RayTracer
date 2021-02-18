#pragma once
#include "../../Core/Vector.h"
#include "../../Core/Matrix.h"
#include "../../Core/Ray.h"
#include "../../Core/Bounds.h"
#include "../RayTracingResult.h"
#include <vector>

class Mesh;

namespace RayTracer
{
	class SceneDataBase;
	class RTMaterialShader;
	class SamplerBase;

	class PrimitiveBase
	{
	public:
		PrimitiveBase();
		virtual ~PrimitiveBase();

		virtual void AddToScene(SceneDataBase* sceneData) = 0;
		virtual bool Tracing(const Ray& ray, RayTracingResult& result) = 0;
		virtual void GetBounds(Bounds& bounds) = 0;
		float GetArea() const { return m_area; }
		virtual void RefreshArea() = 0;
		virtual Vector3 GetSurfacePosition(SamplerBase* sampler, Vector2 pos) = 0;
		virtual Vector3 GetSurfaceNormal(Vector3 pos) = 0;

	public:
		RTMaterialShader* material;

	protected:
		float m_area;
	};

	class CubePrimitive : public PrimitiveBase
	{
	public:
		CubePrimitive();
		virtual ~CubePrimitive() override;

		virtual void AddToScene(SceneDataBase* sceneData) override;
		virtual bool Tracing(const Ray& ray, RayTracingResult& result) override;
		virtual void GetBounds(Bounds& bounds) override;
		virtual void RefreshArea() override;
		virtual Vector3 GetSurfacePosition(SamplerBase* sampler, Vector2 pos) override;
		virtual Vector3 GetSurfaceNormal(Vector3 pos) override;

	public:
		Vector3 position;
		Vector3 size;
	};

	class SpherePrimitive : public PrimitiveBase
	{
	public:
		SpherePrimitive();
		virtual ~SpherePrimitive() override;

		virtual void AddToScene(SceneDataBase* sceneData) override;
		virtual bool Tracing(const Ray& ray, RayTracingResult& result) override;
		virtual void GetBounds(Bounds& bounds) override;
		virtual void RefreshArea() override;
		virtual Vector3 GetSurfacePosition(SamplerBase* sampler, Vector2 pos) override;
		virtual Vector3 GetSurfaceNormal(Vector3 pos) override;

	public:
		Vector3 position;
		double radius;
	};

	class MeshPrimitive : public PrimitiveBase
	{
	public:
		MeshPrimitive();
		virtual ~MeshPrimitive() override;

		virtual void AddToScene(SceneDataBase* sceneData) override;
		virtual bool Tracing(const Ray& ray, RayTracingResult& result) override;
		virtual void GetBounds(Bounds& bounds) override;
		virtual void RefreshArea() override {};
		virtual Vector3 GetSurfacePosition(SamplerBase* sampler, Vector2 pos) override;
		virtual Vector3 GetSurfaceNormal(Vector3 pos) override;

	public:
		Mesh* mesh;
		Matrix4x4 localToWorld;

	private:
		std::vector<class TrianglePrimitive*> m_triangles;
	};

	struct Vertex
	{
		public:
			Vector3 position;
			Vector3 normal;
			Vector4 tangent;
			Vector2 uv;
	};

	class TrianglePrimitive : public PrimitiveBase
	{
	public:
		TrianglePrimitive();
		virtual ~TrianglePrimitive() override;

		void SetVertex(const Vector3& position0, const Vector3& position1, const Vector3& position2);
		void SetNormal(const Vector3& normal0, const Vector3& normal1, const Vector3& normal2);
		void SetUV(const Vector2& uv0, const Vector2& uv1, const Vector2& uv2);
		void SetTangent(const Vector4& tangent0, const Vector4& tangent1, const Vector4& tangent2);

		virtual void AddToScene(SceneDataBase* sceneData) override;
		virtual bool Tracing(const Ray& ray, RayTracingResult& result) override;
		virtual void GetBounds(Bounds& bounds) override;
		virtual void RefreshArea() override;
		virtual Vector3 GetSurfacePosition(SamplerBase* sampler, Vector2 pos) override;
		virtual Vector3 GetSurfaceNormal(Vector3 pos) override;

	public:
		Vertex vertex0;
		Vertex vertex1;
		Vertex vertex2;
	};
};