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

	virtual bool ShouldUpdateCubeMap() = 0;
};

class EnvironmentMapSkyShader : public SkyShader
{
public:
	EnvironmentMapSkyShader(class GLContext* glContext);
	virtual ~EnvironmentMapSkyShader() override;

	void SetEnvironmentMap(class Texture* environmentMap);
	void SetEnvironmentColor(const Color& color);
	void SetEnvironmentIntensity(float intensity);
	virtual bool ShouldUpdateCubeMap() override;
	virtual RayTracer::RTShaderBase* GetRTShader() override;

protected:
	virtual bool OnApplyParameters() override;

private:
	class Texture* m_environmentMap;
	Color m_environmentColor;
	float m_environmentIntensity;
	class IShaderUniformBuffer* m_environmentSkyUniformBuffer;
	bool m_isEnvironmentSkyUniformBufferDirty;
	bool m_isCubeMapDirty;
	RayTracer::RTEnvironmentMapSkyShader* m_rtShader;
};