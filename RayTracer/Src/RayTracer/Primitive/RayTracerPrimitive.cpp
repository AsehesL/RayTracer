#include <float.h>
#include "RayTracerPrimitive.h"
#include "../SceneData/SceneDataBase.h"
#include "../../Primitive/Mesh.h"

RayTracer::PrimitiveBase::PrimitiveBase()
{
	material = nullptr;
}

RayTracer::PrimitiveBase::~PrimitiveBase()
{
}

RayTracer::CubePrimitive::CubePrimitive() : PrimitiveBase()
{
}

RayTracer::CubePrimitive::~CubePrimitive()
{
}

void RayTracer::CubePrimitive::AddToScene(SceneDataBase* sceneData)
{
	if (sceneData != nullptr)
		sceneData->AddPrimitive(this);
}

bool RayTracer::CubePrimitive::Tracing(const Ray& ray, RayTracingResult& result)
{
	double tmin = 0.0;
	double tmax = result.distance;

	Vector3 min = position - size * 0.5;
	Vector3 max = position + size * 0.5;
	Vector3 normal;

	for (int i = 0; i < 3; i++)
	{
		Vector3 n = Vector3(i == 0 ? -1 : 0, i == 1 ? -1 : 0, i == 2 ? -1 : 0);
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
				n *= -1;
			}

			if (t1 > tmin)
			{
				tmin = t1;
				normal = n;
			}

			if (t2 < tmax)
			{
				tmax = t2;
			}

			if (tmin > tmax)
				return false;
		}
	}

	//if (tmin > -double.Epsilon && tmin < double.Epsilon)
	//    return false;
	result.distance = tmin - DBL_EPSILON * 2;
	result.material = material;
	result.hit = ray.origin + ray.direction * result.distance;
	result.texcoord = Vector2(0, 0); //Cube射线检测uv，懒得实现 o(*￣3￣)o 
	result.normal = normal;
	return true;
}

void RayTracer::CubePrimitive::GetBounds(Bounds& bounds)
{
	bounds.center = position;
	bounds.halfSize = size * 0.5;
}

RayTracer::SpherePrimitive::SpherePrimitive() : PrimitiveBase()
{
	radius = 0.0;
}

RayTracer::SpherePrimitive::~SpherePrimitive()
{
}

void RayTracer::SpherePrimitive::AddToScene(SceneDataBase* sceneData)
{
	if (sceneData != nullptr)
		sceneData->AddPrimitive(this);
}

bool RayTracer::SpherePrimitive::Tracing(const Ray& ray, RayTracingResult& result)
{
	Vector3 tocenter = ray.origin - this->position;

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

		if (t > DBL_EPSILON && t <= result.distance)
		{

			result.distance = t;
			result.normal = (tocenter + ray.direction * result.distance) / radius;
			result.hit = ray.origin + ray.direction * result.distance;
			result.material = material;

			return true;
		}


		t = (-valb + e) / denom;

		if (t > DBL_EPSILON && t <= result.distance)
		{

			result.distance = t;
			result.normal = (tocenter + ray.direction * result.distance) / radius;
			result.hit = ray.origin + ray.direction * result.distance;
			result.material = material;

			return true;
		}
	}

	return false;
}

void RayTracer::SpherePrimitive::GetBounds(Bounds& bounds)
{
	bounds.center = position;
	bounds.halfSize = Vector3(radius, radius, radius);
}

RayTracer::MeshPrimitive::MeshPrimitive() : PrimitiveBase()
{
	mesh = nullptr;
}

RayTracer::MeshPrimitive::~MeshPrimitive()
{
	for (int i = 0; i < m_triangles.size(); i++)
	{
		TrianglePrimitive* triangle = m_triangles[i];
		delete triangle;
	}
	m_triangles.clear();
}

void RayTracer::MeshPrimitive::AddToScene(SceneDataBase* sceneData)
{
	for (int i = 0; i < m_triangles.size(); i++)
	{
		TrianglePrimitive* triangle = m_triangles[i];
		delete triangle;
	}
	m_triangles.clear();
	if (sceneData != nullptr && mesh != nullptr)
	{
		unsigned int indexCount = mesh->GetIndexCount();
		for (unsigned int i = 0; i < indexCount; i += 3)
		{
			Vector3 pos0 = mesh->GetVertex(i);
			Vector3 pos1 = mesh->GetVertex(i + 1);
			Vector3 pos2 = mesh->GetVertex(i + 2);

			pos0 = localToWorld.TransformPoint(pos0);
			pos1 = localToWorld.TransformPoint(pos1);
			pos2 = localToWorld.TransformPoint(pos2);

			Vector3 normal0 = mesh->GetNormal(i);
			Vector3 normal1 = mesh->GetNormal(i + 1);
			Vector3 normal2 = mesh->GetNormal(i + 2);

			normal0 = localToWorld.TransformVector(normal0);
			normal1 = localToWorld.TransformVector(normal1);
			normal2 = localToWorld.TransformVector(normal2);

			Vector2 uv0 = mesh->GetUV(0, i);
			Vector2 uv1 = mesh->GetUV(0, i + 1);
			Vector2 uv2 = mesh->GetUV(0, i + 2);

			Vector4 tangent0 = mesh->GetTangent(i);
			Vector4 tangent1 = mesh->GetTangent(i + 1);
			Vector4 tangent2 = mesh->GetTangent(i + 2);

			Vector3 worldTangent0 = localToWorld.TransformVector(Vector3(tangent0.x, tangent0.y, tangent0.z));
			Vector3 worldTangent1 = localToWorld.TransformVector(Vector3(tangent1.x, tangent1.y, tangent1.z));
			Vector3 worldTangent2 = localToWorld.TransformVector(Vector3(tangent2.x, tangent2.y, tangent2.z));

			tangent0.x = worldTangent0.x;
			tangent0.y = worldTangent0.y;
			tangent0.z = worldTangent0.z;

			tangent1.x = worldTangent1.x;
			tangent1.y = worldTangent1.y;
			tangent1.z = worldTangent1.z;

			tangent2.x = worldTangent2.x;
			tangent2.y = worldTangent2.y;
			tangent2.z = worldTangent2.z;

			TrianglePrimitive* triangle = new TrianglePrimitive();
			triangle->SetVertex(0, pos0, normal0, uv0, tangent0);
			triangle->SetVertex(1, pos1, normal1, uv1, tangent1);
			triangle->SetVertex(2, pos2, normal2, uv2, tangent2);
			triangle->material = material;

			m_triangles.push_back(triangle);

			triangle->AddToScene(sceneData);
		}
	}
}

