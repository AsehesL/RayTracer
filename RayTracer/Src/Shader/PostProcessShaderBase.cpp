#include "PostProcessShaderBase.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../RealtimeRender/GraphicsLib/ShaderProgram.h"
#include "../Texture/Texture.h"

struct ScreenMatrixUniformBufferData
{
public:
	Matrix4x4f screenMatrix;
};

PostProcessShaderBase::PostProcessShaderBase(GLContext* glContext) : ShaderBase(glContext)
{
	m_matrixUniformBuffer = glContext->CreateShaderUniformBuffer(sizeof(ScreenMatrixUniformBufferData));

	m_isMatrixUniformBufferDirty = false;
}

PostProcessShaderBase::~PostProcessShaderBase()
{
	delete m_matrixUniformBuffer;
}

void PostProcessShaderBase::SetRenderSize(unsigned int windowWidth, unsigned int windowHeight, unsigned int renderWidth, unsigned int renderHeight)
{
	float renderAspect = float(renderWidth) / float(renderHeight);
	float windowAspect = float(windowWidth) / float(windowHeight);

	if (renderAspect > windowAspect)
	{
		m_screenMatrix.m00 = 1;
		m_screenMatrix.m01 = 0;
		m_screenMatrix.m02 = 0;
		m_screenMatrix.m03 = 0;

		m_screenMatrix.m10 = 0;
		m_screenMatrix.m11 = float(windowWidth) * float(renderHeight) / float(renderWidth) / float(windowHeight);
		m_screenMatrix.m12 = 0;
		m_screenMatrix.m13 = 0;

		m_screenMatrix.m20 = 0;
		m_screenMatrix.m21 = 0;
		m_screenMatrix.m22 = 1;
		m_screenMatrix.m23 = 0;

		m_screenMatrix.m30 = 0;
		m_screenMatrix.m31 = 0;
		m_screenMatrix.m32 = 0;
		m_screenMatrix.m33 = 1;
	}
	else
	{
		m_screenMatrix.m00 = float(windowHeight) * float(renderWidth) / float(renderHeight) / float(windowWidth);
		m_screenMatrix.m01 = 0;
		m_screenMatrix.m02 = 0;
		m_screenMatrix.m03 = 0;

		m_screenMatrix.m10 = 0;
		m_screenMatrix.m11 = 1;
		m_screenMatrix.m12 = 0;
		m_screenMatrix.m13 = 0;

		m_screenMatrix.m20 = 0;
		m_screenMatrix.m21 = 0;
		m_screenMatrix.m22 = 1;
		m_screenMatrix.m23 = 0;

		m_screenMatrix.m30 = 0;
		m_screenMatrix.m31 = 0;
		m_screenMatrix.m32 = 0;
		m_screenMatrix.m33 = 1;
	}
	m_isMatrixUniformBufferDirty = true;
}

bool PostProcessShaderBase::OnApplyParameters()
{
	if (m_isMatrixUniformBufferDirty)
	{
		ScreenMatrixUniformBufferData* matrixUniformBuffer = (ScreenMatrixUniformBufferData*)m_matrixUniformBuffer->Map();
		if (matrixUniformBuffer)
		{
			matrixUniformBuffer->screenMatrix = Matrix4x4f(m_screenMatrix);
			m_matrixUniformBuffer->Unmap();
		}
		m_isMatrixUniformBufferDirty = false;
	}
	m_matrixUniformBuffer->ApplyVSUniformBuffer(0);
	return true;
}