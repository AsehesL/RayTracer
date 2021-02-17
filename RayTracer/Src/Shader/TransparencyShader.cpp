#include "TransparencyShader.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../RealtimeRender/GraphicsLib/ShaderProgram.h"
#include "../RealtimeRender/GraphicsLib/ShaderResource.h"
#include "../Texture/RenderTexture.h"
#include "../Common/GlobalResource.h"

struct TransparencyMaterialPSUniformBuffer
{
public:
	Color color;
};

TransparencyShader::TransparencyShader(GLContext* glContext) : MaterialShader(glContext)
{
	m_transparencyMaterialPSUniformBuffer = glContext->CreateShaderUniformBuffer(sizeof(TransparencyMaterialPSUniformBuffer));

	m_shaderProgram->Compile(L"Shaders/TransparencyVS.hlsl", L"Shaders/TransparencyPS.hlsl");

	m_isTransparencyMaterialPSUniformBufferDirty = true;

	m_screenCapture = nullptr;
}

TransparencyShader::~TransparencyShader()
{
	delete m_transparencyMaterialPSUniformBuffer;
}

void TransparencyShader::SetScreenCapture(RenderTexture* screenCapture)
{
	m_screenCapture = screenCapture;
}

RayTracer::RTShaderBase* TransparencyShader::GetRTShader()
{
	return nullptr;
}

bool TransparencyShader::OnApplyParameters()
{
	if (!MaterialShader::OnApplyParameters())
		return false;
	if (m_isTransparencyMaterialPSUniformBufferDirty)
	{
		TransparencyMaterialPSUniformBuffer* transparencyMaterialPSUniformBuffer = (TransparencyMaterialPSUniformBuffer*)m_transparencyMaterialPSUniformBuffer->Map();
		if (transparencyMaterialPSUniformBuffer)
		{
			transparencyMaterialPSUniformBuffer->color = m_color;
			m_transparencyMaterialPSUniformBuffer->Unmap();
		}
		m_isTransparencyMaterialPSUniformBufferDirty = false;
	}
	m_matrixUniformBuffer->ApplyPSUniformBuffer(0);
	m_viewUniformBuffer->ApplyPSUniformBuffer(1);
	m_transparencyMaterialPSUniformBuffer->ApplyPSUniformBuffer(2);

	IShaderResource* screenCapture = m_screenCapture;
	if (screenCapture == nullptr)
	{
		screenCapture = GlobalResource::GetWhiteTexture();
	}
	screenCapture->SetShaderResource(0);

	return true;
}
