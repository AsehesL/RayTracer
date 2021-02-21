#include "LinearToGammaShader.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../RealtimeRender/GraphicsLib/ShaderProgram.h"
#include "../Texture/TextureBase.h"
#include "ShaderConstants.h"

LinearToGammaShader::LinearToGammaShader(GLContext* glContext) : PostProcessShaderBase(glContext)
{
	m_shaderProgram->Compile(L"Shaders/ScreenQuadVS.hlsl", L"Shaders/LinearToGammaPS.hlsl");
}

LinearToGammaShader::~LinearToGammaShader()
{
}

bool LinearToGammaShader::OnApplyParameters()
{
	if (!PostProcessShaderBase::OnApplyParameters())
		return false;
	TextureBase* renderTexture = GetTexture(SHADER_TEXTURE_TEXTURE);
	if (renderTexture != nullptr)
	{
		renderTexture->PSSetTexture(0);
		return true;
	}
	return false;
}