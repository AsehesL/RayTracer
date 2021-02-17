#pragma once
#include "MaterialShader.h"
#include "../Core/Color.h"

namespace RayTracer
{
	class RTPBRStandardShader;
}

class PBRShaderBase : public MaterialShader
{
public:
	PBRShaderBase(class GLContext* glContext);
	virtual ~PBRShaderBase() override;

	void SetAlbedoColor(const Color& color) 
	{ 
		m_albedoColor = color; 
		m_ispbrMaterialPSUniformBufferDirty = true;
	}

	Color GetAlbedoColor()
	{
		return m_albedoColor;
	}
	void SetRoughness(float roughness)
	{
		m_roughness = roughness;
		m_ispbrMaterialPSUniformBufferDirty = true;
	}
	float GetRoughness()
	{
		return m_roughness;
	}
	void SetMetallic(float metallic)
	{
		m_metallic = metallic;
		m_ispbrMaterialPSUniformBufferDirty = true;
	}
	float GetMetallic()
	{
		return m_metallic;
	}
	void SetUVTile(Vector2 uvTile)
	{
		m_uvTile = uvTile;
		m_ispbrMaterialPSUniformBufferDirty = true;
	}
	Vector2 GetUVTile()
	{
		return m_uvTile;
	}

	void SetSkyLightCubeMap(class CubeMapRenderTexture* cubeMap) override;
	
protected:
	virtual bool OnApplyParameters() override;

public:
	class Texture* albedoTexture;

protected:
	Color m_albedoColor;
	float m_roughness;
	float m_metallic;
	Vector2 m_uvTile;

	class CubeMapRenderTexture* m_skylight;

	class IShaderUniformBuffer* m_pbrMaterialPSUniformBuffer;
	
	bool m_ispbrMaterialPSUniformBufferDirty;
};

class PBR_StardardShader : public PBRShaderBase
{
public:
	PBR_StardardShader(class GLContext* glContext);
	virtual ~PBR_StardardShader() override;

	virtual RayTracer::RTShaderBase* GetRTShader() override;

private:
	RayTracer::RTPBRStandardShader* m_rtShader;
};