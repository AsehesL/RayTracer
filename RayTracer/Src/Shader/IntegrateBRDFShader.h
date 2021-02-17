#pragma once

#include "PostProcessShaderBase.h"

class IntegrateBRDFShader : public PostProcessShaderBase
{
public:
	IntegrateBRDFShader(class GLContext* glContext);
	~IntegrateBRDFShader() override;

protected:
	virtual bool OnApplyParameters() override;
};