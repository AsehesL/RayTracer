#pragma once
#include "ShaderBase.h"
#include "../Core/Matrix.h"
#include "../Core/Color.h"

namespace RayTracer
{
	class RTShaderBase;
}

class MaterialShader : public ShaderBase
{
public:
	MaterialShader(class GLContext* glContext);
	~MaterialShader() override;
	bool ExecuteForShadow();
	virtual bool IsTransparency() { return false; }
	virtual RayTracer::RTShaderBase* GetRTShader() = 0;

protected:
	virtual bool OnApplyParameters() override;
	virtual bool OnApplyShadowParameters();

protected:
	class ShaderProgram* m_shaderProgramForShadow;
};