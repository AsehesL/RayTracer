#include "RayTracerSunLight.h"
#include "../Sampler/Sampler.h"
#include "../Shader/RayTracerMaterialShader.h"

#define SUN_ROUGHNESS 0.12f

RayTracer::RayTracerSunLight::RayTracerSunLight()
{
	isActive = false;
}

RayTracer::RayTracerSunLight::~RayTracerSunLight()
{
}

void RayTracer::RayTracerSunLight::SetDirection(const Vector3& direction)
{
	m_sunDirection = direction;
}

Vector3 RayTracer::RayTracerSunLight::GetDirection(SamplerBase* sampler)
{
	if (sampler == nullptr)
		return m_sunDirection;
	return RTMaterialShader::ImportanceGGXDirection(m_sunDirection, sampler, SUN_ROUGHNESS);
}

void RayTracer::RayTracerSunLight::SetColor(const Color& color)
{
	m_sunColor = color;
}

Color RayTracer::RayTracerSunLight::GetColor() const
{
	return m_sunColor;
}
