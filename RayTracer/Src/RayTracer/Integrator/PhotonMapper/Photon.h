#pragma once

#include "../../../Core/Color.h"
#include "../../../Core/Vector.h"

namespace RayTracer
{
	struct Photon
	{
	public:
		Photon(const Color& flux, const Vector3& position, const Vector3& direction)
		{
			this->flux = flux;
			this->position = position;
			this->direction = direction;
		}

		Photon(const Photon& photon)
		{
			this->flux = photon.flux;
			this->position = photon.position;
			this->direction = photon.direction;
		}

	public:
		Color flux;
		Vector3 position;
		Vector3 direction;
	};
}