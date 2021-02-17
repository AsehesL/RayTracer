#include "ShaderBase.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../RealtimeRender/GraphicsLib/ShaderProgram.h"

ShaderBase::ShaderBase(GLContext* glContext)
{
	m_glContext = glContext;
	m_shaderProgram = glContext->CreateShaderProgram();
}

ShaderBase::~ShaderBase()
{
	delete m_shaderProgram;
}

bool ShaderBase::Execute()
{
	if (!IsValid())
		return false;
	if (!OnApplyParameters())
		return false;
	OnApplyRenderStates();
	m_glContext->ApplyRenderStates();
	return m_shaderProgram->Execute();
}

bool ShaderBase::IsValid()
{
	if (m_shaderProgram)
		return m_shaderProgram->IsValid();
	return false;
}

void ShaderBase::OnApplyRenderStates()
{
	m_glContext->ztestFunc = CompareFunc::LEqual;
}
