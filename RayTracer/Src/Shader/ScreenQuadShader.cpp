#include "ScreenQuadShader.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../RealtimeRender/GraphicsLib/ShaderProgram.h"
#include "../Texture/Texture.h"
#include "ShaderConstants.h"

ScreenQuadShader::ScreenQuadShader(GLContext* glContext) : PostProcessShaderBase(glContext)
{
	m_shaderProgram->Compile(L"Shaders/ScreenQuadVS.hlsl", L"Shaders/ScreenQuadPS.hlsl");
}

ScreenQuadShader::~ScreenQuadShader()
{
}

bool ScreenQuadShader::OnApplyParameters()
{
	if (!PostProcessShaderBase::OnApplyParameters())
		return false;
	TextureBase* texture = GetTexture(SHADER_TEXTURE_TEXTURE);
	if (texture != nullptr)
	{
		texture->PSSetTexture(0);
		return true;
	}
	return false;
}
