#pragma once
#include "Vector.h"

typedef struct Ray
{
public:
	Ray(Vector3 origin, Vector3 direction)
	{
		this->origin = origin;
		direction.GetNormalized(this->direction);
	}

	Ray()
	{
		origin = Vector3(0,0,0);
		direction = Vector3(0,0,0);
	}

	Ray(const Ray& ray) : Ray(ray.origin, ray.direction)
	{
	}
	
	void GetPoint(double t, Vector3& out) const
	{
		out.x = origin.x + t * direction.x;
		out.y = origin.y + t * direction.y;
		out.z = origin.z + t * direction.z;
	}

	Vector3 GetPoint(double t) const
	{
		return Vector3(origin.x + t * direction.x, origin.y + t * direction.y, origin.z + t * direction.z);
	}

public:
	Vector3 origin;
	Vector3 direction;
} Ray;
