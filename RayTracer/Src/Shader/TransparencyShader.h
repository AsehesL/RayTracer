#pragma once
#include "MaterialShader.h"
#include "../Core/Color.h"

namespace RayTracer
{
	class RTTransparencyShader;
}

class TransparencyShader : public MaterialShader
{
public:
	TransparencyShader(class GLContext* glContext);
	virtual ~TransparencyShader() override;
	virtual bool IsTransparency() override { return true; } 

	virtual RayTracer::RTShaderBase* GetRTShader() override;

protected:
	virtual bool OnApplyParameters() override;

private:
	RayTracer::RTTransparencyShader* m_rtShader;
};