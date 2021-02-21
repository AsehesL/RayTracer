#include "PBRShader.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../RealtimeRender/GraphicsLib/ShaderProgram.h"
#include "../RealtimeRender/GraphicsLib/UniformBuffer.h"
#include "../Texture/Texture.h"
#include "../Texture/RenderTexture.h"
#include "../Common/GlobalResource.h"
#include "../RayTracer/Shader/RayTracerPBRShader.h"
#include "ShaderConstants.h"

struct PBRMaterialPSUniformData
{
public:
	Color albedoColor;
	Vector4f roughness;
	Vector4f metallic;
	Vector4f uvTile;
};

struct SkyLightSHUniformBufferData
{
public:
	Vector4f skyLightDiffuseSH0R;
	Vector4f skyLightDiffuseSH0G;
	Vector4f skyLightDiffuseSH0B;
	Vector4f skyLightDiffuseSH1R;
	Vector4f skyLightDiffuseSH1G;
	Vector4f skyLightDiffuseSH1B;
	Vector4f skyLightDiffuseSH2;
	Vector4f skyLightDiffuseSH3;
};

PBRShaderBase::PBRShaderBase(GLContext* glContext) : MaterialShader(glContext)
{
	m_shaderProgramForShadow->Compile(L"Shaders/OpaqueShadowVS.hlsl", L"Shaders/OpaqueShadowPS.hlsl");

	SetFloat("roughness", 0.5f);
	SetFloat("metallic", 0.0f);
	SetVector2("uvTile", Vector2(1,1));
}

PBRShaderBase::~PBRShaderBase()
{
}

