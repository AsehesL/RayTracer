#pragma once

#include "../Core/Vector.h"
#include "../Core/Matrix.h"
#include "../Core/Color.h"

namespace RayTracer 
{
	class RTSkyShader;
}

class SkyLight
{
public:
	SkyLight(class GLContext* glContext);
	virtual ~SkyLight();

	void Render(const Vector3& cameraPosition, const Matrix4x4& worldToViewMatrix, const Matrix4x4& projectionMatrix);
	class CubeMapRenderTexture* GetCubeMap();
	void GetDiffuseSkyLightSH(Vector4& sh0R, Vector4& sh0G, Vector4& sh0B, Vector4& sh1R, Vector4& sh1G, Vector4& sh1B, Vector3& sh2, Vector3& sh3);
	virtual RayTracer::RTSkyShader* GetRayTracerSkyLightShader() = 0;

protected:
	void MakeCubeMapDirty() { m_isCubeMapDirty = true; }
	bool IsCubeMapDirty() const { return m_isCubeMapDirty; }

private:
	void RenderCubemapFace(class CubeMapRenderTexture* cubeMap, int face);

protected:
	class GLContext* m_glContext;
	class Mesh* m_skySphereMesh;
	class SkyShader* m_shader;
	class CubeMapRenderTexture* m_cubeMap;
	class SkyLightToSHComputeShader* m_skyLightToSHComputeShader;

private:
	bool m_isCubeMapDirty;
};

class EnvironmentMapSkyLight : public SkyLight
{
public:
	EnvironmentMapSkyLight(class GLContext* glContext);
	virtual ~EnvironmentMapSkyLight() override;

	void SetEnvironmentMap(class Texture* texture);
	void SetEnvironmentColor(const Color& color);
	void SetEnvironmentIntensity(float intensity);

	virtual RayTracer::RTSkyShader* GetRayTracerSkyLightShader() override;
};