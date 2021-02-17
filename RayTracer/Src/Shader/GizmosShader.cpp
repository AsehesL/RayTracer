#include "GizmosShader.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../RealtimeRender/GraphicsLib/ShaderProgram.h"

GizmosShader::GizmosShader(GLContext* glContext) : MaterialShader(glContext)
{
	m_shaderProgram->Compile(L"Shaders/GizmosVS.hlsl", L"Shaders/GizmosPS.hlsl");
}

GizmosShader::~GizmosShader()
{
}