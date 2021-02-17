#pragma once

class ShaderBase
{
public:
	ShaderBase(class GLContext* glContext);
	virtual ~ShaderBase();

	bool Execute();

	bool IsValid();

protected:
	virtual bool OnApplyParameters() = 0;
	virtual void OnApplyRenderStates();

protected:
	class GLContext* m_glContext;
	class IShaderProgram* m_shaderProgram;
};