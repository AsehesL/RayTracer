#pragma once
#include "../../Core/Color.h"
#include "../../Core/Ray.h"
#include "../RayTracingResult.h"

namespace RayTracer
{
	class PathTracer;
	class SamplerBase;
	class RayTracerScene;

	class RTShaderBase
	{
	public:
		RTShaderBase();
		virtual ~RTShaderBase();

	public:
		static Vector3 ImportanceSampleGGX(const Vector2& sample, float roughness);
		static Vector3 ImportanceGGXDirection(const Vector3& direction, SamplerBase* sampler, float roughness);

	protected:
		static bool TracingOnce(const Ray& ray, RayTracerScene* scene);
	};

	
}