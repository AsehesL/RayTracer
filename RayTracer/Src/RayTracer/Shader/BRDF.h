#pragma once
#include "../../Core/Vector.h"

namespace RayTracer
{
	class BRDF
	{
	public:
		BRDF();
		virtual ~BRDF();

		virtual float GetBRDF(const Vector3& inDir, const Vector3& outDir, const Vector3& normal, float roughness) = 0;

		virtual float GetDirectionalBRDF(const Vector3& inDir, const Vector3& outDir, const Vector3& normal, float roughness) = 0;
	};

	class LambertatianBRDF : public BRDF
	{
	public:
		LambertatianBRDF();
		virtual ~LambertatianBRDF() override;
		virtual float GetBRDF(const Vector3& inDir, const Vector3& outDir, const Vector3& normal, float roughness) override;
		virtual float GetDirectionalBRDF(const Vector3& inDir, const Vector3& outDir, const Vector3& normal, float roughness) override;
	};

	class CookTorranceBRDF : public BRDF
	{
	public:
		CookTorranceBRDF();
		virtual ~CookTorranceBRDF() override;
		virtual float GetBRDF(const Vector3& inDir, const Vector3& outDir, const Vector3& normal, float roughness) override;
		virtual float GetDirectionalBRDF(const Vector3& inDir, const Vector3& outDir, const Vector3& normal, float roughness) override;
	
	private:
		float D_GGX(const Vector3& half, const Vector3& normal, float roughness);
		float G_SmithGGX(const Vector3& inDir, const Vector3& outDir, const Vector3& normal, float roughness);
		float GeometrySchlickGGX(float NdotV, float roughness);
	};
}