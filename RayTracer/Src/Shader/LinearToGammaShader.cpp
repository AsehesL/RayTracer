#include "LinearToGammaShader.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../RealtimeRender/GraphicsLib/ShaderProgram.h"
#include "../Texture/RenderTexture.h"

LinearToGammaShader::LinearToGammaShader(GLContext* glContext) : PostProcessShaderBase(glContext)
{
	m_shaderProgram->Compile(L"Shaders/ScreenQuadVS.hlsl", L"Shaders/LinearToGammaPS.hlsl");

	renderTexture = nullptr;
}

LinearToGammaShader::~LinearToGammaShader()
{
}

bool LinearToGammaShader::OnApplyParameters()
{
	if (!PostProcessShaderBase::OnApplyParameters())
		return false;
	if (renderTexture != nullptr)
	{
		renderTexture->SetShaderResource(0);
		return true;
	}
	return false;
}