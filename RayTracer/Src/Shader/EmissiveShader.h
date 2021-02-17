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

	void SetColor(const Color& color)
	{
		m_color = color;
		m_isEmissiveMaterialPSUniformBufferDirty = true;
	}

	Color GetColor() const
	{
		return m_color;
	}

	void SetIntensity(float intensity)
	{
		m_intensity = intensity;
		m_isEmissiveMaterialPSUniformBufferDirty = true;
	}

	float GetIntensity() const
	{
		return m_intensity;
	}

protected:
	virtual bool OnApplyParameters() override;

private:
	Color m_color;
	float m_intensity;

	class IShaderUniformBuffer* m_emissiveMaterialPSUniformBuffer;

	RayTracer::RTEmissiveShader* m_rtShader;

	bool m_isEmissiveMaterialPSUniformBufferDirty;
};