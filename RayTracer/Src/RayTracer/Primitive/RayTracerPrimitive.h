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

	class PrimitiveBase
	{
	public:
		PrimitiveBase();
		virtual ~PrimitiveBase();

		virtual void AddToScene(SceneDataBase* sceneData) = 0;
		virtual bool Tracing(const Ray& ray, RayTracingResult& result) = 0;
		virtual void GetBounds(Bounds& bounds) = 0;

	public:
		RTMaterialShader* material;
	};

	class CubePrimitive : public PrimitiveBase
	{
	public:
		CubePrimitive();
		virtual ~CubePrimitive() override;

		virtual void AddToScene(SceneDataBase* sceneData) override;
		virtual bool Tracing(const Ray& ray, RayTracingResult& result) override;
		virtual void GetBounds(Bounds& bounds) override;

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

		void SetVertex(int index, Vector3 position, Vector3 normal, Vector2 uv, Vector4 tangent);

		virtual void AddToScene(SceneDataBase* sceneData) override;
		virtual bool Tracing(const Ray& ray, RayTracingResult& result) override;
		virtual void GetBounds(Bounds& bounds) override;

	public:
		Vertex vertex0;
		Vertex vertex1;
		Vertex vertex2;
	};
};