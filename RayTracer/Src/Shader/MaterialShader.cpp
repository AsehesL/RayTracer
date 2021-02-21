#include "MaterialShader.h"
#include "ShaderConstants.h"
#include "../RealtimeRender/GraphicsLib/ShaderProgram.h"
#include "../RealtimeRender/GraphicsLib/UniformBuffer.h"
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
	m_shaderProgramForShadow = glContext->CreateShaderProgram();
}

MaterialShader::~MaterialShader()
{
	delete m_shaderProgramForShadow;
}

bool MaterialShader::ExecuteForShadow()
{
	if (m_shaderProgramForShadow == nullptr || !m_shaderProgramForShadow->IsValid())
		return false;
	if (!OnApplyShadowParameters())
		return false;
	return m_shaderProgramForShadow->Execute();
}

bool MaterialShader::OnApplyParameters()
{
	UniformBuffer* matrixUniformBuffer = m_shaderProgram->GetUniformBuffer("MatrixBuffer");
	UniformBuffer* viewUniformBuffer = m_shaderProgram->GetUniformBuffer("ViewBuffer");
	UniformBuffer* shadowUniformBuffer = m_shaderProgram->GetUniformBuffer("ShadowUniformBuffer");
	if (IsUniformBufferDirty(SHADER_CONSTANT_LOCAL_TO_WORLD) || IsUniformBufferDirty(SHADER_CONSTANT_WORLD_TO_VIEW) || IsUniformBufferDirty(SHADER_CONSTANT_PROJECTION))
	{
		if (matrixUniformBuffer)
		{
			MatrixUniformBufferData* matrixUniformBufferData = (MatrixUniformBufferData*)matrixUniformBuffer->Map();
			if (matrixUniformBufferData)
			{
				Matrix4x4f localToWorld = GetMatrixf(SHADER_CONSTANT_LOCAL_TO_WORLD);
				Matrix4x4f worldToView = GetMatrixf(SHADER_CONSTANT_WORLD_TO_VIEW);
				Matrix4x4f projection = GetMatrixf(SHADER_CONSTANT_PROJECTION);
				matrixUniformBufferData->localToWorld = localToWorld;
				matrixUniformBufferData->worldToView = worldToView;
				matrixUniformBufferData->projection = projection;
				matrixUniformBuffer->Unmap();
			}
		}
		ClearDirtyFlag(SHADER_CONSTANT_LOCAL_TO_WORLD);
		ClearDirtyFlag(SHADER_CONSTANT_WORLD_TO_VIEW);
		ClearDirtyFlag(SHADER_CONSTANT_PROJECTION);
	}
	if (IsUniformBufferDirty(SHADER_CONSTANT_CAMERA_POSITION) || IsUniformBufferDirty(SHADER_CONSTANT_SUN_LIGHT_DIRECTION) || IsUniformBufferDirty(SHADER_CONSTANT_SUN_LIGHT_COLOR))
	{
		if (viewUniformBuffer)
		{
			ViewUniformBufferData* viewUniformBufferData = (ViewUniformBufferData*)viewUniformBuffer->Map();
			if (viewUniformBufferData)
			{
				Vector4f cameraPosition = GetVectorf(SHADER_CONSTANT_CAMERA_POSITION);
				Vector4f sunLightDirection = GetVectorf(SHADER_CONSTANT_SUN_LIGHT_DIRECTION);
				Color sunLightColor = GetColor(SHADER_CONSTANT_SUN_LIGHT_COLOR);
				viewUniformBufferData->cameraPosition = cameraPosition;
				viewUniformBufferData->sunLightDirection = sunLightDirection;
				viewUniformBufferData->sunLightColor = sunLightColor;
				viewUniformBuffer->Unmap();
			}
		}
		ClearDirtyFlag(SHADER_CONSTANT_CAMERA_POSITION);
		ClearDirtyFlag(SHADER_CONSTANT_SUN_LIGHT_DIRECTION);
		ClearDirtyFlag(SHADER_CONSTANT_SUN_LIGHT_COLOR);
	}
	if (IsUniformBufferDirty(SHADER_CONSTANT_WORLD_TO_LIGHTSPACE) || IsUniformBufferDirty(SHADER_CONSTANT_SHADOW_PROJECTION) || IsUniformBufferDirty(SHADER_CONSTANT_SHADOW_MAP_SIZE))
	{
		if (shadowUniformBuffer)
		{
			ShadowUniformBufferData* shadowUniformBufferData = (ShadowUniformBufferData*)shadowUniformBuffer->Map();
			if (shadowUniformBufferData)
			{
				Matrix4x4f worldToLightSpace = GetMatrixf(SHADER_CONSTANT_WORLD_TO_LIGHTSPACE);
				Matrix4x4f shadowProjection = GetMatrixf(SHADER_CONSTANT_SHADOW_PROJECTION);
				Vector4f shadowMapSize = GetVectorf(SHADER_CONSTANT_SHADOW_MAP_SIZE);
				shadowUniformBufferData->worldToLightSpace = worldToLightSpace;
				shadowUniformBufferData->shadowProjection = shadowProjection;
				shadowUniformBufferData->shadowMapSize = shadowMapSize;
				shadowUniformBuffer->Unmap();
			}
		}
		ClearDirtyFlag(SHADER_CONSTANT_WORLD_TO_LIGHTSPACE);
		ClearDirtyFlag(SHADER_CONSTANT_SHADOW_PROJECTION);
		ClearDirtyFlag(SHADER_CONSTANT_SHADOW_MAP_SIZE);
	}

	if (viewUniformBuffer)
	{ 
		viewUniformBuffer->VSSetUniformBuffer(1);
		viewUniformBuffer->PSSetUniformBuffer(0);
	}
	if (matrixUniformBuffer)
		matrixUniformBuffer->VSSetUniformBuffer(0);
	return true;
}

