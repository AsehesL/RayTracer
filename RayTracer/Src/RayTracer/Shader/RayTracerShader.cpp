#include "RayTracerShader.h"
#include "../Scene/RayTracerScene.h"
#include "../Sampler/Sampler.h"

RayTracer::RTShaderBase::RTShaderBase()
{
}

RayTracer::RTShaderBase::~RTShaderBase()
{
}

bool RayTracer::RTShaderBase::TracingOnce(const Ray& ray, RayTracerScene* scene)
{
	if (scene == nullptr)
		return false;
	RayTracingResult hitResult;
	hitResult.distance = DBL_MAX;

	if (scene->Tracing(ray, hitResult))
	{
		return true;
	}
	return false;
}

Vector3 RayTracer::RTShaderBase::ImportanceSampleGGX(const Vector2& sample, float roughness)
{
	float a = roughness * roughness;

	double phi = 2.0 * Math::PI * sample.x;
	double cos_theta = sqrt((1.0 - sample.y) / (1.0 + (a * a - 1.0) * sample.y));
	double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

	return Vector3(cos(phi) * sin_theta, sin(phi) * sin_theta, cos_theta);
}

Vector3 RayTracer::RTShaderBase::ImportanceGGXDirection(const Vector3& direction, SamplerBase* sampler, float roughness)
{
	Vector3 w = direction;
	Vector3 u = Vector3::Cross(Vector3(0.00424, 1, 0.00764), w);
	u.Normalize();
	Vector3 v = Vector3::Cross(u, w);
	Vector2 sample;
	sampler->Sample(sample);
	Vector3 sp = ImportanceSampleGGX(sample, roughness);
	Vector3 l = sp.x * u + sp.y * v + sp.z * w;
	if (Vector3::Dot(l, direction) < 0.0)
		l = -sp.x * u - sp.y * v - sp.z * w;
	l.Normalize();
	return l;
}
