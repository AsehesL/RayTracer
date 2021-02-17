#pragma once
#include "RayTracerMaterialShader.h"

class Texture;

namespace RayTracer
{
	class BRDF;

	struct RTPBRShaderProperty
	{
	public:
		RTPBRShaderProperty();
		bool GetTangentSpaceNormal(const RayTracingResult& result, Color& tangentSpaceNormal);
		float GetRoughness(const RayTracingResult& result);
		float GetMetallic(const RayTracingResult& result);
		float GetOcclusion(const RayTracingResult& result);
		Color GetAlbedo(const RayTracingResult& result);
		Color GetEmissive(const RayTracingResult& result);

	private:
		void SampleAlbedo(const RayTracingResult& result);
		void SampleNormal(const RayTracingResult& result);
		void SampleMRO(const RayTracingResult& result);
		void SampleEmissive(const RayTracingResult& result);

	public:
		Vector2 uvTile;
		Texture* albedoTex;
		Texture* bump;
		Texture* mro;
		Texture* emissiveTex;
		float roughness;
		float metallic;
		Color emissiveColor;
		Color albedoColor;

	private:
		bool m_isAlbedoSampled;
		bool m_isNormalSampled;
		bool m_isMROSampled;
		bool m_isEmissiveSampled;
		Color m_tangentSpaceNormal;
		float m_occlusion;
		bool m_hasTangentSpaceNormal;
	};

	class RTPBRShaderBase : public RTMaterialShader
	{
	public:
		RTPBRShaderBase();
		virtual ~RTPBRShaderBase() override;
		virtual Color PathTracing(PathTracer* pathTracer, SamplerBase* sampler, RayTracerScene* scene, const Ray& ray, RayTracingResult& result) override;
	
	protected:
		virtual Color PathTracingDirectionalLight(PathTracer* pathTracer, SamplerBase* sampler, RayTracerScene* scene, const Ray& ray, const RayTracingResult& result, RTPBRShaderProperty& shaderProperty) = 0;
		virtual Color PathTracingAmbientLighting(PathTracer* pathTracer, SamplerBase* sampler, RayTracerScene* scene, const Ray& ray, const RayTracingResult& result, RTPBRShaderProperty& shaderProperty, int depth) = 0;

		virtual void GetProperty(RTPBRShaderProperty& shaderProperty) = 0;

	protected:
		BRDF* m_specularBRDF;
		BRDF* m_diffuseBRDF;
	};

	class RTPBRStandardShader : public RTPBRShaderBase
	{
	public:
		RTPBRStandardShader();
		virtual ~RTPBRStandardShader() override;

	protected:
		virtual Color PathTracingDirectionalLight(PathTracer* pathTracer, SamplerBase* sampler, RayTracerScene* scene, const Ray& ray, const RayTracingResult& result, RTPBRShaderProperty& shaderProperty) override;
		virtual Color PathTracingAmbientLighting(PathTracer* pathTracer, SamplerBase* sampler, RayTracerScene* scene, const Ray& ray, const RayTracingResult& result, RTPBRShaderProperty& shaderProperty, int depth) override;

		virtual void GetProperty(RTPBRShaderProperty& shaderProperty) override;

	public:
		Vector2 uvTile;
		Texture* albedoTex;
		Texture* bump;
		Texture* mro;
		Texture* emissiveTex;
		float roughness;
		float metallic;
		Color emissiveColor;
		Color albedoColor;
	};
}