bool PBRShaderBase::OnApplyParameters()
{
	if (!MaterialShader::OnApplyParameters())
		return false;
	TextureBase* cubeMap = GetTexture(SHADER_TEXTURE_CUBE_MAP);
	if (cubeMap == nullptr)
		cubeMap = GlobalResource::GetBlackTexture();
	TextureBase* shadowMap = GetTexture(SHADER_TEXTURE_SHADOW_MAP);
	if (shadowMap == nullptr)
		shadowMap = GlobalResource::GetWhiteTexture();
	UniformBuffer* pbrMaterialPSUniformBuffer = m_shaderProgram->GetUniformBuffer("PBRBuffer");
	UniformBuffer* skyLightSHUniformBuffer = m_shaderProgram->GetUniformBuffer("SkyLightSHBuffer");
	if (IsUniformBufferDirty("albedoColor") || IsUniformBufferDirty("roughness") || IsUniformBufferDirty("metallic") || IsUniformBufferDirty("uvTile"))
	{
		if (pbrMaterialPSUniformBuffer)
		{
			PBRMaterialPSUniformData* pbrMaterialPSUniformBufferData = (PBRMaterialPSUniformData*)pbrMaterialPSUniformBuffer->Map();
			if (pbrMaterialPSUniformBufferData)
			{
				Color albedoColor = GetColor("albedoColor");
				Vector4f roughness = GetVectorf("roughness");
				Vector4f metallic = GetVectorf("metallic");
				Vector4f uvTile = GetVectorf("uvTile");
				pbrMaterialPSUniformBufferData->albedoColor = albedoColor;
				pbrMaterialPSUniformBufferData->roughness = roughness;
				pbrMaterialPSUniformBufferData->metallic = metallic;
				pbrMaterialPSUniformBufferData->uvTile = uvTile;
				pbrMaterialPSUniformBuffer->Unmap();
			}
		}
		ClearDirtyFlag("albedoColor");
		ClearDirtyFlag("roughness");
		ClearDirtyFlag("metallic");
		ClearDirtyFlag("uvTile");
	}
	if (IsUniformBufferDirty(SHADER_CONSTANT_SKY_LIGHT_SH0R) ||
		IsUniformBufferDirty(SHADER_CONSTANT_SKY_LIGHT_SH0G) ||
		IsUniformBufferDirty(SHADER_CONSTANT_SKY_LIGHT_SH0B) ||
		IsUniformBufferDirty(SHADER_CONSTANT_SKY_LIGHT_SH1R) ||
		IsUniformBufferDirty(SHADER_CONSTANT_SKY_LIGHT_SH1G) ||
		IsUniformBufferDirty(SHADER_CONSTANT_SKY_LIGHT_SH1B) ||
		IsUniformBufferDirty(SHADER_CONSTANT_SKY_LIGHT_SH2) ||
		IsUniformBufferDirty(SHADER_CONSTANT_SKY_LIGHT_SH3))
	{
		if (skyLightSHUniformBuffer)
		{
			SkyLightSHUniformBufferData* skyLightSHUniformBufferData = (SkyLightSHUniformBufferData*)skyLightSHUniformBuffer->Map();
			if (skyLightSHUniformBufferData)
			{
				Vector4f skyLight0R = GetVectorf(SHADER_CONSTANT_SKY_LIGHT_SH0R);
				Vector4f skyLight0G = GetVectorf(SHADER_CONSTANT_SKY_LIGHT_SH0G);
				Vector4f skyLight0B = GetVectorf(SHADER_CONSTANT_SKY_LIGHT_SH0B);
				Vector4f skyLight1R = GetVectorf(SHADER_CONSTANT_SKY_LIGHT_SH1R);
				Vector4f skyLight1G = GetVectorf(SHADER_CONSTANT_SKY_LIGHT_SH1G);
				Vector4f skyLight1B = GetVectorf(SHADER_CONSTANT_SKY_LIGHT_SH1B);
				Vector4f skyLight2 = GetVectorf(SHADER_CONSTANT_SKY_LIGHT_SH2);
				Vector4f skyLight3 = GetVectorf(SHADER_CONSTANT_SKY_LIGHT_SH3);
				skyLightSHUniformBufferData->skyLightDiffuseSH0R = skyLight0R;
				skyLightSHUniformBufferData->skyLightDiffuseSH0G = skyLight0G;
				skyLightSHUniformBufferData->skyLightDiffuseSH0B = skyLight0B;
				skyLightSHUniformBufferData->skyLightDiffuseSH1R = skyLight1R;
				skyLightSHUniformBufferData->skyLightDiffuseSH1G = skyLight1G;
				skyLightSHUniformBufferData->skyLightDiffuseSH1B = skyLight1B;
				skyLightSHUniformBufferData->skyLightDiffuseSH2 = skyLight2;
				skyLightSHUniformBufferData->skyLightDiffuseSH3 = skyLight3;
				skyLightSHUniformBuffer->Unmap();
			}
		}
		ClearDirtyFlag(SHADER_CONSTANT_SKY_LIGHT_SH0R);
		ClearDirtyFlag(SHADER_CONSTANT_SKY_LIGHT_SH0G);
		ClearDirtyFlag(SHADER_CONSTANT_SKY_LIGHT_SH0B);
		ClearDirtyFlag(SHADER_CONSTANT_SKY_LIGHT_SH1R);
		ClearDirtyFlag(SHADER_CONSTANT_SKY_LIGHT_SH1G);
		ClearDirtyFlag(SHADER_CONSTANT_SKY_LIGHT_SH1B);
		ClearDirtyFlag(SHADER_CONSTANT_SKY_LIGHT_SH2);
		ClearDirtyFlag(SHADER_CONSTANT_SKY_LIGHT_SH3);
	}
	UniformBuffer* matrixUniformBuffer = m_shaderProgram->GetUniformBuffer("MatrixBuffer");
	UniformBuffer* viewUniformBuffer = m_shaderProgram->GetUniformBuffer("ViewBuffer");
	UniformBuffer* shadowUniformBuffer = m_shaderProgram->GetUniformBuffer("ShadowUniformBuffer");
	
	if (matrixUniformBuffer)
		matrixUniformBuffer->PSSetUniformBuffer(0);
	if (viewUniformBuffer)
		viewUniformBuffer->PSSetUniformBuffer(1);
	if (pbrMaterialPSUniformBuffer)
		pbrMaterialPSUniformBuffer->PSSetUniformBuffer(2);
	if (skyLightSHUniformBuffer)
		skyLightSHUniformBuffer->PSSetUniformBuffer(3);
	if (shadowUniformBuffer)
		shadowUniformBuffer->PSSetUniformBuffer(4);
	TextureBase* albedoTexture = GetTexture("albedoTexture");
	if (albedoTexture != nullptr)
	{
		albedoTexture->PSSetTexture(0);
	}
	else
	{
		Texture* whiteTexture = GlobalResource::GetWhiteTexture();
		whiteTexture->PSSetTexture(0);
	}
	TextureBase* mroTexture = GetTexture("mroTex");
	if (mroTexture != nullptr)
		mroTexture->PSSetTexture(1);
	else
	{
		Texture* whiteTexture = GlobalResource::GetWhiteTexture();
		whiteTexture->PSSetTexture(1);
	}
	TextureBase* bumpTexture = GetTexture("bumpTex");
	if (bumpTexture != nullptr)
		bumpTexture->PSSetTexture(2);
	else
	{
		Texture* defaultBump = GlobalResource::GetDefaultBumpMap();
		defaultBump->PSSetTexture(2);
	}
	cubeMap->PSSetTexture(3);
	shadowMap->PSSetTexture(4);
	TextureBase* integrateBRDFTexture = GlobalResource::GetPreIntegratedBRDFTexture();
	if (!integrateBRDFTexture)
		integrateBRDFTexture = GlobalResource::GetBlackTexture();
	integrateBRDFTexture->PSSetTexture(5);
	
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
	Color albedoColor = GetColor("albedoColor");
	float roughness = GetFloat("roughness");
	float metallic = GetFloat("metallic");
	Vector2 uvTile = GetVector2("uvTile");
	Texture* albedoTexture = (Texture*)GetTexture("albedoTexture");
	Texture* mroTexture = (Texture*)GetTexture("mroTex");
	Texture* bumpTexture = (Texture*)GetTexture("bumpTex");

	m_rtShader->albedoColor = albedoColor;
	m_rtShader->albedoTex = albedoTexture;
	m_rtShader->bump = bumpTexture;
	m_rtShader->emissiveColor = Color(0,0,0,0);
	m_rtShader->emissiveTex = nullptr;
	m_rtShader->metallic = metallic;
	m_rtShader->mro = mroTexture;
	m_rtShader->roughness = roughness;
	m_rtShader->uvTile = uvTile;
	return m_rtShader;
}
