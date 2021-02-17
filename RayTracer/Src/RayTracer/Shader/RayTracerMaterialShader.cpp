#include "RayTracerMaterialShader.h"

RayTracer::RTMaterialShader::RTMaterialShader()
{
}

RayTracer::RTMaterialShader::~RTMaterialShader()
{
}

void RayTracer::RTMaterialShader::RecalucateNormal(const Vector3& worldNormal, const Vector4& worldTangent, const Color& tangentSpaceNormal, Vector3& outNormal)
{
	Vector3 wbnormal = Vector3::Cross(worldNormal, worldTangent.xyz()).GetNormalized() * worldTangent.w;

	Vector3 rnormal = Vector3(tangentSpaceNormal.r * 2.0f - 1.0, tangentSpaceNormal.g * 2.0f - 1.0f, tangentSpaceNormal.b * 2.0f - 1.0f) * -1;
	outNormal = Vector3(0, 0, 0);
	outNormal.x = worldTangent.x * rnormal.x + wbnormal.x * rnormal.y + worldNormal.x * rnormal.z;
	outNormal.y = worldTangent.y * rnormal.x + wbnormal.y * rnormal.y + worldNormal.y * rnormal.z;
	outNormal.z = worldTangent.z * rnormal.x + wbnormal.z * rnormal.y + worldNormal.z * rnormal.z;
	outNormal.Normalize();
	if (Vector3::Dot(outNormal, worldNormal) < 0)
		outNormal *= -1;
}

float RayTracer::RTMaterialShader::FresnelSchlickRoughness(float cosTheta, float F0, float roughness)
{
	return F0 + (Math::Max(1.0f - roughness, F0) - F0) * (float)pow(1.0 - cosTheta, 5.0);
}

Color RayTracer::RTMaterialShader::FresnelSchlickRoughness(float cosTheta, const Color& F0, float roughness)
{
	Color f = Color(Math::Max(1.0f - roughness, F0.r), Math::Max(1.0f - roughness, F0.g),
		Math::Max(1.0f - roughness, F0.b), 1.0f);
	Color r = F0 + (f - F0) * (float)pow(1.0 - cosTheta, 5.0);
	r.a = 1.0f;
	return r;
}

float RayTracer::RTMaterialShader::FresnelSchlickRoughnessRefractive(float cosTheta, float refractive, float roughness)
{
	float F0 = (1.0f - refractive) / (1.0f + refractive);
	F0 = F0 * F0;
	return F0 + (Math::Max(1.0f - roughness, F0) - F0) * (float)pow(1.0 - cosTheta, 5.0);
}

RayTracer::RTEmissiveShader::RTEmissiveShader()
{
}

RayTracer::RTEmissiveShader::~RTEmissiveShader()
{
}

Color RayTracer::RTEmissiveShader::PathTracing(PathTracer* pathTracer, SamplerBase* sampler, RayTracerScene* scene, const Ray& ray, RayTracingResult& result)
{
	return color;
}

RayTracer::RTTransparencyShader::RTTransparencyShader()
{
}

RayTracer::RTTransparencyShader::~RTTransparencyShader()
{
}

Color RayTracer::RTTransparencyShader::PathTracing(PathTracer* pathTracer, SamplerBase* sampler, RayTracerScene* scene, const Ray& ray, RayTracingResult& result)
{
	return Color();
}
