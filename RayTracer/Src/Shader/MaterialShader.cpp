#include "MaterialShader.h"
#include "../RealtimeRender/GraphicsLib/ShaderProgram.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../RealtimeRender/GraphicsLib/ShaderProgram.h"
#include "../Texture/RenderTexture.h"

struct MatrixUniformBufferData
{
public:
	Matrix4x4f localToWorld;
	Matrix4x4f worldToView;
	Matrix4x4f projection;
};

struct ViewUniformBufferData
{
public:
	Vector4f cameraPosition;
	Vector4f sunLightDirection;
	Color sunLightColor;
};

struct ShadowUniformBufferData
{
public:
	Matrix4x4f worldToLightSpace;
	Matrix4x4f shadowProjection;
	Vector4f shadowMapSize;
};

struct ShadowPassUniformBufferData
{
public:
	Vector4f shadowBias;
};

MaterialShader::MaterialShader(GLContext* glContext) : ShaderBase(glContext)
{
	m_matrixUniformBuffer = glContext->CreateShaderUniformBuffer(sizeof(MatrixUniformBufferData));
	m_viewUniformBuffer = glContext->CreateShaderUniformBuffer(sizeof(ViewUniformBufferData));
	m_shadowUniformBuffer = glContext->CreateShaderUniformBuffer(sizeof(ShadowUniformBufferData));
	m_shadowPassUniformBuffer = glContext->CreateShaderUniformBuffer(sizeof(ShadowPassUniformBufferData));
	m_shaderProgramForShadow = glContext->CreateShaderProgram();
	m_isMatrixUniformBufferDirty = true;
	m_isViewUniformBufferDirty = true;
	m_isShadowUniformBufferDirty = true;
	m_isShadowPassUniformBufferDirty = true;

	m_shadowMap = nullptr;
}

MaterialShader::~MaterialShader()
{
	delete m_matrixUniformBuffer;
	delete m_viewUniformBuffer;
	delete m_shaderProgramForShadow;
	delete m_shadowPassUniformBuffer;
}

bool MaterialShader::ExecuteForShadow()
{
	if (m_shaderProgramForShadow == nullptr || !m_shaderProgramForShadow->IsValid())
		return false;
	if (!OnApplyShadowParameters())
		return false;
	return m_shaderProgramForShadow->Execute();
}

void MaterialShader::SetLocalToWorldMatrix(const Matrix4x4& localToWorldMatrix)
{
	m_localToWorldMatrix = localToWorldMatrix;
	m_isMatrixUniformBufferDirty = true;
}

void MaterialShader::SetWorldToViewMatrix(const Matrix4x4& worldToViewMatrix)
{
	m_worldToViewMatrix = worldToViewMatrix;
	m_isMatrixUniformBufferDirty = true;
}

void MaterialShader::SetProjectionMatrix(const Matrix4x4& projectionMatrix)
{
	m_projectionMatrix = projectionMatrix;
	m_isMatrixUniformBufferDirty = true;
}

void MaterialShader::SetCameraInformation(const Vector3& cameraPosition)
{
	m_cameraPosition = cameraPosition;
	m_isViewUniformBufferDirty = true;
}

void MaterialShader::SetSunLightInformation(const Vector3& sunDirection, const Color& sunColor)
{
	m_sunDirection = sunDirection;
	m_sunColor = sunColor;
	m_isViewUniformBufferDirty = true;
}

void MaterialShader::SetShadowParameters(RenderTexture* shadowMap, const Matrix4x4& lightSpaceMatrix, const Matrix4x4& shadowProjectionMatrix)
{
	m_shadowMap = shadowMap;
	m_worldToLightSpaceMatrix = lightSpaceMatrix;
	m_shadowProjectionMatrix = shadowProjectionMatrix;
	m_isShadowUniformBufferDirty = true;

	if (shadowMap != nullptr)
		m_shadowMapSize = Vector4(1.0f / shadowMap->GetWidth(), 1.0f / shadowMap->GetHeight(), shadowMap->GetWidth(), shadowMap->GetHeight());
}

bool MaterialShader::OnApplyParameters()
{
	if (m_isMatrixUniformBufferDirty)
	{ 
		MatrixUniformBufferData* matrixUniformBuffer = (MatrixUniformBufferData*)m_matrixUniformBuffer->Map();
		if (matrixUniformBuffer)
		{
			matrixUniformBuffer->localToWorld = Matrix4x4f(m_localToWorldMatrix);
			matrixUniformBuffer->worldToView = Matrix4x4f(m_worldToViewMatrix);
			matrixUniformBuffer->projection = Matrix4x4f(m_projectionMatrix);
			m_matrixUniformBuffer->Unmap();
		}
		m_isMatrixUniformBufferDirty = false;
	}
	if (m_isViewUniformBufferDirty)
	{
		ViewUniformBufferData* viewUniformBuffer = (ViewUniformBufferData*)m_viewUniformBuffer->Map();
		if (viewUniformBuffer)
		{
			viewUniformBuffer->cameraPosition = Vector4f(m_cameraPosition);
			viewUniformBuffer->sunLightDirection = Vector4f(m_sunDirection);
			viewUniformBuffer->sunLightColor = m_sunColor;
			m_viewUniformBuffer->Unmap();
		}
		m_isViewUniformBufferDirty = false;
	}
	if (m_isShadowUniformBufferDirty)
	{
		ShadowUniformBufferData* shadowUniformBuffer = (ShadowUniformBufferData*)m_shadowUniformBuffer->Map();
		if (shadowUniformBuffer)
		{
			shadowUniformBuffer->worldToLightSpace = m_worldToLightSpaceMatrix;
			shadowUniformBuffer->shadowProjection = m_shadowProjectionMatrix;
			shadowUniformBuffer->shadowMapSize = Vector4f(m_shadowMapSize);
			m_shadowUniformBuffer->Unmap();
		}
		m_isShadowUniformBufferDirty = false;
	}
	m_viewUniformBuffer->ApplyVSUniformBuffer(1);
	m_viewUniformBuffer->ApplyPSUniformBuffer(0);
	m_matrixUniformBuffer->ApplyVSUniformBuffer(0);
	return true;
}

bool MaterialShader::OnApplyShadowParameters()
{
	if (m_isMatrixUniformBufferDirty)
	{
		MatrixUniformBufferData* matrixUniformBuffer = (MatrixUniformBufferData*)m_matrixUniformBuffer->Map();
		if (matrixUniformBuffer)
		{
			matrixUniformBuffer->localToWorld = Matrix4x4f(m_localToWorldMatrix);
			matrixUniformBuffer->worldToView = Matrix4x4f(m_worldToViewMatrix);
			matrixUniformBuffer->projection = Matrix4x4f(m_projectionMatrix);
			m_matrixUniformBuffer->Unmap();
		}
		m_isMatrixUniformBufferDirty = false;
	}
	if (m_isShadowPassUniformBufferDirty)
	{
		ShadowPassUniformBufferData* shadowPassUniformBuffer = (ShadowPassUniformBufferData*)m_shadowPassUniformBuffer->Map();
		if (shadowPassUniformBuffer)
		{
			shadowPassUniformBuffer->shadowBias = Vector4f(m_shadowBias);
		}
		m_shadowPassUniformBuffer->Unmap();
		m_isShadowPassUniformBufferDirty = false;
	}
	m_matrixUniformBuffer->ApplyVSUniformBuffer(0);
	m_shadowPassUniformBuffer->ApplyPSUniformBuffer(0);
	return true;
}
