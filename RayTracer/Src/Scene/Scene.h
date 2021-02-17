#pragma once
#include <vector>

namespace RayTracer 
{
	class RayTracerScene;
}

class Scene
{
public:
	Scene();
	~Scene();

	bool AddPrimitive(class PrimitiveBase* primitive);
	bool RemovePrimitive(class PrimitiveBase* primitive);
	void SetCamera(class Camera* camera)
	{
		m_camera = camera;
	}

	class Camera* GetCamera() const
	{
		return m_camera;
	}
	void SetSkyLight(class SkyLight* skyLight)
	{
		m_skyLight = skyLight;
	}
	class SkyLight* GetSkyLight() const
	{
		return m_skyLight;
	}
	void SetSunLight(class SunLight* sunLight)
	{
		m_sunLight = sunLight;
	}
	class SunLight* GetSunLight() const
	{
		return m_sunLight;
	}

	RayTracer::RayTracerScene* BuildRayTracerScene();
	void PrepareForColorPass(int& opaqueCount, int& transparencyCount);
	void RenderOpaqueScene();
	void RenderTransparencyScene(class RenderTexture* opaqueSceneTexture);
	void RenderGizmos(class GizmosRenderer* gizmosRenderer);
	void RenderShadowScene();

private:
	std::vector<class PrimitiveBase*> m_primitives;
	std::vector<class PrimitiveBase*> m_opaquePrimitives;
	std::vector<class PrimitiveBase*> m_transparencyPrimitives;
	class Camera* m_camera;
	class SkyLight* m_skyLight;
	class SunLight* m_sunLight;
};