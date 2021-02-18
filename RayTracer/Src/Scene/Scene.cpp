#include "Scene.h"
#include "../Camera/Camera.h"
#include "../Light/SkyLight.h"
#include "../Light/SunLight.h"
#include "../Primitive/Primitive.h"
#include "../Shader/MaterialShader.h"
#include "../RayTracer/Scene/PathTracerScene.h"
#include "../RayTracer/Scene/PhotonMapperScene.h"
#include "../RayTracer/Light/RayTracerSkyLight.h"
#include "../RayTracer/Light/RayTracerSunLight.h"
#include "../Texture/RenderTexture.h"
#include "../RealtimeRender/GraphicsLib/RenderBuffer.h"
#include "../Gizmos/GizmosRenderer.h"

Scene::Scene()
{
	m_camera = nullptr;
	m_skyLight = nullptr;
	m_sunLight = nullptr;
}

Scene::~Scene()
{
	m_primitives.clear();
}

bool Scene::AddPrimitive(PrimitiveBase* primitive)
{
	if (!primitive)
		return -1;
	int primitiveCount = m_primitives.size();
	for (int i = 0; i < primitiveCount; ++i)
	{
		PrimitiveBase* findprimitive = m_primitives[i];
		if (findprimitive && findprimitive == primitive)
			return false;
	}
	m_primitives.push_back(primitive);
	return true;
}

bool Scene::RemovePrimitive(PrimitiveBase* primitive)
{
	if (!primitive)
		return false;
	std::vector<PrimitiveBase*>::iterator itr = m_primitives.begin();
	while (itr != m_primitives.end())
	{
		if (*itr == primitive)
		{
			m_primitives.erase(itr);
			return true;
		}
		itr++;
	}
	return false;
}

PrimitiveBase* Scene::GetPrimitiveByScreenPos(int screenPosX, int screenPosY)
{
	if (m_camera == nullptr)
		return nullptr;
	if (m_primitives.size() == 0)
		return nullptr;
	Ray ray;
	m_camera->GetRayFromPixel(screenPosX, screenPosY, ray);
	double distance = DBL_MAX;
	PrimitiveBase* hitprimitive = nullptr;
	for (int i = 0; i < m_primitives.size(); i++)
	{
		PrimitiveBase* primitive = m_primitives[i];
		Bounds bounds;
		primitive->GetBounds(bounds);
		double dis;
		if (bounds.Raycast(ray, dis))
		{
			if (dis < distance)
			{
				distance = dis;
				hitprimitive = primitive;
			}
		}
	}
	return hitprimitive;
}

//RayTracer::RayTracerScene* Scene::BuildRayTracerScene()
//{
//	RayTracer::RayTracerScene* RTScene = new RayTracer::RayTracerScene();
//	RTScene->camera = m_camera;
//	RayTracer::RTSkyShader* rtSkyShader = m_skyLight ? m_skyLight->GetRayTracerSkyLightShader() : nullptr;
//	if (m_sunLight != nullptr)
//	{
//		RayTracer::RayTracerSunLight* rtSunLight = RTScene->GetSunLight();
//		rtSunLight->isActive = true;
//		rtSunLight->SetDirection(m_sunLight->GetForward());
//		rtSunLight->SetColor(m_sunLight->sunColor * m_sunLight->sunIntensity);
//	}
//	else
//	{
//		RayTracer::RayTracerSunLight* rtSunLight = RTScene->GetSunLight();
//		rtSunLight->isActive = false;
//	}
//	RayTracer::RayTracerSkyLight* rtSkyLight = RTScene->GetSkyLight();
//	rtSkyLight->skyShader = rtSkyShader;
//	//RTScene->sunLight = m_sunLight ? m_sunLight->GetRayTracerSunLight();
//	
//	int primitiveCount = m_primitives.size();
//	for (int i = 0; i < primitiveCount; ++i)
//	{
//		PrimitiveBase* primitive = m_primitives[i];
//		if (primitive)
//		{
//			RayTracer::PrimitiveBase* RTPrimitive = primitive->GetRayTracerPrimitive();
//			RTScene->AddPrimitive(RTPrimitive);
//		}
//	}
//	RTScene->Build();
//	return RTScene;
//}

