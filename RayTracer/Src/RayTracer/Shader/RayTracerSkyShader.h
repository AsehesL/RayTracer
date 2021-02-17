#pragma once
#include "../../Core/Color.h"
#include "../../Core/Ray.h"
#include "../RayTracingResult.h"
#include "RayTracerShader.h"

class Texture;

namespace RayTracer
{
	class PathTracer;
	class SamplerBase;
	class RayTracerScene;

	class RTSkyShader : public RTShaderBase
	{
	public:
		RTSkyShader();
		virtual ~RTSkyShader() override;

		virtual Color PathTracing(PathTracer* pathTracer, SamplerBase* sampler, const Ray& ray) = 0;
	};

	class RTEnvironmentMapSkyShader : public RTSkyShader
	{
	public:
		RTEnvironmentMapSkyShader();
		virtual ~RTEnvironmentMapSkyShader() override;

		virtual Color PathTracing(PathTracer* pathTracer, SamplerBase* sampler, const Ray& ray) override;

	public:
		Color environmentColor;
		Texture* environmentMap;
	};
}