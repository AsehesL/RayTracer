#include "RayTracerScene.h"
#include "SceneData/BVH.h"
#include "Primitive/RayTracerPrimitive.h"
#include "Light/RayTracerSkyLight.h"
#include "Light/RayTracerSunLight.h"

RayTracer::RayTracerScene::RayTracerScene()
{
	camera = nullptr;
	m_sceneData = new RayTracer::BVH();
	m_skyLight = new RayTracer::RayTracerSkyLight();
	m_sunLight = new RayTracer::RayTracerSunLight();
}

RayTracer::RayTracerScene::~RayTracerScene()
{
	delete m_sceneData;
	delete m_skyLight;
	delete m_sunLight;
}

void RayTracer::RayTracerScene::AddPrimitive(PrimitiveBase* primitive)
{
	if (primitive != nullptr && m_sceneData != nullptr)
	{
		primitive->AddToScene(m_sceneData);
	}
}

void RayTracer::RayTracerScene::Build()
{
	m_sceneData->Build();
}

RayTracer::RayTracerSkyLight* RayTracer::RayTracerScene::GetSkyLight() const
{
	return m_skyLight;
}

RayTracer::RayTracerSunLight* RayTracer::RayTracerScene::GetSunLight() const
{
	return m_sunLight;
}

bool RayTracer::RayTracerScene::Tracing(const Ray& ray, RayTracingResult& result)
{
	return m_sceneData->Tracing(ray, result);
}
