#include "PBRShader.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../RealtimeRender/GraphicsLib/ShaderProgram.h"
#include "../Texture/Texture.h"
#include "../Texture/RenderTexture.h"
#include "../Common/GlobalResource.h"
#include "../RayTracer/Shader/RayTracerPBRShader.h"

struct PBRMaterialPSUniformData
{
public:
	Color albedoColor;
	Color roughness;
	Color metallic;
	Vector4f uvTile;
};

PBRShaderBase::PBRShaderBase(GLContext* glContext) : MaterialShader(glContext)
{
	m_pbrMaterialPSUniformBuffer = glContext->CreateShaderUniformBuffer(sizeof(PBRMaterialPSUniformData));
	m_shaderProgramForShadow->Compile(L"Shaders/OpaqueShadowVS.hlsl", L"Shaders/OpaqueShadowPS.hlsl");

	m_ispbrMaterialPSUniformBufferDirty = true;

	m_roughness = 0.5f;
	m_metallic = 0.0f;

	albedoTexture = nullptr;

	m_skylight = nullptr;
	
	m_uvTile = Vector2(1, 1);
}

PBRShaderBase::~PBRShaderBase()
{
	delete m_pbrMaterialPSUniformBuffer;
	delete m_shadowUniformBuffer;
}

void PBRShaderBase::SetSkyLightCubeMap(CubeMapRenderTexture* cubeMap)
{
	m_skylight = cubeMap;
}

bool PBRShaderBase::OnApplyParameters()
{
	if (!MaterialShader::OnApplyParameters())
		return false;
	IShaderResource* cubeMap = m_skylight;
	if (cubeMap == nullptr)
		cubeMap = GlobalResource::GetBlackTexture();
	IShaderResource* shadowMap = m_shadowMap;
	if (shadowMap == nullptr)
		shadowMap = GlobalResource::GetWhiteTexture();
	if (m_ispbrMaterialPSUniformBufferDirty)
	{
		PBRMaterialPSUniformData* pbrMaterialPSUniformBuffer = (PBRMaterialPSUniformData*)m_pbrMaterialPSUniformBuffer->Map();
		if (pbrMaterialPSUniformBuffer)
		{
			pbrMaterialPSUniformBuffer->albedoColor = m_albedoColor;
			pbrMaterialPSUniformBuffer->roughness = Color(m_roughness, m_roughness, m_roughness, m_roughness);
			pbrMaterialPSUniformBuffer->metallic = Color(m_metallic, m_metallic, m_metallic, m_metallic);
			pbrMaterialPSUniformBuffer->uvTile = Vector4f(m_uvTile);
			m_pbrMaterialPSUniformBuffer->Unmap();
		}
		m_ispbrMaterialPSUniformBufferDirty = false;
	}
	
	m_matrixUniformBuffer->ApplyPSUniformBuffer(0);
	m_viewUniformBuffer->ApplyPSUniformBuffer(1);
	m_pbrMaterialPSUniformBuffer->ApplyPSUniformBuffer(2);
	m_shadowUniformBuffer->ApplyPSUniformBuffer(3);
	if (albedoTexture != nullptr)
	{
		albedoTexture->SetShaderResource(0);
	}
	else
	{
		Texture* whiteTexture = GlobalResource::GetWhiteTexture();
		whiteTexture->SetShaderResource(0);
	}
	cubeMap->SetShaderResource(1);
	shadowMap->SetShaderResource(2);
	IShaderResource* integrateBRDFTexture = GlobalResource::GetPreIntegratedBRDFTexture();
	if (!integrateBRDFTexture)
		integrateBRDFTexture = GlobalResource::GetBlackTexture();
	integrateBRDFTexture->SetShaderResource(3);
	
	return true;
}

PBR_StardardShader::PBR_StardardShader(GLContext* glContext) : PBRShaderBase(glContext)
{
	m_shaderProgram->Compile(L"Shaders/PBR_StandardVS.hlsl", L"Shaders/PBR_StandardPS.hlsl");
	m_rtShader = new RayTracer::RTPBRStandardShader();
}

PBR_StardardShader::~PBR_StardardShader()
{
	delete m_rtShader;
}

RayTracer::RTShaderBase* PBR_StardardShader::GetRTShader()
{
	m_rtShader->albedoColor = m_albedoColor;
	m_rtShader->albedoTex = albedoTexture;
	m_rtShader->bump = nullptr;
	m_rtShader->emissiveColor = Color(0,0,0,0);
	m_rtShader->emissiveTex = nullptr;
	m_rtShader->metallic = m_metallic;
	m_rtShader->mro = nullptr;
	m_rtShader->roughness = m_roughness;
	m_rtShader->uvTile = m_uvTile;
	return m_rtShader;
}
