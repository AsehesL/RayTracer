#pragma once

#include "RayTracerShader.h"

namespace RayTracer
{
	class RTMaterialShader : public RTShaderBase
	{
	public:
		RTMaterialShader();
		virtual ~RTMaterialShader() override;

		virtual Color PathTracing(PathTracer* pathTracer, SamplerBase* sampler, RayTracerScene* scene, const Ray& ray, RayTracingResult& result) = 0;

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

		virtual Color PathTracing(PathTracer* pathTracer, SamplerBase* sampler, RayTracerScene* scene, const Ray& ray, RayTracingResult& result) override;
	
	public:
		Color color;
	};

	class RTTransparencyShader : public RTMaterialShader
	{
	public:
		RTTransparencyShader();
		virtual ~RTTransparencyShader() override;

		virtual Color PathTracing(PathTracer* pathTracer, SamplerBase* sampler, RayTracerScene* scene, const Ray& ray, RayTracingResult& result) override;
	};
}