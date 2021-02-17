#pragma once
#include "../Core/Ray.h"
#include "RayTracingResult.h"

class Camera;

namespace RayTracer
{
	class PrimitiveBase;
	class RayTracerSkyLight;
	class RayTracerSunLight;
	class RTSkyShader;

	class RayTracerScene
	{
	public:
		RayTracerScene();
		~RayTracerScene();

		void AddPrimitive(PrimitiveBase* primitive);
		void Build();

		RayTracerSkyLight* GetSkyLight() const;
		RayTracerSunLight* GetSunLight() const;

		bool Tracing(const Ray& ray, RayTracingResult& result);

	public:
		Camera* camera;

	private:
		class SceneDataBase* m_sceneData;
		RayTracerSkyLight* m_skyLight;
		RayTracerSunLight* m_sunLight;
	};
};