#include "SkyLightToSHComputeShader.h"
#include "../RealtimeRender/GraphicsLib/ComputeShaderProgram.h"
#include "../RealtimeRender/GraphicsLib/ComputeBuffer.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../Texture/TextureBase.h"
#include "../Shader/ShaderConstants.h"

SkyLightToSHComputeShader::SkyLightToSHComputeShader(GLContext* glContext) : ComputeShaderBase(glContext)
{
	m_computeShaderProgram->Compile(L"Shaders/SkyLightToSH_CS.hlsl");
	m_computeBuffer = glContext->CreateComputeBuffer(sizeof(float)*3,9,nullptr);
}

SkyLightToSHComputeShader::~SkyLightToSHComputeShader()
{
	delete m_computeBuffer;
}

bool SkyLightToSHComputeShader::OnApplyParameters()
{
	TextureBase* skyLight = GetTexture(SHADER_TEXTURE_CUBE_MAP);

	if (!skyLight)
		return false;

	skyLight->CSSetTexture(0);
	m_computeBuffer->CSSetUAV(0);

	return true;
}

void SkyLightToSHComputeShader::OnGetDatas()
{
	float* data = (float*)m_computeBuffer->Map();
	if (data)
	{
		m_skyLightSH0R.x = 0.5 * sqrt(3.0f / Math::PI) * data[3*3];
		m_skyLightSH0R.y = 0.5 * sqrt(3.0f / Math::PI) * data[1*3]; 
		m_skyLightSH0R.z = 0.5 * sqrt(3.0f / Math::PI) * data[2*3];
		m_skyLightSH0R.w = 0.5 * sqrt(1.0f / Math::PI) * data[0*3];

		m_skyLightSH0G.x = 0.5 * sqrt(3.0f / Math::PI) * data[3*3+1];
		m_skyLightSH0G.y = 0.5 * sqrt(3.0f / Math::PI) * data[1 * 3 + 1];
		m_skyLightSH0G.z = 0.5 * sqrt(3.0f / Math::PI) * data[2 * 3 + 1];
		m_skyLightSH0G.w = 0.5 * sqrt(1.0f / Math::PI) * data[0 * 3 + 1];

		m_skyLightSH0B.x = 0.5 * sqrt(3.0f / Math::PI) * data[3*3+2];
		m_skyLightSH0B.y = 0.5 * sqrt(3.0f / Math::PI) * data[1 * 3 + 2];
		m_skyLightSH0B.z = 0.5 * sqrt(3.0f / Math::PI) * data[2 * 3 + 2];
		m_skyLightSH0B.w = 0.5 * sqrt(1.0f / Math::PI) * data[0 * 3 + 2];

		m_skyLightSH1R.x = 0.5 * sqrt(15.0f / Math::PI) * data[4 * 3];
		m_skyLightSH1R.y = 0.5 * sqrt(15.0f / Math::PI) * data[5 * 3];
		m_skyLightSH1R.z = 0.5 * sqrt(5.0f / Math::PI) * data[6 * 3];
		m_skyLightSH1R.w = 0.5 * sqrt(15.0f / Math::PI) * data[7 * 3];

		m_skyLightSH1G.x = 0.5 * sqrt(15.0f / Math::PI) * data[4 * 3 + 1];
		m_skyLightSH1G.y = 0.5 * sqrt(15.0f / Math::PI) * data[5 * 3 + 1];
		m_skyLightSH1G.z = 0.5 * sqrt(5.0f / Math::PI) * data[6 * 3 + 1];
		m_skyLightSH1G.w = 0.5 * sqrt(15.0f / Math::PI) * data[7 * 3 + 1];

		m_skyLightSH1B.x = 0.5 * sqrt(15.0f / Math::PI) * data[4 * 3 + 2];
		m_skyLightSH1B.y = 0.5 * sqrt(15.0f / Math::PI) * data[5 * 3 + 2];
		m_skyLightSH1B.z = 0.5 * sqrt(5.0f / Math::PI) * data[6 * 3 + 2];
		m_skyLightSH1B.w = 0.5 * sqrt(15.0f / Math::PI) * data[7 * 3 + 2];

		m_skyLightSH2.x = 0.25f * sqrt(15.0f / Math::PI) * data[8 * 3] -
			0.25f * sqrt(5.0f / Math::PI) * data[6 * 3];
		m_skyLightSH2.y = 0.25f * sqrt(15.0f / Math::PI) * data[8 * 3 + 1] -
			0.25f * sqrt(5.0f / Math::PI) * data[6 * 3 + 1];
		m_skyLightSH2.z = 0.25f * sqrt(15.0f / Math::PI) * data[8 * 3 + 2] -
			0.25f * sqrt(5.0f / Math::PI) * data[6 * 3 + 2];

		m_skyLightSH3.x = -0.25f * sqrt(5.0f / Math::PI) * data[6 * 3] -
			0.25f * sqrt(15.0f / Math::PI) * data[8 * 3];
		m_skyLightSH3.y = -0.25f * sqrt(5.0f / Math::PI) * data[6 * 3 + 1] -
			0.25f * sqrt(15.0f / Math::PI) * data[8 * 3 + 1];
		m_skyLightSH3.z = -0.25f * sqrt(5.0f / Math::PI) * data[6 * 3 + 2] -
			0.25f * sqrt(15.0f / Math::PI) * data[8 * 3 + 2];
	}
	m_computeBuffer->Unmap();
}
