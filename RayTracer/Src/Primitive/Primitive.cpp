#include "Primitive.h"
#include "Mesh.h"
#include "../Shader/MaterialShader.h"
#include "../Shader/ShaderConstants.h"
#include "../RayTracer/Primitive/RayTracerPrimitive.h"
#include "../Common/GlobalResource.h"
#include "../Gizmos/GizmosRenderer.h"

int ComputeOutCode(const Vector4& inputpos, const Matrix4x4& projection)
{
	Vector4 pos = projection * inputpos;
	int code = 0;
	if (pos.x < -pos.w) code |= 0x01;
	if (pos.x > pos.w) code |= 0x02;
	if (pos.y < -pos.w) code |= 0x04;
	if (pos.y > pos.w) code |= 0x08;
	if (pos.z < -pos.w) code |= 0x10;
	if (pos.z > pos.w) code |= 0x20;
	return code;
}

PrimitiveBase::PrimitiveBase(GLContext* glContext)
{
	m_realTimeRenderMesh = nullptr;
	material = nullptr;
	m_rayTracerPrimitive = nullptr;

	m_glContext = glContext;
	displayBounds = false;
}

PrimitiveBase::~PrimitiveBase()
{
	if (m_rayTracerPrimitive)
		delete m_rayTracerPrimitive;
	m_rayTracerPrimitive = nullptr;
}

void PrimitiveBase::Render(const Matrix4x4& worldToViewMatrix, const Matrix4x4& projectionMatrix)
{
	if (m_realTimeRenderMesh)
	{
		MaterialShader* renderMat = material;
		if (renderMat == nullptr)
			renderMat = GlobalResource::GetErrorShader();

		Matrix4x4 localToWorld;
		GetRenderMatrix(localToWorld);

		renderMat->SetMatrix(SHADER_CONSTANT_LOCAL_TO_WORLD, localToWorld);
		renderMat->SetMatrix(SHADER_CONSTANT_WORLD_TO_VIEW, worldToViewMatrix);
		renderMat->SetMatrix(SHADER_CONSTANT_PROJECTION, projectionMatrix);
		if (renderMat->Execute())
		{
			m_realTimeRenderMesh->Commit();
		}
	}
}

bool PrimitiveBase::Culled(const Matrix4x4& worldToViewMatrix, const Matrix4x4& projectionMatrix)
{
	Bounds bounds = GetBounds();

	Matrix4x4 mv =  projectionMatrix * worldToViewMatrix;
	int code = ComputeOutCode(Vector4(bounds.center.x + bounds.halfSize.x, bounds.center.y + bounds.halfSize.y,
		bounds.center.z + bounds.halfSize.z, 1), mv);

	code &=
		ComputeOutCode(Vector4(bounds.center.x - bounds.halfSize.x, bounds.center.y + bounds.halfSize.y,
			bounds.center.z + bounds.halfSize.z, 1), mv);

	code &=
		ComputeOutCode(Vector4(bounds.center.x + bounds.halfSize.x, bounds.center.y - bounds.halfSize.y,
			bounds.center.z + bounds.halfSize.z, 1), mv);

	code &=
		ComputeOutCode(Vector4(bounds.center.x - bounds.halfSize.x, bounds.center.y - bounds.halfSize.y,
			bounds.center.z + bounds.halfSize.z, 1), mv);

	code &=
		ComputeOutCode(Vector4(bounds.center.x + bounds.halfSize.x, bounds.center.y + bounds.halfSize.y,
			bounds.center.z - bounds.halfSize.z, 1), mv);

	code &=
		ComputeOutCode(Vector4(bounds.center.x - bounds.halfSize.x, bounds.center.y + bounds.halfSize.y,
			bounds.center.z - bounds.halfSize.z, 1), mv);

	code &=
		ComputeOutCode(Vector4(bounds.center.x + bounds.halfSize.x, bounds.center.y - bounds.halfSize.y,
			bounds.center.z - bounds.halfSize.z, 1), mv);

	code &=
		ComputeOutCode(Vector4(bounds.center.x - bounds.halfSize.x, bounds.center.y - bounds.halfSize.y,
			bounds.center.z - bounds.halfSize.z, 1), mv);

	if (code != 0) return true;

	return false;
}

void PrimitiveBase::RenderShadow(const Matrix4x4& lightSpaceMatrix, const Matrix4x4& shadowProjectionMatrix)
{
	if (m_realTimeRenderMesh)
	{
		MaterialShader* renderMat = material;
		if (renderMat == nullptr)
			renderMat = GlobalResource::GetErrorShader();

		Matrix4x4 localToWorld;
		GetRenderMatrix(localToWorld);

		renderMat->SetMatrix(SHADER_CONSTANT_LOCAL_TO_WORLD, localToWorld);
		renderMat->SetMatrix(SHADER_CONSTANT_WORLD_TO_VIEW, lightSpaceMatrix);
		renderMat->SetMatrix(SHADER_CONSTANT_PROJECTION, shadowProjectionMatrix);
		if (renderMat->ExecuteForShadow())
		{
			m_realTimeRenderMesh->Commit();
		}
	}
}

