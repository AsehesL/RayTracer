#pragma once 

#include "PostProcessShaderBase.h"

class LinearToGammaShader : public PostProcessShaderBase
{
public:
	LinearToGammaShader(class GLContext* glContext);
	~LinearToGammaShader() override;

protected:
	virtual bool OnApplyParameters() override;

public:
	class RenderTexture* renderTexture;
};