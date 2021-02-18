#include "RayTracerPBRShader.h"
#include "../../Texture/Texture.h"
#include "../Scene/PathTracerScene.h"
#include "../Light/RayTracerSkyLight.h"
#include "../Light/RayTracerSunLight.h"
#include "../Sampler/Sampler.h"
#include "../Integrator/PathTracer/PathTracer.h"
#include "BRDF.h"

RayTracer::RTPBRShaderBase::RTPBRShaderBase() : RayTracer::RTMaterialShader()
{
	m_specularBRDF = nullptr;
	m_diffuseBRDF = nullptr;
}

RayTracer::RTPBRShaderBase::~RTPBRShaderBase()
{
	if (m_diffuseBRDF)
		delete m_diffuseBRDF;
	m_diffuseBRDF = nullptr;
	if (m_specularBRDF)
		delete m_specularBRDF;
	m_specularBRDF = nullptr;
}

Color RayTracer::RTPBRShaderBase::PathTracing(PathTracer* pathTracer, SamplerBase* sampler, PathTracerScene* scene, const Ray& ray, RayTracingResult& result)
{
	RTPBRShaderProperty shaderProperty;
	GetProperty(shaderProperty);

	//计算世界空间法线（可能需要根据切空间法线重新计算法线）
	Color tangentSpaceNormalColor;
	if (shaderProperty.GetTangentSpaceNormal(result, tangentSpaceNormalColor))
	{
		Vector3 worldNormal = result.normal;
		RecalucateNormal(result.normal, result.tangent, tangentSpaceNormalColor, worldNormal);
		result.normal = worldNormal;
	}

	Color resultColor = Color(0,0,0,0);

	if (scene != nullptr && scene->GetSunLight() != nullptr && scene->GetSunLight()->isActive)
	{
		//单独计算方向光照
		resultColor += PathTracingDirectionalLight(pathTracer, sampler, scene, ray, result, shaderProperty);
	}

	//计算环境光照
	resultColor += shaderProperty.GetOcclusion(result) * PathTracingAmbientLighting(pathTracer, sampler, scene, ray, result, shaderProperty, result.depth);

	return resultColor + shaderProperty.GetEmissive(result);
}

