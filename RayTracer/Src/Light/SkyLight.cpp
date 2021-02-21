#include "SkyLight.h"
#include "../Primitive/Mesh.h"
#include "../Shader/SkyShader.h"
#include "../Shader/SkyLightToSHComputeShader.h"
#include "../Shader/ShaderConstants.h"
#include "../Common/GlobalResource.h"
#include "../Texture/RenderTexture.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../RealtimeRender/GraphicsLib/RenderBuffer.h"
#include "../RayTracer/Shader/RayTracerSkyShader.h"

SkyLight::SkyLight(GLContext* glContext)
{
	m_skySphereMesh = GlobalResource::GetSkySphereMesh();
	m_skyLightToSHComputeShader = GlobalResource::CreateComputeShader<SkyLightToSHComputeShader>();
	m_shader = nullptr;

	m_cubeMap = nullptr;

	m_glContext = glContext;

	m_isCubeMapDirty = false;
}

SkyLight::~SkyLight()
{
}

void SkyLight::Render(const Vector3& cameraPosition, const Matrix4x4& worldToViewMatrix, const Matrix4x4& projectionMatrix)
{
	if (m_skySphereMesh && m_shader)
	{
		Matrix4x4 localToWorld;
		Matrix4x4::Translate(&localToWorld, cameraPosition.x, cameraPosition.y, cameraPosition.z);
		m_shader->SetMatrix(SHADER_CONSTANT_LOCAL_TO_WORLD, localToWorld);
		m_shader->SetMatrix(SHADER_CONSTANT_WORLD_TO_VIEW, worldToViewMatrix);
		m_shader->SetMatrix(SHADER_CONSTANT_PROJECTION, projectionMatrix);

		if (m_shader->Execute())
		{
			m_skySphereMesh->Commit();
		}
	}
}

CubeMapRenderTexture* SkyLight::GetCubeMap()
{
	if (m_skySphereMesh != nullptr && m_shader != nullptr && IsCubeMapDirty())
	{
		if (m_cubeMap != nullptr)
		{
			delete m_cubeMap;
			m_cubeMap = nullptr;
		}
		m_cubeMap = new CubeMapRenderTexture(256, m_glContext);
		RenderCubemapFace(m_cubeMap, 0);
		RenderCubemapFace(m_cubeMap, 1);
		RenderCubemapFace(m_cubeMap, 2);
		RenderCubemapFace(m_cubeMap, 3);
		RenderCubemapFace(m_cubeMap, 4);
		RenderCubemapFace(m_cubeMap, 5);

		m_cubeMap->GenerateMipMap();

		m_isCubeMapDirty = false;
	}
	return m_cubeMap;  
}

void SkyLight::GetDiffuseSkyLightSH(Vector4& sh0R, Vector4& sh0G, Vector4& sh0B, Vector4& sh1R, Vector4& sh1G, Vector4& sh1B, Vector3& sh2, Vector3& sh3)
{
	sh0R = Vector4(0,0,0,0);
	sh0G = Vector4(0, 0, 0, 0);
	sh0B = Vector4(0, 0, 0, 0);
	sh1R = Vector4(0, 0, 0, 0);
	sh1G = Vector4(0, 0, 0, 0);
	sh1B = Vector4(0, 0, 0, 0);
	sh2 = Vector3(0, 0, 0);
	sh3 = Vector3(0, 0, 0);
	if (m_skyLightToSHComputeShader)
	{
		auto cubeMap = GetCubeMap();
		int dsipatch = (int)ceil(9.0f / 16);
		m_skyLightToSHComputeShader->SetTexture(SHADER_TEXTURE_CUBE_MAP, cubeMap);
		m_skyLightToSHComputeShader->Dispatch(dsipatch, 1, 1);
		m_skyLightToSHComputeShader->GetSkyLightSH0(sh0R, sh0G, sh0B); 
		m_skyLightToSHComputeShader->GetSkyLightSH1(sh1R, sh1G, sh1B);
		m_skyLightToSHComputeShader->GetSkyLightSH2(sh2);
		m_skyLightToSHComputeShader->GetSkyLightSH3(sh3);
	}
}

