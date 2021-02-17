#include "ScreenQuadShader.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../RealtimeRender/GraphicsLib/ShaderProgram.h"
#include "../Texture/Texture.h"

ScreenQuadShader::ScreenQuadShader(GLContext* glContext) : PostProcessShaderBase(glContext)
{
	m_shaderProgram->Compile(L"Shaders/ScreenQuadVS.hlsl", L"Shaders/ScreenQuadPS.hlsl");

	texture = nullptr;
}

ScreenQuadShader::~ScreenQuadShader()
{
}

bool ScreenQuadShader::OnApplyParameters()
{
	if (!PostProcessShaderBase::OnApplyParameters())
		return false;
	if (texture != nullptr)
	{
		texture->SetShaderResource(0);
		return true;
	}
	return false;
}
