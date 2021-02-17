#pragma once
#include "../Sampler/Sampler.h"

class RayTracingPreviewRenderer;

namespace RayTracer
{
	class RenderTarget;

	class IntegratorBase
	{
	public:
		IntegratorBase(RayTracingPreviewRenderer* previewRenderer);
		virtual ~IntegratorBase() {};

		virtual void Render(class RayTracerScene* scene, RenderTarget* renderTarget) = 0;

	public:
		SamplerType samplerType;

	protected:
		unsigned int m_threadCount;
		RayTracingPreviewRenderer* m_previewRenderer;
	};
};