#pragma once
#include "ShaderBase.h"
#include "../Core/Matrix.h"

class PostProcessShaderBase : public ShaderBase
{
public:
	PostProcessShaderBase(class GLContext* glContext);
	~PostProcessShaderBase() override;

	void SetRenderSize(unsigned int windowWidth, unsigned int windowHeight, unsigned int renderWidth, unsigned int renderHeight);

protected:
	virtual bool OnApplyParameters() override;
};