#pragma once
#include "ShaderBase.h"
#include "../Core/Matrix.h"
#include "../Core/Color.h"

namespace RayTracer
{
	class RTShaderBase;
}

class MaterialShader : public ShaderBase
{
public:
	MaterialShader(class GLContext* glContext);
	~MaterialShader() override;
	bool ExecuteForShadow();
	void SetLocalToWorldMatrix(const Matrix4x4& localToWorldMatrix);
	void SetWorldToViewMatrix(const Matrix4x4& worldToViewMatrix);
	void SetProjectionMatrix(const Matrix4x4& projectionMatrix);
	void SetCameraInformation(const Vector3& cameraPosition);
	void SetSunLightInformation(const Vector3& sunDirection, const Color& sunColor);
	virtual void SetSkyLightCubeMap(class CubeMapRenderTexture* cubeMap) {}
	void SetShadowParameters(class RenderTexture* shadowMap, const Matrix4x4& lightSpaceMatrix, const Matrix4x4& shadowProjectionMatrix);
	void SetShadowBias(float shadowBias)
	{
		m_shadowBias = shadowBias;
		m_isShadowPassUniformBufferDirty = true;
	}
	virtual void SetScreenCapture(class RenderTexture* screenCapture) {}
	virtual bool IsTransparency() { return false; }
	virtual RayTracer::RTShaderBase* GetRTShader() = 0;

protected:
	virtual bool OnApplyParameters() override;
	virtual bool OnApplyShadowParameters();

protected:
	class IShaderProgram* m_shaderProgramForShadow;

protected:
	class IShaderUniformBuffer* m_matrixUniformBuffer;
	class IShaderUniformBuffer* m_viewUniformBuffer;
	class IShaderUniformBuffer* m_shadowUniformBuffer;
	class IShaderUniformBuffer* m_shadowPassUniformBuffer;

	class RenderTexture* m_shadowMap;

private:
	Matrix4x4 m_localToWorldMatrix;
	Matrix4x4 m_worldToViewMatrix;
	Matrix4x4 m_projectionMatrix;

	Matrix4x4 m_worldToLightSpaceMatrix;
	Matrix4x4 m_shadowProjectionMatrix;

	Color m_sunColor;
	Vector3 m_sunDirection;
	Vector3 m_cameraPosition;

	Vector4 m_shadowMapSize;
	float m_shadowBias;

	bool m_isMatrixUniformBufferDirty;
	bool m_isViewUniformBufferDirty;
	bool m_isShadowUniformBufferDirty;
	bool m_isShadowPassUniformBufferDirty;
};