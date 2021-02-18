#pragma once
#include "../../Core/Ray.h"
#include "../RayTracingResult.h"

class Camera;

namespace RayTracer
{
	class PrimitiveBase;
	
	class RTSkyShader;

	class RayTracerScene
	{
	public:
		RayTracerScene();
		virtual ~RayTracerScene();

		virtual void AddPrimitive(PrimitiveBase* primitive) = 0;
		void Build();

		bool Tracing(const Ray& ray, RayTracingResult& result);

	public:
		Camera* camera;

	protected:
		class SceneDataBase* m_sceneData;
	};
};