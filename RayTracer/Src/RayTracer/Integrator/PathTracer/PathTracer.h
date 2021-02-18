#pragma once
#include "../IntegratorBase.h"
#include "../../../Core/ConcurrentQueue.hpp"
#include "../../Render/RayTracerRenderTile.h"
#include "../../../Core/Color.h"
#include "../../../Core/Ray.h"
#include <vector>

class Camera;
class RayTracingPreviewRenderer;

namespace RayTracer
{ 
	class RenderTarget;
	class PathTracerScene;
	class SamplerBase;

	class PathTracer : public IntegratorBase
	{
	public:
		PathTracer(RayTracingPreviewRenderer* previewRenderer);
		virtual ~PathTracer() override;

		virtual void Render(RayTracerScene* scene, RenderTarget* renderTarget) override;
		virtual RayTracerScene* BuildScene(class Scene* scene) override;

		void Tracing(const Ray& ray, SamplerBase* sampler, PathTracerScene* scene, int depth, Color& outColor);

	private:
		void ProcessPathTracing(ConcurrentQueue<RenderTile>& tiles, RenderTarget* renderTarget, Camera* camera, SamplerBase* sampler, PathTracerScene* scene);
	
		void UpdateRenderTarget(ConcurrentQueue<RenderTile>& tiles, RenderTarget* renderTarget);

	public:
		int sampleNums;
		int bounce;

	private:
		std::vector<class SamplerBase*> m_samplers;
	};
};