void SkyLight::RenderCubemapFace(CubeMapRenderTexture* cubeMap, int face)
{
	if (m_skySphereMesh == nullptr || m_shader == nullptr)
		return;

	m_glContext->SetCubeRenderBuffer(cubeMap->GetCubeMapColorBuffer(), cubeMap->GetDepthBuffer(), face);
	m_glContext->SetViewport(0, 0, cubeMap->GetSize(), cubeMap->GetSize());
	m_glContext->ClearColor(0, 0, 0, 0);
	m_glContext->Clear((int)ClearFlags::Color | (int)ClearFlags::Depth | (int)ClearFlags::Stencil);

	Vector3 look, up;
	if (face == 0) {
		look = Vector3(1, 0, 0);
		up = Vector3(0, -1, 0);
	}
	else if (face == 1)
	{
		look = Vector3(-1, 0, 0);
		up = Vector3(0, -1, 0);
	}
	else if (face == 2)
	{
		look = Vector3(0, -1, 0);
		up = Vector3(0, 0, 1);
	}
	else if (face == 3)
	{
		look = Vector3(0, 1, 0);
		up = Vector3(0, 0, -1);
	}
	else if (face == 4)
	{
		look = Vector3(0, 0, -1);
		up = Vector3(0, -1, 0);
	}
	else if (face == 5)
	{
		look = Vector3(0, 0, 1);
		up = Vector3(0, -1, 0);
	}

	Matrix4x4 view;

	Matrix4x4::LookAt(&view, Vector3(0, 0, 0), look, up);

	Matrix4x4 proj, model;
	Matrix4x4::Identity(&model);
	Matrix4x4::Perspective(&proj, 90, 1.0, 0.01, 1000);

	m_shader->SetMatrix(SHADER_CONSTANT_LOCAL_TO_WORLD, model);
	m_shader->SetMatrix(SHADER_CONSTANT_WORLD_TO_VIEW, view);
	m_shader->SetMatrix(SHADER_CONSTANT_PROJECTION, proj);
	if (m_shader->Execute())
	{
		m_skySphereMesh->Commit();
	}

	m_glContext->SetRenderBuffer(nullptr, nullptr);
}

EnvironmentMapSkyLight::EnvironmentMapSkyLight(GLContext* glContext) : SkyLight(glContext)
{
	m_shader = GlobalResource::CreateShader<EnvironmentMapSkyShader>();
}

EnvironmentMapSkyLight::~EnvironmentMapSkyLight()
{
}

void EnvironmentMapSkyLight::SetEnvironmentMap(Texture* texture)
{
	if (m_shader)
	{
		((EnvironmentMapSkyShader*)m_shader)->SetTexture(SHADER_TEXTURE_ENVIRONMENT, texture);
		MakeCubeMapDirty();
	}
}

void EnvironmentMapSkyLight::SetEnvironmentColor(const Color& color)
{
	if (m_shader)
	{
		((EnvironmentMapSkyShader*)m_shader)->SetColor(SHADER_CONSTANT_COLOR, color);
		MakeCubeMapDirty();
	}
}

void EnvironmentMapSkyLight::SetEnvironmentIntensity(float intensity)
{
	if (m_shader)
	{
		((EnvironmentMapSkyShader*)m_shader)->SetFloat(SHADER_CONSTANT_INTENSITY, intensity);
		MakeCubeMapDirty();
	}
}

RayTracer::RTSkyShader* EnvironmentMapSkyLight::GetRayTracerSkyLightShader()
{
	RayTracer::RTShaderBase* shader = ((EnvironmentMapSkyShader*)m_shader)->GetRTShader();
	if (shader == nullptr)
		return nullptr;
	return (RayTracer::RTSkyShader*)shader;
}