void RayTracer::RTPBRShaderBase::PhotonMapperInteract(SamplerBase* sampler, const Vector3& direction, RayTracingResult& hitResult, PhotonMappingResult& photonMappingResult)
{
	RTPBRShaderProperty shaderProperty;
	GetProperty(shaderProperty);

	Color tangentSpaceNormalColor;
	if (shaderProperty.GetTangentSpaceNormal(hitResult, tangentSpaceNormalColor))
	{
		Vector3 worldNormal = hitResult.normal;
		RecalucateNormal(hitResult.normal, hitResult.tangent, tangentSpaceNormalColor, worldNormal);
		hitResult.normal = worldNormal;
	}

	float ndv = (float)Math::Max(0.0, Vector3::Dot(hitResult.normal, -1.0 * direction));
	Color albedo = shaderProperty.GetAlbedo(hitResult);
	float roughness = shaderProperty.GetRoughness(hitResult);
	if (sampler->GetRandom() < shaderProperty.GetMetallic(hitResult))
	{
		//Metallic
		Color F = FresnelSchlickRoughness(ndv, albedo, roughness);
		
		Vector3 N = ImportanceGGXDirection(hitResult.normal, sampler, roughness);
		Vector3 L = Vector3::Reflect(direction * -1, N);
		float ndl = (float)Math::Max(Vector3::Dot(hitResult.normal, L), 0.0);
		photonMappingResult.ray = Ray(hitResult.hit, L);
		photonMappingResult.specular = true;
		photonMappingResult.materialType = MaterialType::Reflect;
		photonMappingResult.flux = F * m_specularBRDF->GetBRDF(-1.0 * direction, L, hitResult.normal, roughness) * ndl;
		return;
	}
	else
	{
		//Dielectric
		float F = FresnelSchlickRoughness(ndv, 0.04f, roughness);
		if (sampler->GetRandom() < F)
		{
			//Specular
			Vector3 N = ImportanceGGXDirection(hitResult.normal, sampler, roughness);
			Vector3 L = Vector3::Reflect(direction * -1, N);
			float ndl = (float)Math::Max(Vector3::Dot(hitResult.normal, L), 0.0);
			photonMappingResult.ray = Ray(hitResult.hit, L);
			photonMappingResult.specular = true;
			photonMappingResult.materialType = MaterialType::Reflect;
			float brdf = m_specularBRDF->GetBRDF(-1.0 * direction, L, hitResult.normal, roughness) * ndl;
			photonMappingResult.flux = Color(brdf,brdf,brdf);
			return;
		}
		else
		{
			//Diffuse
			Vector3 w = hitResult.normal;
			Vector3 u = Vector3::Cross(Vector3(0.00424, 1, 0.00764), w);
			u.Normalize();
			Vector3 v = Vector3::Cross(u, w);
			Vector3 sp;
			sampler->SampleHemiSphere(0, sp);

			Vector3 wi = sp.x * u + sp.y * v + sp.z * w;
			wi.Normalize();
			float ndl = (float)Math::Max(0.0, Vector3::Dot(hitResult.normal, wi));

			photonMappingResult.ray = Ray(hitResult.hit, wi);
			photonMappingResult.specular = false;
			photonMappingResult.materialType = MaterialType::Diffuse;
			photonMappingResult.flux = ndl * albedo * m_diffuseBRDF->GetBRDF(-1.0 * direction, wi, hitResult.normal, roughness);
		}

		return;
	}
}

RayTracer::RTPBRShaderProperty::RTPBRShaderProperty()
{
	m_isNormalSampled = false;
	m_isMROSampled = false;
	m_isEmissiveSampled = false;
	m_isAlbedoSampled = false;
	m_hasTangentSpaceNormal = false;
	albedoTex = nullptr;
	bump = nullptr;
	mro = nullptr;
	emissiveTex = nullptr;
	m_occlusion = 1.0f;
	metallic = 0;
	roughness = 0.5f;
}

bool RayTracer::RTPBRShaderProperty::GetTangentSpaceNormal(const RayTracingResult& result, Color& tangentSpaceNormal)
{
	if (!m_isNormalSampled)
		SampleNormal(result);
	tangentSpaceNormal = m_tangentSpaceNormal;
	return m_hasTangentSpaceNormal;
}

float RayTracer::RTPBRShaderProperty::GetRoughness(const RayTracingResult& result)
{
	if (!m_isMROSampled)
		SampleMRO(result);
	return roughness;
}

float RayTracer::RTPBRShaderProperty::GetMetallic(const RayTracingResult& result)
{
	if (!m_isMROSampled)
		SampleMRO(result);
	return metallic;
}

float RayTracer::RTPBRShaderProperty::GetOcclusion(const RayTracingResult& result)
{
	if (!m_isMROSampled)
		SampleMRO(result);
	return m_occlusion;
}

Color RayTracer::RTPBRShaderProperty::GetAlbedo(const RayTracingResult& result)
{
	if (!m_isAlbedoSampled)
		SampleAlbedo(result);
	return albedoColor;
}

Color RayTracer::RTPBRShaderProperty::GetEmissive(const RayTracingResult& result)
{
	if (!m_isEmissiveSampled)
		SampleEmissive(result);
	return emissiveColor;
}

void RayTracer::RTPBRShaderProperty::SampleAlbedo(const RayTracingResult& result)
{
	if (!m_isAlbedoSampled)
	{
		if (albedoTex != nullptr)
		{
			Color albedoTexColor;
			albedoTex->Sample((float)(result.texcoord.x * uvTile.x), (float)(result.texcoord.y * uvTile.y), albedoTexColor);
			albedoColor *= albedoTexColor;
		}
		m_isAlbedoSampled = true;
	}
}

