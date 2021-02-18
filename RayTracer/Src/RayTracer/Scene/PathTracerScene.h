#pragma once

#include "../../Core/Ray.h"
#include "../RayTracingResult.h"
#include "RayTracerScene.h"

namespace RayTracer
{
	class RayTracerSkyLight;
	class RayTracerSunLight;

	class PathTracerScene : public RayTracerScene
	{
	public:
		PathTracerScene();
		virtual ~PathTracerScene() override;

		virtual void AddPrimitive(PrimitiveBase* primitive) override;

		RayTracerSkyLight* GetSkyLight() const;
		RayTracerSunLight* GetSunLight() const;

	private:
		RayTracerSkyLight* m_skyLight;
		RayTracerSunLight* m_sunLight;
	};
}