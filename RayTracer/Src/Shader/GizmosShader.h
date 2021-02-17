#pragma once

#include "MaterialShader.h"

class GizmosShader : public MaterialShader
{
public:
	GizmosShader(class GLContext* glContext);
	virtual ~GizmosShader() override;

	virtual RayTracer::RTShaderBase* GetRTShader() override { return nullptr; }
};