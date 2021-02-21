#include "RayTracerMaterialShader.h"
#include "../Scene/PathTracerScene.h"
#include "../Light/RayTracerSkyLight.h"
#include "../Light/RayTracerSunLight.h"
#include "../Sampler/Sampler.h"
#include "../Integrator/PathTracer/PathTracer.h"
#include "BRDF.h"

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

Color RayTracer::RTEmissiveShader::PathTracing(PathTracer* pathTracer, SamplerBase* sampler, PathTracerScene* scene, const Ray& ray, RayTracingResult& result)
{
	return color;
}

void RayTracer::RTEmissiveShader::PhotonMapperInteract(SamplerBase* sampler, const Vector3& direction, RayTracingResult& hitResult, PhotonMappingResult& photonMappingResult)
{
}

Color RayTracer::RTEmissiveShader::GetEmissions() const
{
	return color;
}

RayTracer::RTTransparencyShader::RTTransparencyShader()
{
	m_specularBRDF = new CookTorranceBRDF();
}

RayTracer::RTTransparencyShader::~RTTransparencyShader()
{
	delete m_specularBRDF;
}

Color RayTracer::RTTransparencyShader::PathTracing(PathTracer* pathTracer, SamplerBase* sampler, PathTracerScene* scene, const Ray& ray, RayTracingResult& result)
{
	Color col = COLOR_BLACK;;

	if (scene != nullptr && scene->GetSunLight() != nullptr && scene->GetSunLight()->isActive)
	{
		//单独计算方向光照
		col += PathTracingDirectionalLight(pathTracer, sampler, scene, result.hit, result.normal, ray.direction);
	}

	//计算环境光照
	col += PathTracingAmbientLighting(pathTracer, sampler, scene, result.hit, result.normal, ray.direction, result.depth);

	return col;
}

void RayTracer::RTTransparencyShader::PhotonMapperInteract(SamplerBase* sampler, const Vector3& direction, RayTracingResult& hitResult, PhotonMappingResult& photonMappingResult)
{
}

bool RayTracer::RTTransparencyShader::IsEmissive() const
{
	return false;
}

Color RayTracer::RTTransparencyShader::GetEmissions() const
{
	return Color();
}

Color RayTracer::RTTransparencyShader::PathTracingDirectionalLight(PathTracer* pathTracer, SamplerBase* sampler, PathTracerScene* scene, const Vector3& worldPoint, const Vector3& worldNormal, const Vector3& viewDirection)
{
	Vector3 N = ImportanceGGXDirection(worldNormal, sampler, roughness);
	float et;
	Vector3 n;
	float cosine;
	float reflectProb;
	float ndv = (float)Vector3::Dot(viewDirection, N);
	if (ndv > 0)
	{
		n = -1 * N;
		et = refractive;
		cosine = refractive * ndv;

	}
	else
	{
		n = N;
		et = 1.0f / refractive;
		cosine = -ndv;
	}

	Vector3 refrac;
	if (Vector3::Refract(viewDirection, n, et, refrac))
	{
		reflectProb = FresnelSchlickRoughnessRefractive(cosine, refractive, roughness);
	}
	else
	{
		reflectProb = 1.0f;
	}

	if (sampler->GetRandom() < reflectProb)
	{
		Vector3 L = -1.0 * scene->GetSunLight()->GetDirection(sampler);
		double ndl = Vector3::Dot(worldNormal, L);
		if (ndl < 0.0)
			return COLOR_BLACK;
		Ray lray = Ray(worldPoint, L);
		bool shadow = TracingOnce(lray, scene);
		if (shadow)
			return COLOR_BLACK;
		ndl = Math::Max(ndl, 0.0);
		return  scene->GetSunLight()->GetColor() * m_specularBRDF->GetDirectionalBRDF(-1.0 * viewDirection, L, worldNormal, roughness) * (float)ndl;
	}
	else
	{
		return COLOR_BLACK;
	}
}

Color RayTracer::RTTransparencyShader::PathTracingAmbientLighting(PathTracer* pathTracer, SamplerBase* sampler, PathTracerScene* scene, const Vector3& worldPoint, const Vector3& worldNormal, const Vector3& viewDirection, int depth)
{
	Vector3 N = ImportanceGGXDirection(worldNormal, sampler, roughness);
	float et;
	Vector3 n;
	float cosine;
	float reflectProb;
	float ndv = (float)Vector3::Dot(viewDirection, N);
	if (ndv > 0)
	{
		n = -1 * N;
		et = refractive;
		cosine = refractive * ndv;

	}
	else
	{
		n = N;
		et = 1.0f / refractive;
		cosine = -ndv;
	}

	Vector3 refrac;
	if (Vector3::Refract(viewDirection, n, et, refrac))
	{
		reflectProb = FresnelSchlickRoughnessRefractive(cosine, refractive, roughness);
	}
	else
	{
		reflectProb = 1.0f;
	}

	if (sampler->GetRandom() < reflectProb)
	{
		Ray lray = Ray(worldPoint, Vector3::Reflect(viewDirection * -1, N));
		Color tracingCol;
		pathTracer->Tracing(lray, sampler, scene, depth + 1, tracingCol);
		return tracingCol;
	}
	else
	{
		Ray lray = Ray(worldPoint, refrac);
		Color tracingCol;
		pathTracer->Tracing(lray, sampler, scene, depth + 1, tracingCol);
		return color * tracingCol;
	}
}
