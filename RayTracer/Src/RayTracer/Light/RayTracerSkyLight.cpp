#include "RayTracerSkyLight.h"
#include "../Shader/RayTracerSkyShader.h"
#include "../../Texture/Texture.h"

RayTracer::RayTracerSkyLight::RayTracerSkyLight()
{
	skyShader = nullptr;
}

RayTracer::RayTracerSkyLight::~RayTracerSkyLight()
{
}

Color RayTracer::RayTracerSkyLight::PathTracing(PathTracer* pathTracer, SamplerBase* sampler, const Ray& ray)
{
	if (skyShader)
	{
		return skyShader->PathTracing(pathTracer, sampler, ray);
	}
	return COLOR_DEFAULT_SKY;
}