void PrimitiveBase::RenderGizmos(GizmosRenderer* gizmosRenderer)
{
	if (gizmosRenderer != nullptr)
	{
		if (displayBounds)
		{
			Bounds bounds = GetBounds();
			gizmosRenderer->SetColor(Color(0,1,0,1));
			gizmosRenderer->DrawBounds(bounds);
		}
	}
}

RayTracer::PrimitiveBase* PrimitiveBase::GetRayTracerPrimitive()
{
	UpdateRayTracerPrimitive();
	m_rayTracerPrimitive->material = material ? (RayTracer::RTMaterialShader*)(material->GetRTShader()) : nullptr;
	return m_rayTracerPrimitive;
}

Bounds PrimitiveBase::GetBounds()
{
	Bounds outBounds;
	GetBounds(outBounds);
	return outBounds;
}

CubePrimitive::CubePrimitive(GLContext* glContext) : PrimitiveBase(glContext)
{
	m_realTimeRenderMesh = GlobalResource::GetCubeMesh();
	m_rayTracerPrimitive = new RayTracer::CubePrimitive();
}

CubePrimitive::~CubePrimitive()
{
}

void CubePrimitive::GetBounds(Bounds& OutBounds)
{
	OutBounds.center = position;
	OutBounds.halfSize = scale * 0.5f;
}

void CubePrimitive::GetRenderMatrix(Matrix4x4& renderMatrix)
{
	Quaternion rotation = QUATERNION_IDENTITY;
	Matrix4x4::TRS(&renderMatrix, position, rotation, scale);
}

void CubePrimitive::UpdateRayTracerPrimitive()
{
	RayTracer::CubePrimitive* cube = (RayTracer::CubePrimitive*)m_rayTracerPrimitive;
	cube->position = position;
	cube->size = scale;
	cube->material = material ? (RayTracer::RTMaterialShader*)(material->GetRTShader()) : nullptr;
}

MeshPrimitive::MeshPrimitive(GLContext* glContext) : PrimitiveBase(glContext)
{
	m_rayTracerPrimitive = new RayTracer::MeshPrimitive();
}

MeshPrimitive::~MeshPrimitive()
{
}

void MeshPrimitive::SetMesh(Mesh* mesh)
{
	m_realTimeRenderMesh = mesh;
}

void MeshPrimitive::GetBounds(Bounds& OutBounds)
{
	if (m_realTimeRenderMesh)
	{
		Bounds bd;
		m_realTimeRenderMesh->GetBounds(bd);

		Matrix4x4 ltw;
		GetRenderMatrix(ltw);
		int i, j;
		for (i = 0; i < 3; i++)
		{
			OutBounds.center[i] = ltw.TransposeIndex(12 + i);
			OutBounds.halfSize[i] = 0.0f;
			for (j = 0; j < 3; j++)
			{
				OutBounds.center[i] += ltw.TransposeIndex(j * 4 + i) * bd.center[j];
				OutBounds.halfSize[i] += fabsf(ltw.TransposeIndex(j * 4 + i)) * bd.halfSize[j];
			}
		}
	}
}

void MeshPrimitive::GetRenderMatrix(Matrix4x4& renderMatrix)
{
	Quaternion rotation;
	Quaternion::Euler(&rotation, euler);
	Matrix4x4::TRS(&renderMatrix, position, rotation, scale);
}

void MeshPrimitive::UpdateRayTracerPrimitive()
{
	RayTracer::MeshPrimitive* meshprimitive = (RayTracer::MeshPrimitive*)m_rayTracerPrimitive;
	meshprimitive->mesh = m_realTimeRenderMesh;
	meshprimitive->material = material ? (RayTracer::RTMaterialShader*)(material->GetRTShader()) : nullptr;
	Matrix4x4 localToWorld;
	GetRenderMatrix(localToWorld);
	meshprimitive->localToWorld = localToWorld;
}

SpherePrimitive::SpherePrimitive(GLContext* glContext) : PrimitiveBase(glContext)
{
	m_realTimeRenderMesh = GlobalResource::GetSphereMesh();
	m_rayTracerPrimitive = new RayTracer::SpherePrimitive();
}

SpherePrimitive::~SpherePrimitive()
{
}

