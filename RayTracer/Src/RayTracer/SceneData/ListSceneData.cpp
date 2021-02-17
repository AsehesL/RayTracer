#include "ListSceneData.h"
#include "../Primitive/RayTracerPrimitive.h"

RayTracer::ListSceneData::ListSceneData()
{
}

RayTracer::ListSceneData::~ListSceneData()
{
}

bool RayTracer::ListSceneData::Tracing(const Ray& ray, RayTracingResult& result)
{
	if (m_primitives.size() == 0)
		return false;
	bool isHit = false;
	int primitiveCount = m_primitives.size();
	for (int i = 0; i < primitiveCount; i++)
	{
		PrimitiveBase* primitive = m_primitives[i];
		if (primitive && primitive->Tracing(ray, result))
		{
			isHit = true;
		}
	}

	return isHit;
}

void RayTracer::ListSceneData::BuildSceneData(const Bounds& bounds)
{
	m_primitives.clear();
	for (int i = 0; i < m_primitivesForBuild.size(); i++)
	{
		m_primitives.push_back(m_primitivesForBuild[i]);
	}
}
