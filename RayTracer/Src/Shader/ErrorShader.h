#pragma once
#include "MaterialShader.h"
#include "../Core/Color.h"

class ErrorShader : public MaterialShader
{
public:
	ErrorShader(class GLContext* glContext);
	virtual ~ErrorShader() override;

	virtual RayTracer::RTShaderBase* GetRTShader() override { return nullptr; }
};