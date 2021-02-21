#pragma once

#include "PostProcessShaderBase.h"

class ScreenQuadShader : public PostProcessShaderBase
{
public:
	ScreenQuadShader(class GLContext* glContext);
	~ScreenQuadShader() override;

protected:
	virtual bool OnApplyParameters() override;
};