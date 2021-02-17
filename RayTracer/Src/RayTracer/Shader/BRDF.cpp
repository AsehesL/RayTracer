#include "BRDF.h"

RayTracer::BRDF::BRDF()
{
}

RayTracer::BRDF::~BRDF()
{
}

RayTracer::LambertatianBRDF::LambertatianBRDF()
{
}

RayTracer::LambertatianBRDF::~LambertatianBRDF()
{
}

float RayTracer::LambertatianBRDF::GetBRDF(const Vector3& inDir, const Vector3& outDir, const Vector3& normal, float roughness)
{
	return (float)Math::InvPI;
}

float RayTracer::LambertatianBRDF::GetDirectionalBRDF(const Vector3& inDir, const Vector3& outDir, const Vector3& normal, float roughness)
{
	return (float)Math::InvPI;
}

RayTracer::CookTorranceBRDF::CookTorranceBRDF()
{
}

RayTracer::CookTorranceBRDF::~CookTorranceBRDF()
{
}

float RayTracer::CookTorranceBRDF::GetBRDF(const Vector3& inDir, const Vector3& outDir, const Vector3& normal, float roughness)
{
	double denominator = 4.0 * Math::Max(Vector3::Dot(normal, inDir), 0.0) * Math::Max(Vector3::Dot(normal, outDir), 0.0) + 0.001;

	float nominator = G_SmithGGX(inDir, outDir, normal, roughness);

	return nominator / (float)denominator;
}

float RayTracer::CookTorranceBRDF::GetDirectionalBRDF(const Vector3& inDir, const Vector3& outDir, const Vector3& normal, float roughness)
{
	double denominator = 4.0 * Math::Max(Vector3::Dot(normal, inDir), 0.0) * Math::Max(Vector3::Dot(normal, outDir), 0.0) + 0.001;

	float nominator = D_GGX((inDir + outDir).GetNormalized(), normal, roughness) * G_SmithGGX(inDir, outDir, normal, roughness);

	return nominator / (float)denominator;
}

float RayTracer::CookTorranceBRDF::D_GGX(const Vector3& half, const Vector3& normal, float roughness)
{
	float NdotH = (float)Vector3::Dot(half, normal);

	float root = roughness / (NdotH * NdotH * (roughness * roughness - 1.0f) + 1.0f);

	return (float)Math::InvPI * (root * root);
}

float RayTracer::CookTorranceBRDF::G_SmithGGX(const Vector3& inDir, const Vector3& outDir, const Vector3& normal, float roughness)
{
	float NdotV = (float)Math::Max(0.0, Vector3::Dot(inDir, normal));
	float NdotL = (float)Math::Max(0.0, Vector3::Dot(outDir, normal));

	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

float RayTracer::CookTorranceBRDF::GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0f);
	float k = (r * r) / 8.0f;

	float nom = NdotV;
	float denom = NdotV * (1.0f - k) + k;

	return nom / denom;
}
