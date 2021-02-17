#include "SceneDataBase.h"
#include "../Primitive/RayTracerPrimitive.h"

RayTracer::SceneDataBase::SceneDataBase()
{
	m_max = Vector3(DBL_MIN, DBL_MIN, DBL_MIN);
	m_min = Vector3(DBL_MAX, DBL_MAX, DBL_MAX);
}

RayTracer::SceneDataBase::~SceneDataBase()
{
}

void RayTracer::SceneDataBase::Build()
{
	Vector3 center = (m_min + m_max) * 0.5;
	Vector3 size = m_max - m_min;
	size *= 1.001;
	Bounds bounds = Bounds(center, size);

	BuildSceneData(bounds);

	m_primitivesForBuild.clear();
	m_max = Vector3(DBL_MIN, DBL_MIN, DBL_MIN);
	m_min = Vector3(DBL_MAX, DBL_MAX, DBL_MAX);
}

void RayTracer::SceneDataBase::AddPrimitive(PrimitiveBase* primitive)
{
	if (!primitive)
		return;
	Bounds primitiveBounds;
	primitive->GetBounds(primitiveBounds);
	Vector3 boundsMin = primitiveBounds.GetMin();
	Vector3 boundsMax = primitiveBounds.GetMax();

	m_max = Vector3::Max(m_max, boundsMax);
	m_min = Vector3::Min(m_min, boundsMin); 

	m_primitivesForBuild.push_back(primitive);
}