bool MaterialShader::OnApplyShadowParameters()
{
	UniformBuffer* matrixUniformBuffer = m_shaderProgramForShadow->GetUniformBuffer("MatrixBuffer");
	UniformBuffer* shadowPassUniformBuffer = m_shaderProgramForShadow->GetUniformBuffer("ShadowPassBuffer");
	if (IsUniformBufferDirty(SHADER_CONSTANT_LOCAL_TO_WORLD) || IsUniformBufferDirty(SHADER_CONSTANT_WORLD_TO_VIEW) || IsUniformBufferDirty(SHADER_CONSTANT_PROJECTION))
	{
		if (matrixUniformBuffer)
		{
			MatrixUniformBufferData* matrixUniformBufferData = (MatrixUniformBufferData*)matrixUniformBuffer->Map();
			if (matrixUniformBufferData)
			{
				Matrix4x4f localToWorld = GetMatrixf(SHADER_CONSTANT_LOCAL_TO_WORLD);
				Matrix4x4f worldToView = GetMatrixf(SHADER_CONSTANT_WORLD_TO_VIEW);
				Matrix4x4f projection = GetMatrixf(SHADER_CONSTANT_PROJECTION);
				matrixUniformBufferData->localToWorld = localToWorld;
				matrixUniformBufferData->worldToView = worldToView;
				matrixUniformBufferData->projection = projection;
				matrixUniformBuffer->Unmap();
			}
		}
		ClearDirtyFlag(SHADER_CONSTANT_LOCAL_TO_WORLD);
		ClearDirtyFlag(SHADER_CONSTANT_WORLD_TO_VIEW);
		ClearDirtyFlag(SHADER_CONSTANT_PROJECTION);
	}
	if (IsUniformBufferDirty(SHADER_CONSTANT_SHADOW_BIAS))
	{
		if (shadowPassUniformBuffer)
		{
			ShadowPassUniformBufferData* shadowPassUniformBufferData = (ShadowPassUniformBufferData*)shadowPassUniformBuffer->Map();
			if (shadowPassUniformBufferData)
			{
				Vector4f shadowBias = GetVectorf(SHADER_CONSTANT_SHADOW_BIAS);
				shadowPassUniformBufferData->shadowBias = shadowBias;
				shadowPassUniformBuffer->Unmap();
			}
		}
		ClearDirtyFlag(SHADER_CONSTANT_SHADOW_BIAS);
	}

	if (matrixUniformBuffer)
		matrixUniformBuffer->VSSetUniformBuffer(0);
	if (shadowPassUniformBuffer)
		shadowPassUniformBuffer->PSSetUniformBuffer(0);
	return true;
}
