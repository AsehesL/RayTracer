#pragma once
#include "MaterialShader.h"
#include "../Core/Color.h"

namespace RayTracer
{
	class RTPBRStandardShader;
}

class PBRShaderBase : public MaterialShader
{
public:
	PBRShaderBase(class GLContext* glContext);
	virtual ~PBRShaderBase() override;
	
protected:
	virtual bool OnApplyParameters() override;
};

class PBR_StardardShader : public PBRShaderBase
{
public:
	PBR_StardardShader(class GLContext* glContext);
	virtual ~PBR_StardardShader() override;

	virtual RayTracer::RTShaderBase* GetRTShader() override;

private:
	RayTracer::RTPBRStandardShader* m_rtShader;
};