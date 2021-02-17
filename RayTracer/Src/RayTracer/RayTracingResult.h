#pragma once
#include "../Core/Vector.h"

namespace RayTracer
{
	class RTMaterialShader;

	struct RayTracingResult
	{
	public:
		Vector3 hit;
		Vector2 texcoord;
		Vector3 normal;
		Vector4 tangent;
		int depth;
		RTMaterialShader* material;
		double distance;
	};
}

