#include "PhotonMapperScene.h"
#include "../Primitive/RayTracerPrimitive.h"
#include "../Shader/RayTracerMaterialShader.h"

RayTracer::PhotonMapperScene::PhotonMapperScene()
{
}

RayTracer::PhotonMapperScene::~PhotonMapperScene()
{
}

void RayTracer::PhotonMapperScene::AddPrimitive(PrimitiveBase* primitive)
{
	if (primitive && primitive->material && primitive->material->IsEmissive())
	{
		primitive->RefreshArea();
		m_lights.push_back(primitive);
		return;
	}
	if (primitive != nullptr && m_sceneData != nullptr)
	{
		primitive->RefreshArea();
		primitive->AddToScene(m_sceneData);
	}
}

int RayTracer::PhotonMapperScene::GetLightCounts() const
{
	return m_lights.size();
}

RayTracer::PrimitiveBase* RayTracer::PhotonMapperScene::GetLight(int index) const
{
	if (index < 0 || index >= m_lights.size())
		return nullptr;
	return m_lights[index];
}
