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
	virtual RayTracer::RTSkyShader* GetRayTracerSkyLightShader() = 0;

private:
	void RenderCubemapFace(class CubeMapRenderTexture* cubeMap, int face);

protected:
	class GLContext* m_glContext;
	class Mesh* m_skySphereMesh;
	class SkyShader* m_shader;
	class CubeMapRenderTexture* m_cubeMap;
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