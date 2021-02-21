#include "EmissiveShader.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../RealtimeRender/GraphicsLib/ShaderProgram.h"
#include "../RealtimeRender/GraphicsLib/UniformBuffer.h"
#include "../RayTracer/Shader/RayTracerMaterialShader.h"

struct EmissiveMaterialPSBufferData
{
public:
	Color color;
};

EmissiveShader::EmissiveShader(GLContext* glContext) : MaterialShader(glContext)
{
	m_shaderProgram->Compile(L"Shaders/EmissiveVS.hlsl", L"Shaders/EmissivePS.hlsl");

	m_rtShader = new RayTracer::RTEmissiveShader();

	SetColor("color", Color(1,1,1,1));
	SetFloat("intensity", 1);
}

EmissiveShader::~EmissiveShader()
{
	delete m_rtShader;
}

RayTracer::RTShaderBase* EmissiveShader::GetRTShader()
{
	Color color = GetColor("color");
	float intensity = GetFloat("intensity");
	m_rtShader->color = color * intensity;
	return m_rtShader;
}

bool EmissiveShader::OnApplyParameters()
{
	if (!MaterialShader::OnApplyParameters())
		return false;
	UniformBuffer* emissiveMaterialPSUniformBuffer = m_shaderProgram->GetUniformBuffer("EmissiveBuffer");
	if (IsUniformBufferDirty("color") || IsUniformBufferDirty("intensity"))
	{
		if (emissiveMaterialPSUniformBuffer)
		{
			EmissiveMaterialPSBufferData* emissiveMaterialPSUniformBufferData = (EmissiveMaterialPSBufferData*)emissiveMaterialPSUniformBuffer->Map();
			if (emissiveMaterialPSUniformBufferData)
			{
				Color color = GetColor("color");
				float intensity = GetFloat("intensity");
				emissiveMaterialPSUniformBufferData->color = color * intensity;
				emissiveMaterialPSUniformBuffer->Unmap();
			}
		}
		ClearDirtyFlag("color");
		ClearDirtyFlag("intensity");
	}
	if (emissiveMaterialPSUniformBuffer)
		emissiveMaterialPSUniformBuffer->PSSetUniformBuffer(0);
	return true;
}