void SpherePrimitive::GetBounds(Bounds& OutBounds)
{
	float diameter = Math::Max(Math::Max(scale.x, scale.y), scale.z);
	Vector3 size = Vector3(diameter, diameter, diameter);
	OutBounds.center = position;
	OutBounds.halfSize = size * 0.5f;
}

void SpherePrimitive::GetRenderMatrix(Matrix4x4& renderMatrix)
{
	Quaternion rotation = QUATERNION_IDENTITY;
	float diameter = Math::Max(Math::Max(scale.x, scale.y), scale.z);
	Vector3 size = Vector3(diameter, diameter, diameter);
	Matrix4x4::TRS(&renderMatrix, position, rotation, size);
}

void SpherePrimitive::UpdateRayTracerPrimitive()
{
	RayTracer::SpherePrimitive* sphere = (RayTracer::SpherePrimitive*)m_rayTracerPrimitive;
	sphere->position = position;
	sphere->radius = Math::Max(Math::Max(scale.x, scale.y), scale.z) * 0.5f;
	sphere->material = material ? (RayTracer::RTMaterialShader*)(material->GetRTShader()) : nullptr;
}

PlanePrimitive::PlanePrimitive(GLContext* glContext) : PrimitiveBase(glContext)
{
	m_realTimeRenderMesh = GlobalResource::GetPlaneMesh();
	m_rayTracerPrimitive = new RayTracer::PlanePrimitive();
}

PlanePrimitive::~PlanePrimitive()
{
}

void PlanePrimitive::GetBounds(Bounds& OutBounds)
{
	Bounds bd = Bounds(Vector3(0,0,0), Vector3(10,0.001,10));

	Matrix4x4 ltw;
	GetRenderMatrix(ltw);
	int i, j;
	for (i = 0; i < 3; i++)
	{
		OutBounds.center[i] = ltw.TransposeIndex(12 + i);
		OutBounds.halfSize[i] = 0.0f;
		for (j = 0; j < 3; j++)
		{
			OutBounds.center[i] += ltw.TransposeIndex(j * 4 + i) * bd.center[j];
			OutBounds.halfSize[i] += abs(ltw.TransposeIndex(j * 4 + i)) * bd.halfSize[j];
		}
	}
}

void PlanePrimitive::GetRenderMatrix(Matrix4x4& renderMatrix)
{
	Quaternion rotation;
	Quaternion::Euler(&rotation, euler);
	Matrix4x4::TRS(&renderMatrix, position, rotation, scale);
}

void PlanePrimitive::UpdateRayTracerPrimitive()
{
	double cosx = cos(euler.x * 0.01745329252 * 0.5);
	double cosy = cos(euler.y * 0.01745329252 * 0.5);
	double cosz = cos(euler.z * 0.01745329252 * 0.5);

	double sinx = sin(euler.x * 0.01745329252 * 0.5);
	double siny = sin(euler.y * 0.01745329252 * 0.5);
	double sinz = sin(euler.z * 0.01745329252 * 0.5);

	double rx = cosy * sinx * cosz + siny * cosx * sinz;
	double ry = siny * cosx * cosz - cosy * sinx * sinz;
	double rz = cosy * cosx * sinz - siny * sinx * cosz;
	double rw = cosy * cosx * cosz + siny * sinx * sinz;

	double x2 = 2.0 * rx * rx;
	double y2 = 2.0 * ry * ry;
	double z2 = 2.0 * rz * rz;
	double xy = 2.0 * rx * ry;
	double xz = 2.0 * rx * rz;
	double xw = 2.0 * rx * rw;
	double yz = 2.0 * ry * rz;
	double yw = 2.0 * ry * rw;
	double zw = 2.0 * rz * rw;

	double ra = 1.0 - y2 - z2;
	double rb = xy + zw;
	double rc = xz - yw;
	double rd = xy - zw;
	double re = 1.0 - x2 - z2;
	double rf = yz + xw;
	double rg = xz + yw;
	double rh = yz - xw;
	double ri = 1.0 - x2 - y2;

	Vector3 right = Vector3(ra, rb, rc);
	Vector3 up = Vector3(rd, re, rf);
	Vector3 forward = Vector3(rg, rh, ri);

	Vector3 size = scale * 10.0;

	RayTracer::PlanePrimitive* plane = (RayTracer::PlanePrimitive*)m_rayTracerPrimitive;
	plane->material = material ? (RayTracer::RTMaterialShader*)(material->GetRTShader()) : nullptr;
	plane->position = position - right * size.x * 0.5 - forward * size.z * 0.5;
	plane->right = right * size.x;
	plane->up = forward * size.z;
	plane->normal = up;
	plane->heightSquared = plane->up.SqrMagnitude();
	plane->widthSquared = plane->right.SqrMagnitude();
}
