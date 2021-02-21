#pragma once
#include "MaterialShader.h"
#include "../Core/Color.h"

namespace RayTracer
{
	class RTEmissiveShader;
}

class EmissiveShader : public MaterialShader
{
public:
	EmissiveShader(class GLContext* glContext);
	virtual ~EmissiveShader() override;
	virtual RayTracer::RTShaderBase* GetRTShader() override;

protected:
	virtual bool OnApplyParameters() override;

private:
	RayTracer::RTEmissiveShader* m_rtShader;
};