#include "IntegrateBRDFShader.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../RealtimeRender/GraphicsLib/ShaderProgram.h"

IntegrateBRDFShader::IntegrateBRDFShader(GLContext* glContext) : PostProcessShaderBase(glContext)
{
	m_shaderProgram->Compile(L"Shaders/ScreenQuadVS.hlsl", L"Shaders/IntegrateBRDF_PS.hlsl");
}

IntegrateBRDFShader::~IntegrateBRDFShader()
{
}

bool IntegrateBRDFShader::OnApplyParameters()
{
	if (!PostProcessShaderBase::OnApplyParameters())
		return false;
	return true;
}