bool RayTracer::MeshPrimitive::Tracing(const Ray& ray, RayTracingResult& result)
{
	return false;
}

void RayTracer::MeshPrimitive::GetBounds(Bounds& bounds)
{

}

RayTracer::TrianglePrimitive::TrianglePrimitive() : PrimitiveBase()
{
}

RayTracer::TrianglePrimitive::~TrianglePrimitive()
{
}

void RayTracer::TrianglePrimitive::SetVertex(int index, Vector3 position, Vector3 normal, Vector2 uv, Vector4 tangent)
{
	if (index == 0)
	{
		vertex0.position = position;
		vertex0.normal = normal;
		vertex0.uv = uv;
		vertex0.tangent = tangent;
	}
	else if (index == 1)
	{
		vertex1.position = position;
		vertex1.normal = normal;
		vertex1.uv = uv;
		vertex1.tangent = tangent;
	}
	else if (index == 2)
	{
		vertex2.position = position;
		vertex2.normal = normal;
		vertex2.uv = uv;
		vertex2.tangent = tangent;
	}
}

void RayTracer::TrianglePrimitive::AddToScene(SceneDataBase* sceneData)
{
	if (sceneData)
		sceneData->AddPrimitive(this);
}

bool RayTracer::TrianglePrimitive::Tracing(const Ray& ray, RayTracingResult& result)
{
	double rt = 0.0;

	Vector3 e1 = this->vertex1.position - this->vertex0.position;
	Vector3 e2 = this->vertex2.position - this->vertex0.position;

	double v = 0;
	double u = 0;

	Vector3 n = Vector3::Cross(e1, e2);
	double ndv = Vector3::Dot(ray.direction, n);
	//bool back = false;
	if (ndv > 0)
	{
		//if (shader != null && shader.ShouldRenderBackFace() == false)
		//	return false;
		//back = true;
		return false;
	}

	Vector3 p = Vector3::Cross(ray.direction, e2);

	double det = Vector3::Dot(e1, p);
	Vector3 t;
	if (det > 0)
	{
		t = ray.origin - this->vertex0.position;
	}
	else
	{
		t = this->vertex0.position - ray.origin;
		det = -det;
	}
	if (det < DBL_EPSILON)
	{
		return false;
	}

	u = Vector3::Dot(t, p);
	if (u < 0.0 || u > det)
		return false;

	Vector3 q = Vector3::Cross(t, e1);

	v = Vector3::Dot(ray.direction, q);
	if (v < 0.0 || u + v > det)
		return false;

	rt = Vector3::Dot(e2, q);

	double finvdet = 1.0 / det;
	rt *= finvdet;
	if (rt < 0.001)
		return false;
	if (rt > result.distance)
		return false;
	u *= finvdet;
	v *= finvdet;

	result.hit = ray.origin + ray.direction * rt;
	result.texcoord = (1.0 - u - v) * vertex0.uv + u * vertex1.uv + v * vertex2.uv;
	result.normal = (1.0 - u - v) * vertex0.normal + u * vertex1.normal + v * vertex2.normal;
	result.tangent = (1.0 - u - v) * vertex0.tangent + u * vertex1.tangent + v * vertex2.tangent;
	result.material = material;
	result.distance = rt;
	//if (back)
	//	hit.normal = -1.0 * hit.normal;
	return true;
}

void RayTracer::TrianglePrimitive::GetBounds(Bounds& bounds)
{
	Vector3 min = vertex0.position;
	Vector3 max = vertex0.position;

	min = Vector3::Min(min, vertex1.position);
	max = Vector3::Max(max, vertex1.position);

	min = Vector3::Min(min, vertex2.position);
	max = Vector3::Max(max, vertex2.position);

	Vector3 si = max - min;
	Vector3 ct = min + si * 0.5;

	if (si.x <= 0)
		si.x = 0.1;
	if (si.y <= 0)
		si.y = 0.1;
	if (si.z <= 0)
		si.z = 0.1;

	bounds = Bounds(ct, si);
}