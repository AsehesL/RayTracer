#include "EmissiveShader.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../RealtimeRender/GraphicsLib/ShaderProgram.h"
#include "../RayTracer/Shader/RayTracerMaterialShader.h"

struct EmissiveMaterialPSBufferData
{
public:
	Color color;
};

EmissiveShader::EmissiveShader(GLContext* glContext) : MaterialShader(glContext)
{
	m_emissiveMaterialPSUniformBuffer = glContext->CreateShaderUniformBuffer(sizeof(EmissiveMaterialPSBufferData));

	m_shaderProgram->Compile(L"Shaders/EmissiveVS.hlsl", L"Shaders/EmissivePS.hlsl");

	m_isEmissiveMaterialPSUniformBufferDirty = true;

	m_rtShader = new RayTracer::RTEmissiveShader();

	m_intensity = 1.0f;
}

EmissiveShader::~EmissiveShader()
{
	delete m_emissiveMaterialPSUniformBuffer;
	delete m_rtShader;
}

RayTracer::RTShaderBase* EmissiveShader::GetRTShader()
{
	m_rtShader->color = m_color * m_intensity;
	return m_rtShader;
}

bool EmissiveShader::OnApplyParameters()
{
	if (!MaterialShader::OnApplyParameters())
		return false;
	if (m_isEmissiveMaterialPSUniformBufferDirty)
	{
		EmissiveMaterialPSBufferData* emissiveMaterialPSUniformBuffer = (EmissiveMaterialPSBufferData*)m_emissiveMaterialPSUniformBuffer->Map();
		if (emissiveMaterialPSUniformBuffer)
		{
			emissiveMaterialPSUniformBuffer->color = m_color * m_intensity;
			m_emissiveMaterialPSUniformBuffer->Unmap();
		}
		m_isEmissiveMaterialPSUniformBufferDirty = false;
	}
	m_emissiveMaterialPSUniformBuffer->ApplyPSUniformBuffer(0);
	return true;
}
