#include "TransparencyShader.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../RealtimeRender/GraphicsLib/ShaderProgram.h"
#include "../RealtimeRender/GraphicsLib/UniformBuffer.h"
#include "../Texture/TextureBase.h"
#include "../Texture/RenderTexture.h"
#include "../Common/GlobalResource.h"
#include "ShaderConstants.h"
#include "../RayTracer/Shader/RayTracerMaterialShader.h"

struct TransparencyMaterialPSUniformBuffer
{
public:
	Color color;
};

TransparencyShader::TransparencyShader(GLContext* glContext) : MaterialShader(glContext)
{
	m_shaderProgram->Compile(L"Shaders/TransparencyVS.hlsl", L"Shaders/TransparencyPS.hlsl");
	m_rtShader = new RayTracer::RTTransparencyShader();
}

TransparencyShader::~TransparencyShader()
{
	delete m_rtShader;
}

RayTracer::RTShaderBase* TransparencyShader::GetRTShader()
{
	m_rtShader->color = GetColor("color");
	m_rtShader->refractive = GetFloat("refractive");
	m_rtShader->roughness = GetFloat("roughness");
	return m_rtShader;
}

bool TransparencyShader::OnApplyParameters()
{
	if (!MaterialShader::OnApplyParameters())
		return false;
	UniformBuffer* transparencyMaterialPSUniformBuffer = m_shaderProgram->GetUniformBuffer("TransparencyBuffer");
	UniformBuffer* matrixUniformBuffer = m_shaderProgram->GetUniformBuffer("MatrixBuffer");
	//UniformBuffer* viewUniformBuffer = m_shaderProgram->GetUniformBuffer("ViewBuffer");
	if (IsUniformBufferDirty("color"))
	{
		if (transparencyMaterialPSUniformBuffer)
		{
			TransparencyMaterialPSUniformBuffer* transparencyMaterialPSUniformBufferData = (TransparencyMaterialPSUniformBuffer*)transparencyMaterialPSUniformBuffer->Map();
			if (transparencyMaterialPSUniformBufferData)
			{
				Color color = GetColor("color");
				transparencyMaterialPSUniformBufferData->color = color;
				transparencyMaterialPSUniformBuffer->Unmap();
			}
		}
		ClearDirtyFlag("color");
	}
	if (matrixUniformBuffer)
		matrixUniformBuffer->PSSetUniformBuffer(0);
	//if(viewUniformBuffer)
	//	viewUniformBuffer->PSSetUniformBuffer(1);
	if (transparencyMaterialPSUniformBuffer)
		transparencyMaterialPSUniformBuffer->PSSetUniformBuffer(1);

	TextureBase* screenCapture = GetTexture(SHADER_TEXTURE_SCREEN_CAPTURE);
	if (screenCapture == nullptr)
	{
		screenCapture = GlobalResource::GetWhiteTexture();
	}
	screenCapture->PSSetTexture(0);

	return true;
}
