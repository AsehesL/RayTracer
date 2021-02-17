#include "SkyShader.h"
#include "../RealtimeRender/GraphicsLib/ShaderProgram.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../Texture/Texture.h"
#include "../RayTracer/Shader/RayTracerSkyShader.h"

struct SkyUniformBufferData
{
public:
	Color skyColor;
};

SkyShader::SkyShader(GLContext* glContext) : MaterialShader(glContext)
{
}

SkyShader::~SkyShader()
{
}

EnvironmentMapSkyShader::EnvironmentMapSkyShader(GLContext* glContext) : SkyShader(glContext)
{
	m_environmentSkyUniformBuffer = glContext->CreateShaderUniformBuffer(sizeof(SkyUniformBufferData));
	m_shaderProgram->Compile(L"Shaders/EnvironmentSkyShaderVS.hlsl", L"Shaders/EnvironmentSkyShaderPS.hlsl");
	m_environmentMap = nullptr;
	m_environmentIntensity = 1.0f;
	m_isEnvironmentSkyUniformBufferDirty = true;
	m_isCubeMapDirty = true;
	m_rtShader = new RayTracer::RTEnvironmentMapSkyShader();
}

EnvironmentMapSkyShader::~EnvironmentMapSkyShader()
{
	delete m_environmentSkyUniformBuffer;
	delete m_rtShader;
}

void EnvironmentMapSkyShader::SetEnvironmentMap(Texture* environmentMap)
{
	m_environmentMap = environmentMap;
	m_isCubeMapDirty = true;
}

void EnvironmentMapSkyShader::SetEnvironmentColor(const Color& color)
{
	m_environmentColor = color;
	m_isCubeMapDirty = true;
	m_isEnvironmentSkyUniformBufferDirty = true;
}

void EnvironmentMapSkyShader::SetEnvironmentIntensity(float intensity)
{
	m_environmentIntensity = intensity;
	m_isCubeMapDirty = true;
	m_isEnvironmentSkyUniformBufferDirty = true;
}

bool EnvironmentMapSkyShader::ShouldUpdateCubeMap()
{
	if (m_isCubeMapDirty)
	{
		m_isCubeMapDirty = false;
		return true;
	}
	return false;
}

RayTracer::RTShaderBase* EnvironmentMapSkyShader::GetRTShader()
{
	m_rtShader->environmentMap = m_environmentMap;
	m_rtShader->environmentColor = m_environmentColor * m_environmentIntensity;
	return m_rtShader;
}

bool EnvironmentMapSkyShader::OnApplyParameters()
{
	if (!SkyShader::OnApplyParameters())
		return false;
	if (m_isEnvironmentSkyUniformBufferDirty)
	{
		SkyUniformBufferData* environmentSkyUniformBuffer = (SkyUniformBufferData*)m_environmentSkyUniformBuffer->Map();
		if (environmentSkyUniformBuffer)
		{
			environmentSkyUniformBuffer->skyColor = m_environmentColor * m_environmentIntensity;
			m_environmentSkyUniformBuffer->Unmap();
		}
		m_isEnvironmentSkyUniformBufferDirty = false;
	}
	if (m_environmentMap == nullptr)
	{
		return false;
	}
	m_environmentSkyUniformBuffer->ApplyPSUniformBuffer(0);
	m_environmentMap->SetShaderResource(0);
	return true;
}
