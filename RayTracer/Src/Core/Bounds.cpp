#include "Bounds.h"

Bounds::Bounds(double maxX, double maxY, double maxZ, double minX, double minY, double minZ)
{
	halfSize = Vector3(maxX - minX, maxY - minY, maxZ - minZ) * 0.5;
	center = Vector3(minX + halfSize.x, minY + halfSize.y, minZ + halfSize.z);
}

Bounds::Bounds(const Bounds& bounds)
{
	center = bounds.center;
	halfSize = bounds.halfSize;
}

Bounds::Bounds(Vector3 center, Vector3 size)
{
	this->center = center;
	this->halfSize = size * 0.5;
}

Bounds::Bounds() : Bounds(Vector3(0.0, 0.0, 0.0), Vector3(0.0, 0.0, 0.0))
{
}

void Bounds::GetMin(Vector3& min) const
{
	min.x = Math::Min(center.x + halfSize.x, center.x - halfSize.x);
	min.y = Math::Min(center.y + halfSize.y, center.y - halfSize.y);
	min.z = Math::Min(center.z + halfSize.z, center.z - halfSize.z);
}

Vector3 Bounds::GetMin() const
{
	Vector3 out;
	GetMin(out);
	return out;
}

void Bounds::GetMax(Vector3& max) const
{
	max.x = Math::Max(center.x + halfSize.x, center.x - halfSize.x);
	max.y = Math::Max(center.y + halfSize.y, center.y - halfSize.y);
	max.z = Math::Max(center.z + halfSize.z, center.z - halfSize.z);
}

Vector3 Bounds::GetMax() const
{
	Vector3 out;
	GetMax(out);
	return out;
}

void Bounds::GetSize(Vector3& size) const
{
	size.x = halfSize.x * 2;
	size.y = halfSize.y * 2;
	size.z = halfSize.z * 2;
}

Vector3 Bounds::GetSize() const
{
	Vector3 out;
	GetSize(out);
	return out;
}

void Bounds::GetClosestPoint(const Vector3& point, Vector3& out) const
{
	out.x = Math::Clamp<double>(point.x, center.x - halfSize.x, center.x + halfSize.x);
	out.y = Math::Clamp<double>(point.y, center.y - halfSize.y, center.y + halfSize.y);
	out.z = Math::Clamp<double>(point.z, center.z - halfSize.z, center.z + halfSize.z);
}

Vector3 Bounds::GetClosestPoint(const Vector3& point) const
{
	Vector3 out;
	GetClosestPoint(point, out);
	return out;
}

double Bounds::GetDistance(const Vector3& point) const
{
	double dis2 = GetDistance2(point);
	return sqrt(dis2);
}

double Bounds::GetDistance2(const Vector3& point) const
{	
	Vector3 closePos;
	GetClosestPoint(point, closePos);
	return (point - closePos).SqrMagnitude();
}

bool Bounds::Contains(const Vector3& point) const
{
	if (point.x<center.x - halfSize.x || point.x>center.x + halfSize.x)
		return false;
	if (point.y<center.y - halfSize.y || point.y>center.y + halfSize.y)
		return false;
	if (point.z<center.z - halfSize.z || point.z>center.z + halfSize.z)
		return false;
	return true;
}

bool Bounds::Contains(const Bounds& bounds) const
{
	if (!Contains(bounds.center + Vector3(-bounds.halfSize.x, bounds.halfSize.y, -bounds.halfSize.z)))
		return false;
	if (!Contains(bounds.center + Vector3(bounds.halfSize.x, bounds.halfSize.y, -bounds.halfSize.z)))
		return false;
	if (!Contains(bounds.center + Vector3(bounds.halfSize.x, bounds.halfSize.y, bounds.halfSize.z)))
		return false;
	if (!Contains(bounds.center + Vector3(-bounds.halfSize.x, bounds.halfSize.y, bounds.halfSize.z)))
		return false;
	if (!Contains(bounds.center + Vector3(-bounds.halfSize.x, -bounds.halfSize.y, -bounds.halfSize.z)))
		return false;
	if (!Contains(bounds.center + Vector3(bounds.halfSize.x, -bounds.halfSize.y, -bounds.halfSize.z)))
		return false;
	if (!Contains(bounds.center + Vector3(bounds.halfSize.x, -bounds.halfSize.y, bounds.halfSize.z)))
		return false;
	if (!Contains(bounds.center + Vector3(-bounds.halfSize.x, -bounds.halfSize.y, bounds.halfSize.z)))
		return false;
	return true;
}

void Bounds::Encapsulate(const Vector3& point)
{
	Vector3 minV = GetMin();
	Vector3 maxV = GetMax();

	minV.x = Math::Min(minV.x, point.x);
	minV.y = Math::Min(minV.y, point.y);
	minV.z = Math::Min(minV.z, point.z);

	maxV.x = Math::Max(maxV.x, point.x);
	maxV.y = Math::Max(maxV.y, point.y);
	maxV.z = Math::Max(maxV.z, point.z);

	halfSize = Vector3(maxV.x - minV.x, maxV.y - minV.y, maxV.z - minV.z) * 0.5;
	center = Vector3(minV.x + halfSize.x, minV.y + halfSize.y, minV.z + halfSize.z);
}

