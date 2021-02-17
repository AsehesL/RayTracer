#include "RayTracerSkyShader.h"
#include "../../Texture/Texture.h"

RayTracer::RTSkyShader::RTSkyShader()
{
}

RayTracer::RTSkyShader::~RTSkyShader()
{
}

RayTracer::RTEnvironmentMapSkyShader::RTEnvironmentMapSkyShader()
{
	environmentMap = nullptr;
}

RayTracer::RTEnvironmentMapSkyShader::~RTEnvironmentMapSkyShader()
{
}

Color RayTracer::RTEnvironmentMapSkyShader::PathTracing(PathTracer* pathTracer, SamplerBase* sampler, const Ray& ray)
{
	if (environmentMap == nullptr)
		return COLOR_BLACK;
	float fi = (float)atan2(ray.direction.x, ray.direction.z);
	float u = fi * 0.5f * (float)Math::InvPI;
	float theta = (float)acos(ray.direction.y);

	float v = 1.0f - theta * (float)Math::InvPI;

	Color resultColor;
	environmentMap->Sample(u, v, resultColor);
	return resultColor * environmentColor;
}
