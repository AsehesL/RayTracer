#include "ErrorShader.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../RealtimeRender/GraphicsLib/ShaderProgram.h"

ErrorShader::ErrorShader(GLContext* glContext) : MaterialShader(glContext)
{
	m_shaderProgram->Compile(L"Shaders/ErrorShaderVS.hlsl", L"Shaders/ErrorShaderPS.hlsl");
}

ErrorShader::~ErrorShader()
{
}