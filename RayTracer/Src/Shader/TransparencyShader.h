#pragma once
#include "MaterialShader.h"
#include "../Core/Color.h"

class TransparencyShader : public MaterialShader
{
public:
	TransparencyShader(class GLContext* glContext);
	virtual ~TransparencyShader() override;
	virtual bool IsTransparency() override { return true; }
	virtual void SetScreenCapture(class RenderTexture* screenCapture) override;

	void SetColor(const Color& color)
	{
		m_color = color;
		m_isTransparencyMaterialPSUniformBufferDirty = true;
	}

	Color GetColor() const
	{
		return m_color;
	}

	virtual RayTracer::RTShaderBase* GetRTShader() override;

protected:
	virtual bool OnApplyParameters() override;

private:
	Color m_color;

	class IShaderUniformBuffer* m_transparencyMaterialPSUniformBuffer;
	class RenderTexture* m_screenCapture;

	bool m_isTransparencyMaterialPSUniformBufferDirty;
};