void RayTracer::RTPBRShaderProperty::SampleNormal(const RayTracingResult& result)
{
	if (!m_isNormalSampled)
	{
		if (bump == nullptr)
			m_hasTangentSpaceNormal = false;
		else
		{
			m_hasTangentSpaceNormal = true;
			bump->Sample((float)(result.texcoord.x * uvTile.x), (float)(result.texcoord.y * uvTile.y), m_tangentSpaceNormal);
		}
		m_isNormalSampled = true;
	}
}

void RayTracer::RTPBRShaderProperty::SampleMRO(const RayTracingResult& result)
{
	if (!m_isMROSampled)
	{
		if (mro != nullptr)
		{
			Color mroColor;
			mro->Sample((float)(result.texcoord.x * uvTile.x), (float)(result.texcoord.y * uvTile.y), mroColor);
			metallic *= mroColor.r;
			m_occlusion = mroColor.b;
			roughness *= mroColor.g;
		}
		m_isMROSampled = true;
	}
}

void RayTracer::RTPBRShaderProperty::SampleEmissive(const RayTracingResult& result)
{
	if (!m_isEmissiveSampled)
	{
		if (emissiveTex != nullptr)
		{
			Color emissiveTexColor;
			emissiveTex->Sample((float)(result.texcoord.x * uvTile.x), (float)(result.texcoord.y * uvTile.y), emissiveTexColor);
			emissiveColor *= emissiveTexColor;
		}
		m_isEmissiveSampled = true;
	}
}

RayTracer::RTPBRStandardShader::RTPBRStandardShader() : RayTracer::RTPBRShaderBase()
{
	m_specularBRDF = new CookTorranceBRDF();
	m_diffuseBRDF = new LambertatianBRDF();
}

RayTracer::RTPBRStandardShader::~RTPBRStandardShader()
{
}

Color RayTracer::RTPBRStandardShader::PathTracingDirectionalLight(PathTracer* pathTracer, SamplerBase* sampler, PathTracerScene* scene, const Ray& ray, const RayTracingResult& result, RTPBRShaderProperty& shaderProperty)
{
	float ndv = (float)Math::Max(0.0, Vector3::Dot(result.normal, -1.0 * ray.direction));
	Vector3 L = -1.0 * scene->GetSunLight()->GetDirection(sampler);
	Color albedo = shaderProperty.GetAlbedo(result);
	float roughness = shaderProperty.GetRoughness(result);
	if (sampler->GetRandom() < shaderProperty.GetMetallic(result))
	{
		//Metallic
		Color F = FresnelSchlickRoughness(ndv, albedo, roughness);

		double ndl = Vector3::Dot(result.normal, L);
		if (ndl < 0.0)
			return COLOR_BLACK;
		Ray lray = Ray(result.hit, L);
		bool shadow = TracingOnce(lray, scene);
		if (shadow)
			return COLOR_BLACK;
		ndl = Math::Max(ndl, 0.0);
		return scene->GetSunLight()->GetColor() * F * m_specularBRDF->GetDirectionalBRDF(-1.0 * ray.direction, L, result.normal, roughness) * (float)ndl;
	}
	else
	{
		Color resultColor = Color(0,0,0,0);

		//Dielectric
		float F = FresnelSchlickRoughness(ndv, 0.04f, roughness);

		double ndl = Vector3::Dot(result.normal, L);
		if (ndl < 0.0)
			return COLOR_BLACK;
		Ray lray = Ray(result.hit, L);
		bool shadow = TracingOnce(lray, scene);
		if (shadow)
			return COLOR_BLACK;
		ndl = Math::Max(ndl, 0.0);


		resultColor += F * scene->GetSunLight()->GetColor() * m_specularBRDF->GetDirectionalBRDF(-1.0 * ray.direction, L, result.normal, roughness) * (float)ndl;
		resultColor += (1.0f - F) * albedo * scene->GetSunLight()->GetColor() * m_diffuseBRDF->GetDirectionalBRDF(-1.0 * ray.direction, L, result.normal, roughness) * (float)ndl;

		return resultColor;
	}
}

