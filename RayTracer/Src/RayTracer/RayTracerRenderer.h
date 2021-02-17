#pragma once
#include <vector>

class Scene;

namespace RayTracer
{
	class RayTracerScene;
	class IntegratorBase;
	class RenderTarget;

	class RayTracerRenderer
	{
	public:
		RayTracerRenderer(Scene* scene);
		~RayTracerRenderer();

		bool StartRayTracing(IntegratorBase* integrator);

		//bool IsRayTracingFinish();
		//void UpdateFrame(int windowWidth, int windowHeight, int renderWidth, int renderHeight);

	//private:
	//	void UpdateRenderTarget(RenderTarget* renderTarget);
		RenderTarget* GetRenderTarget() const
		{
			return m_renderTarget;
		}

	private:
		RenderTarget* m_renderTarget;
		IntegratorBase* m_integrator;
		Scene* m_scene;
		RayTracerScene* m_rayTracerScene;
		//bool m_isRayTracing;
		//std::vector<RayTracingTile> m_tiles;
		//int m_currentTileIndex;
	};
};