#pragma once

#include "RayTracerShader.h"

namespace RayTracer
{
	class PathTracerScene;
	class PhotonMapper;

	enum class MaterialType
	{
		Diffuse,
		Reflect,
		Refract,
	};

	struct PhotonMappingResult 
	{
	public:
		MaterialType materialType;
		Color flux;
		bool specular;
		Ray ray;
	};

	class RTMaterialShader : public RTShaderBase
	{
	public:
		RTMaterialShader();
		virtual ~RTMaterialShader() override;

		virtual Color PathTracing(PathTracer* pathTracer, SamplerBase* sampler, PathTracerScene* scene, const Ray& ray, RayTracingResult& result) = 0;
		virtual void PhotonMapperInteract(SamplerBase* sampler, const Vector3& direction, RayTracingResult& hitResult, PhotonMappingResult& photonMappingResult) = 0;
		virtual bool IsEmissive() const = 0;
		virtual Color GetEmissions() const = 0;

	protected:
		static void RecalucateNormal(const Vector3& worldNormal, const Vector4& worldTangent, const Color& tangentSpaceNormal, Vector3& outNormal);
		static float FresnelSchlickRoughness(float cosTheta, float F0, float roughness);
		static Color FresnelSchlickRoughness(float cosTheta, const Color& F0, float roughness);
		static float FresnelSchlickRoughnessRefractive(float cosTheta, float refractive, float roughness);
	};

	class RTEmissiveShader : public RTMaterialShader 
	{
	public:
		RTEmissiveShader();
		virtual ~RTEmissiveShader() override;

		virtual Color PathTracing(PathTracer* pathTracer, SamplerBase* sampler, PathTracerScene* scene, const Ray& ray, RayTracingResult& result) override;
		virtual void PhotonMapperInteract(SamplerBase* sampler, const Vector3& direction, RayTracingResult& hitResult, PhotonMappingResult& photonMappingResult) override;
		virtual bool IsEmissive() const { return true; }
		virtual Color GetEmissions() const override;

	public:
		Color color;
	};

	class RTTransparencyShader : public RTMaterialShader
	{
	public:
		RTTransparencyShader();
		virtual ~RTTransparencyShader() override;

		virtual Color PathTracing(PathTracer* pathTracer, SamplerBase* sampler, PathTracerScene* scene, const Ray& ray, RayTracingResult& result) override;
		virtual void PhotonMapperInteract(SamplerBase* sampler, const Vector3& direction, RayTracingResult& hitResult, PhotonMappingResult& photonMappingResult) override;
		virtual bool IsEmissive() const override;
		virtual Color GetEmissions() const override;

	private:
		Color PathTracingDirectionalLight(PathTracer* pathTracer, SamplerBase* sampler, PathTracerScene* scene, const Vector3& worldPoint, const Vector3& worldNormal, const Vector3& viewDirection);
		Color PathTracingAmbientLighting(PathTracer* pathTracer, SamplerBase* sampler, PathTracerScene* scene, const Vector3& worldPoint, const Vector3& worldNormal, const Vector3& viewDirection, int depth);
	
	public:
		float roughness;
		Color color;
		float refractive;

	private:
		class BRDF* m_specularBRDF;
	};
}