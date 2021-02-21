#include "TransparencyShader.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../RealtimeRender/GraphicsLib/ShaderProgram.h"
#include "../RealtimeRender/GraphicsLib/UniformBuffer.h"
#include "../Texture/TextureBase.h"
#include "../Texture/RenderTexture.h"
#include "../Common/GlobalResource.h"
#include "ShaderConstants.h"

struct TransparencyMaterialPSUniformBuffer
{
public:
	Color color;
};

TransparencyShader::TransparencyShader(GLContext* glContext) : MaterialShader(glContext)
{
	m_shaderProgram->Compile(L"Shaders/TransparencyVS.hlsl", L"Shaders/TransparencyPS.hlsl");
}

TransparencyShader::~TransparencyShader()
{
}

RayTracer::RTShaderBase* TransparencyShader::GetRTShader()
{
	return nullptr;
}

bool TransparencyShader::OnApplyParameters()
{
	if (!MaterialShader::OnApplyParameters())
		return false;
	UniformBuffer* transparencyMaterialPSUniformBuffer = m_shaderProgram->GetUniformBuffer("SkyBuffer");
	UniformBuffer* matrixUniformBuffer = m_shaderProgram->GetUniformBuffer("MatrixBuffer");
	UniformBuffer* viewUniformBuffer = m_shaderProgram->GetUniformBuffer("ViewBuffer");
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
	if(viewUniformBuffer)
		viewUniformBuffer->PSSetUniformBuffer(1);
	if (transparencyMaterialPSUniformBuffer)
		transparencyMaterialPSUniformBuffer->PSSetUniformBuffer(2);

	TextureBase* screenCapture = GetTexture(SHADER_TEXTURE_SCREEN_CAPTURE);
	if (screenCapture == nullptr)
	{
		screenCapture = GlobalResource::GetWhiteTexture();
	}
	screenCapture->PSSetTexture(0);

	return true;
}
