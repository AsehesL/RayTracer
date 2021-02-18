#pragma once

#include "../../Core/Ray.h"
#include "../RayTracingResult.h"
#include "RayTracerScene.h"

#include <vector>

namespace RayTracer
{

	class PhotonMapperScene : public RayTracerScene
	{
	public:
		PhotonMapperScene();
		virtual ~PhotonMapperScene() override;

		virtual void AddPrimitive(PrimitiveBase* primitive) override;

		int GetLightCounts() const;
		PrimitiveBase* GetLight(int index) const;

	private:
		std::vector<PrimitiveBase*> m_lights;
	};
}