Color RayTracer::RTPBRStandardShader::PathTracingAmbientLighting(PathTracer* pathTracer, SamplerBase* sampler, PathTracerScene* scene, const Ray& ray, const RayTracingResult& result, RTPBRShaderProperty& shaderProperty, int depth)
{
	float ndv = (float)Math::Max(0.0, Vector3::Dot(result.normal, -1.0 * ray.direction));
	Color albedo = shaderProperty.GetAlbedo(result);
	float roughness = shaderProperty.GetRoughness(result);
	if (sampler->GetRandom() < shaderProperty.GetMetallic(result))
	{
		//Metallic
		Color F = FresnelSchlickRoughness(ndv, albedo, roughness);
		Vector3 N = ImportanceGGXDirection(result.normal, sampler, roughness);
		Vector3 L = Vector3::Reflect(ray.direction * -1, N);
		Ray lray = Ray(result.hit, L);
		Color col = Color(0,0,0,0);
		pathTracer->Tracing(lray, sampler, scene, depth + 1, col);
		float ndl = (float)Math::Max(Vector3::Dot(result.normal, L), 0.0);
		return Color(col.r * F.r, col.g * F.g, col.b * F.b, col.a) * m_specularBRDF->GetBRDF(-1.0 * ray.direction, L, result.normal, roughness) * ndl;
	}
	else
	{

		Color resultColor = Color(0,0,0,0);

		//Dielectric
		float F = FresnelSchlickRoughness(ndv, 0.04f, roughness);
		if (sampler->GetRandom() < F)
		{
			//Specular
			Vector3 N = ImportanceGGXDirection(result.normal, sampler, roughness);
			Vector3 L = Vector3::Reflect(ray.direction * -1, N);
			Ray lray = Ray(result.hit, L);
			float ndl = (float)Math::Max(Vector3::Dot(result.normal, L), 0.0);
			pathTracer->Tracing(lray, sampler, scene, depth + 1, resultColor);
			resultColor *= m_specularBRDF->GetBRDF(-1.0 * ray.direction, L, result.normal, roughness) * ndl;
		}
		else
		{
			//Diffuse
			Vector3 w = result.normal;
			Vector3 u = Vector3::Cross(Vector3(0.00424, 1, 0.00764), w);
			u.Normalize();
			Vector3 v = Vector3::Cross(u, w);
			Vector3 sp;
			sampler->SampleHemiSphere(0, sp);

			Vector3 wi = sp.x * u + sp.y * v + sp.z * w;
			wi.Normalize();

			float ndl = (float)Math::Max(0.0, Vector3::Dot(result.normal, wi));

			Ray lray = Ray(result.hit, wi);
			Color tracingColor;
			pathTracer->Tracing(lray, sampler, scene, depth + 1, tracingColor);
			resultColor = ndl * albedo * tracingColor * m_diffuseBRDF->GetBRDF(-1.0 * ray.direction, wi, result.normal, roughness);
		}

		return resultColor;
	}
}

void RayTracer::RTPBRStandardShader::GetProperty(RTPBRShaderProperty& shaderProperty)
{
	shaderProperty.albedoColor = albedoColor;
	shaderProperty.albedoTex = albedoTex;
	shaderProperty.bump = bump;
	shaderProperty.emissiveColor = emissiveColor;
	shaderProperty.emissiveTex = emissiveTex;
	shaderProperty.metallic = metallic;
	shaderProperty.mro = mro;
	shaderProperty.roughness = roughness;
	shaderProperty.uvTile = uvTile;
}
