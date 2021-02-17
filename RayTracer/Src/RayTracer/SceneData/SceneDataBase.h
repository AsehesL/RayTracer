#pragma once
#include "../RayTracingResult.h"
#include "../../Core/Ray.h"
#include "../../Core/Bounds.h"
#include <vector>

namespace RayTracer
{
	class PrimitiveBase;

	class SceneDataBase
	{
	public:
		SceneDataBase();
		virtual ~SceneDataBase();

		virtual bool Tracing(const Ray& ray, RayTracingResult& result) = 0;
		void Build();
		void AddPrimitive(PrimitiveBase* primitive);

	protected:
		virtual void BuildSceneData(const Bounds& bounds) = 0;

	protected:
		std::vector<PrimitiveBase*> m_primitivesForBuild; 

	private:
		Vector3 m_min;
		Vector3 m_max;
	};
};