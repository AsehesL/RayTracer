#pragma once
#include "../../Core/Vector.h"
#include "../../Core/Color.h"
#include "../../Core/Ray.h"
#include "../RayTracingResult.h"

class Texture;

namespace RayTracer
{
	class SamplerBase;
	class PathTracer;
	class RTSkyShader;

	class RayTracerSkyLight
	{
	public:
		RayTracerSkyLight();
		virtual ~RayTracerSkyLight();

		Color PathTracing(PathTracer* pathTracer, SamplerBase* sampler, const Ray& ray);
		
	public:
		RTSkyShader* skyShader;
	};
}