RayTracer::RayTracerScene* Scene::BuildPathTracingScene()
{
	RayTracer::PathTracerScene* RTScene = new RayTracer::PathTracerScene();
	RTScene->camera = m_camera;
	RayTracer::RTSkyShader* rtSkyShader = m_skyLight ? m_skyLight->GetRayTracerSkyLightShader() : nullptr;
	if (m_sunLight != nullptr)
	{
		RayTracer::RayTracerSunLight* rtSunLight = RTScene->GetSunLight();
		rtSunLight->isActive = true;
		rtSunLight->SetDirection(m_sunLight->GetForward());
		rtSunLight->SetColor(m_sunLight->sunColor * m_sunLight->sunIntensity);
	}
	else
	{
		RayTracer::RayTracerSunLight* rtSunLight = RTScene->GetSunLight();
		rtSunLight->isActive = false;
	}
	RayTracer::RayTracerSkyLight* rtSkyLight = RTScene->GetSkyLight();
	rtSkyLight->skyShader = rtSkyShader;
	//RTScene->sunLight = m_sunLight ? m_sunLight->GetRayTracerSunLight();

	int primitiveCount = m_primitives.size();
	for (int i = 0; i < primitiveCount; ++i)
	{
		PrimitiveBase* primitive = m_primitives[i];
		if (primitive)
		{
			RayTracer::PrimitiveBase* RTPrimitive = primitive->GetRayTracerPrimitive();
			RTScene->AddPrimitive(RTPrimitive);
		}
	}
	RTScene->Build();
	return RTScene;
}

RayTracer::RayTracerScene* Scene::BuildPhotonMapperScene()
{
	RayTracer::PhotonMapperScene* RTScene = new RayTracer::PhotonMapperScene();
	RTScene->camera = m_camera;
	int primitiveCount = m_primitives.size();
	for (int i = 0; i < primitiveCount; ++i)
	{
		PrimitiveBase* primitive = m_primitives[i];
		if (primitive)
		{
			RayTracer::PrimitiveBase* RTPrimitive = primitive->GetRayTracerPrimitive();
			RTScene->AddPrimitive(RTPrimitive);
		}
	}
	RTScene->Build();
	return RTScene;
}

void Scene::PrepareForColorPass(int& opaqueCount, int& transparencyCount)
{
	if (m_camera == nullptr)
	{
		opaqueCount = 0;
		transparencyCount = 0;
		return;
	}
	int primitiveCount = m_primitives.size();
	Matrix4x4 viewMatrix = m_camera ? m_camera->GetWorldToViewMatrix() : Matrix4x4();
	Matrix4x4 projectionMatrix = m_camera ? m_camera->GetProjectionMatrix() : Matrix4x4();
	m_opaquePrimitives.clear();
	m_transparencyPrimitives.clear();
	for (int i = 0; i < primitiveCount; ++i)
	{
		PrimitiveBase* primitive = m_primitives[i];
		if (primitive && !primitive->Culled(viewMatrix, projectionMatrix))
		{
			if (primitive->material)
			{
				if (primitive->material->IsTransparency())
					m_transparencyPrimitives.push_back(primitive);
				else
					m_opaquePrimitives.push_back(primitive);
			}
			else
				m_opaquePrimitives.push_back(primitive);
		}
	}
	opaqueCount = m_opaquePrimitives.size();
	transparencyCount = m_transparencyPrimitives.size();
}

void Scene::RenderOpaqueScene()
{
	if (!m_camera)
		return;
	Matrix4x4 viewMatrix = m_camera->GetWorldToViewMatrix();
	Matrix4x4 projectionMatrix = m_camera->GetProjectionMatrix();
	Vector3 cameraPos = m_camera->GetPosition();

	CubeMapRenderTexture* cubeMap = nullptr;
	if (m_skyLight != nullptr)
	{
		cubeMap = m_skyLight->GetCubeMap();
	}
	RenderTexture* shadowMap = nullptr;
	Matrix4x4 lightSpaceMatrix;
	Matrix4x4 shadowProjectionMatrix;
	Vector3 sunLightDirection;
	Color sunLightColor;
	if (m_sunLight != nullptr)
	{
		shadowMap = m_sunLight->GetShadowMap();
		sunLightDirection = m_sunLight->GetForward() * -1.0;
		sunLightColor = m_sunLight->sunColor * m_sunLight->sunIntensity;
		m_sunLight->GetLightSpaceMatrix(lightSpaceMatrix);
		m_sunLight->GetShadowProjectionMatrix(shadowProjectionMatrix);
	}

	// render opaque objects
	for (int i = 0; i < m_opaquePrimitives.size(); i++)
	{
		PrimitiveBase* primitive = m_opaquePrimitives[i];
		if (primitive->material)
		{
			primitive->material->SetSkyLightCubeMap(cubeMap);
			primitive->material->SetCameraInformation(cameraPos);
			primitive->material->SetSunLightInformation(sunLightDirection, sunLightColor);
			primitive->material->SetShadowParameters(shadowMap, lightSpaceMatrix, shadowProjectionMatrix);
		}
		primitive->Render(viewMatrix, projectionMatrix);
	}

	// render sky light
	if (m_skyLight != nullptr)
	{
		Vector3 camPos = m_camera->GetPosition();
		m_skyLight->Render(camPos, viewMatrix, projectionMatrix);
	}
}

