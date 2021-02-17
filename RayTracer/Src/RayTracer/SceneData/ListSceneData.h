#pragma once
#include "SceneDataBase.h"
#include <vector>

namespace RayTracer
{
	class PrimitiveBase;

	class ListSceneData : public SceneDataBase
	{
	public:
		ListSceneData();
		virtual ~ListSceneData() override;

		virtual bool Tracing(const Ray& ray, RayTracingResult& result) override;

	protected:
		virtual void BuildSceneData(const Bounds& bounds) override;

	private:
		std::vector<class PrimitiveBase*> m_primitives;
	};
};