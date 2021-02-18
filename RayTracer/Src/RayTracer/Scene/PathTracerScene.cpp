#include "PathTracerScene.h"
#include "../Primitive/RayTracerPrimitive.h"
#include "../Light/RayTracerSkyLight.h"
#include "../Light/RayTracerSunLight.h"

RayTracer::PathTracerScene::PathTracerScene() : RayTracer::RayTracerScene()
{
	m_skyLight = new RayTracer::RayTracerSkyLight();
	m_sunLight = new RayTracer::RayTracerSunLight();
}

RayTracer::PathTracerScene::~PathTracerScene()
{
	delete m_skyLight;
	delete m_sunLight;
}

void RayTracer::PathTracerScene::AddPrimitive(PrimitiveBase* primitive)
{
	if (primitive != nullptr && m_sceneData != nullptr)
	{
		primitive->RefreshArea();
		primitive->AddToScene(m_sceneData);
	}
}

RayTracer::RayTracerSkyLight* RayTracer::PathTracerScene::GetSkyLight() const
{
	return m_skyLight;
}

RayTracer::RayTracerSunLight* RayTracer::PathTracerScene::GetSunLight() const
{
	return m_sunLight;
}
