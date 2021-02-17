#pragma once
#include "../../Core/Vector.h"
#include "../../Core/Color.h"

namespace RayTracer
{
	class SamplerBase;

	class RayTracerSunLight
	{
	public:
		RayTracerSunLight();
		~RayTracerSunLight();

		void SetDirection(const Vector3& direction);
		Vector3 GetDirection(SamplerBase* sampler);
		void SetColor(const Color& color);
		Color GetColor() const;

	public:
		bool isActive;

	private:
		Color m_sunColor;
		Vector3 m_sunDirection;
	};
}