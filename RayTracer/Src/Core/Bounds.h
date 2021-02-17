#pragma once
#include "Ray.h"

struct SphereBounds;
struct Bounds;

typedef struct Bounds
{
public:
	Bounds(double maxX, double maxY, double maxZ, double minX, double minY, double minZ);
	Bounds(const Bounds& bounds);
	Bounds(Vector3 center, Vector3 size);
	Bounds();
	void GetMin(Vector3& min) const;
	Vector3 GetMin() const;
	void GetMax(Vector3& max) const;
	Vector3 GetMax() const;
	void GetSize(Vector3& size) const;
	Vector3 GetSize() const;
	void GetClosestPoint(const Vector3& point, Vector3& out) const;
	Vector3 GetClosestPoint(const Vector3& point) const;
	bool Contains(const Vector3& point) const;
	bool Contains(const Bounds& bounds) const;
	void Encapsulate(const Vector3& point);
	void Encapsulate(const Bounds& bounds);
	void Expand(double amound);
	bool Raycast(const Ray& ray, double& distance);
	bool Raycast(const Ray& ray);
	bool Interact(const Bounds& bounds);
	bool InteractWithSphere(const SphereBounds& sphere);

public:
	Vector3 center;
	Vector3 halfSize;
} Bounds;

typedef struct SphereBounds
{
public:
	SphereBounds(const SphereBounds& sphere);
	SphereBounds(Vector3 center, double radius);
	SphereBounds();
	void GetBounds(Bounds& outBounds) const;
	void GetClosestPoint(const Vector3& point, Vector3& out) const;
	bool Contains(const Vector3& point) const;
	bool Raycast(const Ray& ray, double& distance);
	bool Raycast(const Ray& ray);
	bool Interact(const SphereBounds& sphere);
	bool InteractWithBounds(const Bounds& bounds);

public:
	Vector3 center;
	double radius;
} SphereBounds;