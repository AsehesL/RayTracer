#include "PostProcessShaderBase.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../RealtimeRender/GraphicsLib/ShaderProgram.h"
#include "../RealtimeRender/GraphicsLib/UniformBuffer.h"
#include "../Texture/Texture.h"

struct ScreenMatrixUniformBufferData
{
public:
	Matrix4x4f screenMatrix;
};

PostProcessShaderBase::PostProcessShaderBase(GLContext* glContext) : ShaderBase(glContext)
{
}

PostProcessShaderBase::~PostProcessShaderBase()
{
}

void PostProcessShaderBase::SetRenderSize(unsigned int windowWidth, unsigned int windowHeight, unsigned int renderWidth, unsigned int renderHeight)
{
	float renderAspect = float(renderWidth) / float(renderHeight);
	float windowAspect = float(windowWidth) / float(windowHeight);

	Matrix4x4 screenMatrix;
	if (renderAspect > windowAspect)
	{
		screenMatrix.m00 = 1;
		screenMatrix.m01 = 0;
		screenMatrix.m02 = 0;
		screenMatrix.m03 = 0;

		screenMatrix.m10 = 0;
		screenMatrix.m11 = float(windowWidth) * float(renderHeight) / float(renderWidth) / float(windowHeight);
		screenMatrix.m12 = 0;
		screenMatrix.m13 = 0;

		screenMatrix.m20 = 0;
		screenMatrix.m21 = 0;
		screenMatrix.m22 = 1;
		screenMatrix.m23 = 0;

		screenMatrix.m30 = 0;
		screenMatrix.m31 = 0;
		screenMatrix.m32 = 0;
		screenMatrix.m33 = 1;
	}
	else
	{
		screenMatrix.m00 = float(windowHeight) * float(renderWidth) / float(renderHeight) / float(windowWidth);
		screenMatrix.m01 = 0;
		screenMatrix.m02 = 0;
		screenMatrix.m03 = 0;

		screenMatrix.m10 = 0;
		screenMatrix.m11 = 1;
		screenMatrix.m12 = 0;
		screenMatrix.m13 = 0;

		screenMatrix.m20 = 0;
		screenMatrix.m21 = 0;
		screenMatrix.m22 = 1;
		screenMatrix.m23 = 0;

		screenMatrix.m30 = 0;
		screenMatrix.m31 = 0;
		screenMatrix.m32 = 0;
		screenMatrix.m33 = 1;
	}
	SetMatrix("screenMatrix",screenMatrix);
}

bool PostProcessShaderBase::OnApplyParameters()
{
	UniformBuffer* matrixUniformBuffer = m_shaderProgram->GetUniformBuffer("MatrixBuffer");
	if (IsUniformBufferDirty("screenMatrix"))
	{
		if (matrixUniformBuffer)
		{
			ScreenMatrixUniformBufferData* matrixUniformBufferData = (ScreenMatrixUniformBufferData*)matrixUniformBuffer->Map();
			if (matrixUniformBufferData)
			{
				Matrix4x4f screenMatrix = GetMatrixf("screenMatrix");
				matrixUniformBufferData->screenMatrix = screenMatrix;
				matrixUniformBuffer->Unmap();
			}
		}
		ClearDirtyFlag("screenMatrix");
	}

	matrixUniformBuffer->VSSetUniformBuffer(0);
	return true;
}