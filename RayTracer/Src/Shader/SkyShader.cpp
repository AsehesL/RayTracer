#include "SkyShader.h"
#include "../RealtimeRender/GraphicsLib/ShaderProgram.h"
#include "../RealtimeRender/GraphicsLib/UniformBuffer.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../Texture/Texture.h"
#include "../RayTracer/Shader/RayTracerSkyShader.h"
#include "ShaderConstants.h"

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
	m_shaderProgram->Compile(L"Shaders/EnvironmentSkyShaderVS.hlsl", L"Shaders/EnvironmentSkyShaderPS.hlsl");
	m_rtShader = new RayTracer::RTEnvironmentMapSkyShader();

	SetColor(SHADER_CONSTANT_COLOR, Color(1,1,1,1));
	SetFloat(SHADER_CONSTANT_INTENSITY, 1.0f);
}

EnvironmentMapSkyShader::~EnvironmentMapSkyShader()
{
	delete m_rtShader;
}

RayTracer::RTShaderBase* EnvironmentMapSkyShader::GetRTShader()
{
	Color color = GetColor(SHADER_CONSTANT_COLOR);
	float intensity = GetFloat(SHADER_CONSTANT_INTENSITY);
	Texture* environmentMap = (Texture*)GetTexture(SHADER_TEXTURE_ENVIRONMENT);
	m_rtShader->environmentMap = environmentMap;
	m_rtShader->environmentColor = color * intensity;
	return m_rtShader;
}

bool EnvironmentMapSkyShader::OnApplyParameters()
{
	if (!SkyShader::OnApplyParameters())
		return false;
	UniformBuffer* environmentSkyUniformBuffer = m_shaderProgram->GetUniformBuffer("SkyBuffer");
	if (IsUniformBufferDirty(SHADER_CONSTANT_COLOR) || IsUniformBufferDirty(SHADER_CONSTANT_INTENSITY))
	{
		if (environmentSkyUniformBuffer)
		{
			SkyUniformBufferData* environmentSkyUniformBufferData = (SkyUniformBufferData*)environmentSkyUniformBuffer->Map();
			if (environmentSkyUniformBufferData)
			{
				Color color = GetColor(SHADER_CONSTANT_COLOR);
				float intensity = GetFloat(SHADER_CONSTANT_INTENSITY);
				environmentSkyUniformBufferData->skyColor = color * intensity;
				environmentSkyUniformBuffer->Unmap();
			}
		}
		ClearDirtyFlag(SHADER_CONSTANT_COLOR);
		ClearDirtyFlag(SHADER_CONSTANT_INTENSITY);
	}
	TextureBase* environmentMap = GetTexture(SHADER_TEXTURE_ENVIRONMENT);
	if (environmentMap == nullptr)
	{
		return false;
	}
	if (environmentSkyUniformBuffer)
		environmentSkyUniformBuffer->PSSetUniformBuffer(0);
	environmentMap->PSSetTexture(0);
	return true;
}
