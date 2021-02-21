#pragma once
#include "MaterialShader.h"
#include "../Core/Matrix.h"

namespace RayTracer
{
	class RTShaderBase;
	class RTEnvironmentMapSkyShader;
}

class SkyShader : public MaterialShader
{
public:
	SkyShader(class GLContext* glContext);
	virtual ~SkyShader() override;
};

class EnvironmentMapSkyShader : public SkyShader
{
public:
	EnvironmentMapSkyShader(class GLContext* glContext);
	virtual ~EnvironmentMapSkyShader() override;

	virtual RayTracer::RTShaderBase* GetRTShader() override;

protected:
	virtual bool OnApplyParameters() override;

private:
	RayTracer::RTEnvironmentMapSkyShader* m_rtShader;
};