void Scene::RenderTransparencyScene(RenderTexture* opaqueSceneTexture)
{
	if (m_camera == nullptr)
		return;
	Matrix4x4 viewMatrix = m_camera->GetWorldToViewMatrix();
	Matrix4x4 projectionMatrix = m_camera->GetProjectionMatrix();
	Vector3 cameraPos = m_camera->GetPosition();
	CubeMapRenderTexture* cubeMap = nullptr;
	Vector3 sunLightDirection;
	Color sunLightColor;
	if (m_skyLight != nullptr)
	{
		cubeMap = m_skyLight->GetCubeMap();
		sunLightDirection = m_sunLight->GetForward() * -1.0;
		sunLightColor = m_sunLight->sunColor * m_sunLight->sunIntensity;
	}
	// render transparency objects
	for (int i = 0; i < m_transparencyPrimitives.size(); i++)
	{
		PrimitiveBase* primitive = m_transparencyPrimitives[i];
		if (primitive->material)
		{
			primitive->material->SetSkyLightCubeMap(cubeMap);
			primitive->material->SetCameraInformation(cameraPos);
			primitive->material->SetSunLightInformation(sunLightDirection, sunLightColor);
			primitive->material->SetScreenCapture(opaqueSceneTexture);
		}
		primitive->Render(viewMatrix, projectionMatrix);
	}
}

void Scene::RenderShadowScene()
{
	if (m_sunLight == nullptr || m_camera == nullptr)
		return;

	Matrix4x4 lightSpaceMatrix;
	Matrix4x4 shadowProjectionMatrix;
	m_sunLight->RefreshSunLightMatrix(m_camera);
	m_sunLight->GetLightSpaceMatrix(lightSpaceMatrix);
	m_sunLight->GetShadowProjectionMatrix(shadowProjectionMatrix);
	float bias = m_sunLight->shadowBias;

	int primitiveCount = m_primitives.size();
	for (int i = 0; i < primitiveCount; ++i)
	{
		PrimitiveBase* primitive = m_primitives[i];
		if (primitive && !primitive->Culled(lightSpaceMatrix, shadowProjectionMatrix))
		{
			if (primitive->material && !primitive->material->IsTransparency())
			{
				primitive->material->SetShadowBias(bias);
				primitive->RenderShadow(lightSpaceMatrix, shadowProjectionMatrix);
			}
		}
	}
	/*if (m_skyLight != nullptr)
	{
		Vector3 camPos = m_camera ? m_camera->GetPosition() : Vector3(0, 0, 0);
		m_skyLight->RenderShadow(camPos, lightSpaceMatrix, shadowProjectionMatrix);
	}*/
}

void Scene::RenderGizmos(GizmosRenderer* gizmosRenderer)
{
	if (gizmosRenderer && m_camera)
	{
		Matrix4x4 viewMatrix = m_camera->GetWorldToViewMatrix();
		Matrix4x4 projectionMatrix = m_camera->GetProjectionMatrix();

		gizmosRenderer->SetColor(Color(0.2f,0.2f,0.2f,1.0f));
		gizmosRenderer->SetWorldToViewMatrix(viewMatrix);
		gizmosRenderer->SetProjectionMatrix(projectionMatrix);
		for (int i = -5; i <= 5; i++)
		{
			Vector3 begin = Vector3(i, 0, -5);
			Vector3 end = Vector3(i, 0, 5);

			gizmosRenderer->DrawWorldSpaceLine(begin, end);
		}
		for (int i = -5; i <= 5; i++)
		{
			Vector3 begin = Vector3(-5, 0, i);
			Vector3 end = Vector3(5, 0, i);

			gizmosRenderer->DrawWorldSpaceLine(begin, end);
		}
		int primitiveCount = m_primitives.size();
		for (int i = 0; i < primitiveCount; ++i)
		{
			PrimitiveBase* primitive = m_primitives[i];
			if (primitive)
			{
				primitive->RenderGizmos(gizmosRenderer);
			}
		}
	}
}
