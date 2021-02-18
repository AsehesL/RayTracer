#include "RayTracerScene.h"
#include "../SceneData/BVH.h"

RayTracer::RayTracerScene::RayTracerScene()
{
	camera = nullptr;
	m_sceneData = new RayTracer::BVH();
}

RayTracer::RayTracerScene::~RayTracerScene()
{
	delete m_sceneData;
}

void RayTracer::RayTracerScene::Build()
{
	m_sceneData->Build();
}

bool RayTracer::RayTracerScene::Tracing(const Ray& ray, RayTracingResult& result)
{
	return m_sceneData->Tracing(ray, result);
}