void Bounds::Encapsulate(const Bounds& bounds)
{
	Vector3 minA = GetMin();
	Vector3 maxA = GetMax();
	Vector3 minB = bounds.GetMin();
	Vector3 maxB = bounds.GetMax();

	minA.x = Math::Min(minA.x, minB.x);
	minA.y = Math::Min(minA.y, minB.y);
	minA.z = Math::Min(minA.z, minB.z);

	maxA.x = Math::Max(maxA.x, maxB.x);
	maxA.y = Math::Max(maxA.y, maxB.y);
	maxA.z = Math::Max(maxA.z, maxB.z);

	halfSize = Vector3(maxA.x - minA.x, maxA.y - minA.y, maxA.z - minA.z) * 0.5;
	center = Vector3(minA.x + halfSize.x, minA.y + halfSize.y, minA.z + halfSize.z);
}

void Bounds::Expand(double amound)
{
	halfSize = halfSize * amound;
}

bool Bounds::Raycast(const Ray& ray, double& distance)
{
	distance = 0.0;
	double tmax = DBL_MAX;

	Vector3 min = GetMin();
	Vector3 max = GetMax();

	for (int i = 0; i < 3; i++)
	{
		if (abs(ray.direction[i]) < DBL_EPSILON)
		{
			if (ray.origin[i] < min[i] || ray.origin[i] > max[i])
				return false;
		}
		else
		{
			double ood = 1.0 / ray.direction[i];
			double t1 = (min[i] - ray.origin[i]) * ood;
			double t2 = (max[i] - ray.origin[i]) * ood;
			if (t1 > t2)
			{
				double t = t2;
				t2 = t1;
				t1 = t;
			}

			if (t1 > distance)
			{
				distance = t1;
			}

			if (t2 < tmax)
			{
				tmax = t2;
			}

			if (distance > tmax)
				return false;
		}
	}
	return true;
}

bool Bounds::Raycast(const Ray& ray)
{
	double dis = 0.0;
	return Raycast(ray, dis);
}

bool Bounds::Interact(const Bounds& bounds)
{
	Vector3 bMax, bMin, cMax, cMin;
	bounds.GetMax(bMax);
	bounds.GetMin(bMin);
	GetMax(cMax);
	GetMin(cMin);
	if (bMax.x < cMin.x || bMin.x > cMax.x) return false;
	if (bMax.y < cMin.y || bMin.y > cMax.y) return false;
	if (bMax.z < cMin.z || bMin.z > cMax.z) return false;
	return true;
}

bool Bounds::InteractWithSphere(const SphereBounds& sphere)
{
	Vector3 closest;
	Vector3 point = sphere.center;
	GetClosestPoint(point, closest);

	double dis = Vector3::Distance(point, closest);
	if (dis > sphere.radius)
		return false;
	return true;
}

SphereBounds::SphereBounds(const SphereBounds& sphere)
{
	this->center = sphere.center;
	this->radius = sphere.radius;
}

SphereBounds::SphereBounds(Vector3 center, double radius)
{
	this->center = center;
	this->radius = radius;
}

SphereBounds::SphereBounds() : SphereBounds(Vector3(0, 0, 0), 0.0)
{
}

void SphereBounds::GetBounds(Bounds& outBounds) const
{
	Vector3 center = this->center;
	Vector3 size = this->radius * Vector3(2, 2, 2);

	outBounds = Bounds(center, size);
}

void SphereBounds::GetClosestPoint(const Vector3& point, Vector3& out) const
{
	double dis = Vector3::Distance(point, center);
	if (dis < radius)
		out = point;
	else
	{
		Vector3 dir;
		(point - center).GetNormalized(dir);
		out = center + dir * dis;
	}
}

bool SphereBounds::Contains(const Vector3& point) const
{
	double dis = Vector3::Distance(point, center);
	if (dis < radius)
		return true;
	return false;
}

bool SphereBounds::Raycast(const Ray& ray, double& distance)
{
	distance = 0.0;

	Vector3 tocenter = ray.origin - this->center;

	double vala = Vector3::Dot(ray.direction, ray.direction);
	double valb = Vector3::Dot(tocenter, ray.direction) * 2.0;
	double valc = Vector3::Dot(tocenter, tocenter) - radius * radius;

	double dis = valb * valb - 4.0 * vala * valc;


	if (dis < 0.0)
		return false;
	else
	{
		double e = sqrt(dis);
		double denom = 2.0 * vala;
		double t = (-valb - e) / denom;

		if (t > DBL_EPSILON)
		{
			distance = t;
			return true;
		}


		t = (-valb + e) / denom;

		if (t > DBL_EPSILON)
		{
			distance = t;
			return true;
		}
	}

	return false;
}

bool SphereBounds::Raycast(const Ray& ray)
{
	double distance = 0.0;
	return Raycast(ray, distance);
}

bool SphereBounds::Interact(const SphereBounds& sphere)
{
	double dis = Vector3::Distance(sphere.center, center);
	if (dis < radius + sphere.radius)
		return true;
	return false;
}

bool SphereBounds::InteractWithBounds(const Bounds& bounds)
{
	Vector3 closest;
	bounds.GetClosestPoint(center, closest);

	double dis = Vector3::Distance(center, closest);
	if (dis > radius)
		return false;
	return true;
}