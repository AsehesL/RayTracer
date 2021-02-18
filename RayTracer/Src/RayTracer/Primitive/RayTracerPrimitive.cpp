#include <float.h>
#include "RayTracerPrimitive.h"
#include "../SceneData/SceneDataBase.h"
#include "../../Primitive/Mesh.h"
#include "../Sampler/Sampler.h"

RayTracer::PrimitiveBase::PrimitiveBase()
{
	material = nullptr;
	m_area = 0;
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
	result.distance = tmin;
	result.material = material;
	result.hit = ray.origin + ray.direction * result.distance;
	result.texcoord = Vector2(0, 0); //Cube射线检测uv，懒得实现 o(*￣3￣)o 
	result.normal = normal;
	result.hit += normal * DBL_EPSILON;
	return true;
}

void RayTracer::CubePrimitive::GetBounds(Bounds& bounds)
{
	bounds.center = position;
	bounds.halfSize = size * 0.5;
}

void RayTracer::CubePrimitive::RefreshArea()
{
	float a = size.x * size.y;
	float b = size.x * size.z;
	float c = size.y * size.z;
	m_area = (a + b + c) * 2.0f;
}

Vector3 RayTracer::CubePrimitive::GetSurfacePosition(SamplerBase* sampler, Vector2 pos)
{
	int face = (int)round(5.0f*sampler->GetRandom());
	if (face == 0)
		return Vector3(position.x + size.x * 0.5f, position.y + size.y * (pos.x-0.5f), position.z + size.z * (pos.y-0.5f));
	if (face == 1)
		return Vector3(position.x - size.x * 0.5f, position.y + size.y * (pos.x - 0.5f), position.z + size.z * (pos.y - 0.5f));
	if (face == 2)
		return Vector3(position.x + size.x * (pos.x - 0.5f), position.y + size.y * 0.5f, position.z + size.z * (pos.y - 0.5f));
	if (face == 3)
		return Vector3(position.x + size.x * (pos.x - 0.5f), position.y - size.y * 0.5f, position.z + size.z * (pos.y - 0.5f));
	if (face == 4)
		return Vector3(position.x + size.x * (pos.x - 0.5f), position.y + size.y * (pos.y - 0.5f), position.z + size.z * 0.5f);
	if (face == 5)
		return Vector3(position.x + size.x * (pos.x - 0.5f), position.y + size.y * (pos.y - 0.5f), position.z - size.z * 0.5f);
	return Vector3();
}

Vector3 RayTracer::CubePrimitive::GetSurfaceNormal(Vector3 pos)
{
	if (pos.x - position.x <= size.x * 0.5f && pos.x - position.x >= -size.x * 0.5f && pos.z - position.z <= size.z * 0.5f && pos.z - position.z >= -size.z * 0.5f)
	{
		if (pos.y > position.y)
			return Vector3(0,1,0);
		else
			return Vector3(0,-1,0);
	}
	if (pos.x - position.x <= size.x * 0.5f && pos.x - position.x >= -size.x * 0.5f && pos.y - position.y <= size.y * 0.5f && pos.y - position.y >= -size.y * 0.5f)
	{
		if (pos.z > position.z)
			return Vector3(0, 0, 1);
		else
			return Vector3(0, 0, -1);
	}
	if (pos.z - position.z <= size.z * 0.5f && pos.z - position.z >= -size.z * 0.5f && pos.y - position.y <= size.y * 0.5f && pos.y - position.y >= -size.y * 0.5f)
	{
		if (pos.x > position.x)
			return Vector3(1, 0, 0);
		else
			return Vector3(-1, 0, 0);
	}
	return Vector3();
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
	{
		sceneData->AddPrimitive(this);
	}
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

void RayTracer::SpherePrimitive::RefreshArea()
{
	m_area = 2.0f * Math::PI * 2.0f * radius * radius;
}

Vector3 RayTracer::SpherePrimitive::GetSurfacePosition(SamplerBase* sampler, Vector2 pos)
{

	float r = 2.0f * sqrt(pos.y * (1 - pos.y));
	float x = cos(2.0f * Math::PI * pos.x) * r;
	float y = sin(2.0f * Math::PI * pos.x) * r;
	float z = 1.0f - 2.0f * pos.y;

	return position + radius * Vector3(x, y, z);
}

Vector3 RayTracer::SpherePrimitive::GetSurfaceNormal(Vector3 pos)
{
	return (pos - position) / radius;
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
			triangle->SetVertex(pos0,pos1,pos2);
			triangle->SetNormal(normal0,normal1,normal2);
			triangle->SetUV(uv0,uv1,uv2);
			triangle->SetTangent(tangent0,tangent1,tangent2);
			triangle->material = material;

			m_triangles.push_back(triangle);

			triangle->RefreshArea();
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

Vector3 RayTracer::MeshPrimitive::GetSurfacePosition(SamplerBase* sampler, Vector2 pos)
{
	return Vector3();
}

Vector3 RayTracer::MeshPrimitive::GetSurfaceNormal(Vector3 pos)
{
	return Vector3();
}

RayTracer::TrianglePrimitive::TrianglePrimitive() : PrimitiveBase()
{
}

RayTracer::TrianglePrimitive::~TrianglePrimitive()
{
}

void RayTracer::TrianglePrimitive::SetVertex(const Vector3& position0, const Vector3& position1, const Vector3& position2)
{
	vertex0.position = position0;
	vertex1.position = position1;
	vertex2.position = position2;

	m_area = Vector3::Cross(position2 - position1, position2 - position0).Magnitude() / 2.0;
}

void RayTracer::TrianglePrimitive::SetNormal(const Vector3& normal0, const Vector3& normal1, const Vector3& normal2)
{
	vertex0.normal = normal0;
	vertex1.normal = normal1;
	vertex2.normal = normal2;
}

void RayTracer::TrianglePrimitive::SetUV(const Vector2& uv0, const Vector2& uv1, const Vector2& uv2)
{
	vertex0.uv = uv0;
	vertex1.uv = uv1;
	vertex2.uv = uv2;
}

void RayTracer::TrianglePrimitive::SetTangent(const Vector4& tangent0, const Vector4& tangent1, const Vector4& tangent2)
{
	vertex0.tangent = tangent0;
	vertex1.tangent = tangent1;
	vertex2.tangent = tangent2;
}

void RayTracer::TrianglePrimitive::AddToScene(SceneDataBase* sceneData)
{
	if (sceneData)
	{
		sceneData->AddPrimitive(this);
	}
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

void RayTracer::TrianglePrimitive::RefreshArea()
{
	m_area = Vector3::Cross(vertex2.position - vertex1.position, vertex2.position - vertex0.position).Magnitude() / 2.0;
}

Vector3 RayTracer::TrianglePrimitive::GetSurfacePosition(SamplerBase* sampler, Vector2 pos)
{
	double u = pos.x;
	double v = pos.y;
	return (1.0 - u - v) * vertex0.position + u * vertex1.position + v * vertex2.position;
}

Vector3 RayTracer::TrianglePrimitive::GetSurfaceNormal(Vector3 pos)
{
	return 1.0f / 3.0f * (vertex0.normal + vertex1.normal + vertex2.normal);
}
