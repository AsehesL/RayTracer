#include "framework.h"
#include "Src/RealtimeRender/RealtimeRenderer.h"
#include "Src/RealtimeRender/GraphicsLib/GLContext.h"
#include "Src/RayTracer/RayTracerRenderer.h"
#include "Src/RayTracer/Scene/RayTracerScene.h"
#include "Src/RayTracer/Integrator/PathTracer/PathTracer.h"
#include "Src/RayTracer/Integrator/PhotonMapper/PhotonMapper.h"
#include "Src/Core/ObjectPool.hpp"
#include "Src/Primitive/Primitive.h"
#include "Src/Primitive/Mesh.h"
#include "Src/Shader/ShaderBase.h"
#include "Src/Shader/ScreenQuadShader.h"
#include "Src/Camera/Camera.h"
#include "Src/Texture/Texture.h"
#include "Src/Common/GlobalResource.h"
#include "Src/Shader/PBRShader.h"
#include "Src/Shader/EmissiveShader.h"
#include "Src/Shader/TransparencyShader.h"
#include "Src/Scene/Scene.h"
#include "Src/Light/SkyLight.h"
#include "Src/Light/SunLight.h"
#include "Src/RayTracer/Sampler/Sampler.h"
#include "Src/RealtimeRender/RayTracingPreviewRenderer.h"
#include "Src/RayTracer/Render/RayTracerRenderTarget.h"
#include "FreeImage.h"

#include "Src/RealtimeRender/GraphicsLib/D3D11/D3D11Context.h"

GLContext* glContext = nullptr;
Scene* scene = nullptr;
RealtimeRenderer* realtimeRenderer = nullptr;
RayTracingPreviewRenderer* rayTracingPreviewRenderer = nullptr;
RayTracer::RayTracerRenderer* rayTracerRenderer = nullptr;

int integrateType = 0;
RayTracer::PathTracer* pathTracer = nullptr;
RayTracer::PhotonMapper* photonMapper = nullptr;

ObjectPool<Camera>* cameraPool = nullptr;
ObjectPool<PrimitiveBase>* primitiveObjectPool = nullptr;
ObjectPool<SkyLight>* skylightPool = nullptr;
ObjectPool<SunLight>* sunlightPool = nullptr;

static bool isInitialized = false;

static int windowWidth;
static int windowHeight;

static int renderWidth;
static int renderHeight;

static int selectedPrimitiveID = -1;

extern "C" __declspec(dllexport) bool StartRayTracing()
{
	if (!isInitialized)
		return false;
	if (rayTracerRenderer == nullptr)
		return false;
	if (rayTracingPreviewRenderer != nullptr)
	{
		rayTracingPreviewRenderer->SetRenderSizeForPreview(windowWidth, windowHeight, renderWidth, renderHeight);
	}
	if (integrateType == 0)
	{
		if (pathTracer != nullptr)
		{
			return rayTracerRenderer->StartRayTracing(pathTracer);
		}
	}
	if (integrateType == 1)
	{
		if (photonMapper != nullptr)
			return rayTracerRenderer->StartRayTracing(photonMapper);
	}
	return false;
}

extern "C" __declspec(dllexport) bool HasResult()
{
	if (!isInitialized)
		return false;
	if (rayTracerRenderer == nullptr)
		return false;
	if (rayTracerRenderer->GetRenderTarget() == nullptr)
		return false;
	return true;
}

extern "C" __declspec(dllexport) void SaveResult(const char* path, bool isHDR)
{
	if (!isInitialized)
		return;
	if (rayTracerRenderer == nullptr)
		return;
	if (rayTracerRenderer->GetRenderTarget() == nullptr)
		return;
	rayTracerRenderer->GetRenderTarget()->Save(path, isHDR);
}

extern "C" __declspec(dllexport) int GetPrimitiveByScreenPos(int screenPosX, int screenPosY)
{
	if (!isInitialized)
		return -1;
	PrimitiveBase* hitPrimitive = scene->GetPrimitiveByScreenPos(screenPosX, screenPosY);
	return primitiveObjectPool->GetID(hitPrimitive);
}

extern "C" __declspec(dllexport) unsigned int GetRayTracingResultWidth()
{
	if (!isInitialized)
		return 0;
	if (rayTracerRenderer == nullptr)
		return 0;
	RayTracer::RenderTarget* renderTarget = rayTracerRenderer->GetRenderTarget();
	if (renderTarget)
		return renderTarget->GetWidth();
	return 0;
}

extern "C" __declspec(dllexport) unsigned int GetRayTracingResultHeight()
{
	if (!isInitialized)
		return 0;
	if (rayTracerRenderer == nullptr)
		return 0;
	RayTracer::RenderTarget* renderTarget = rayTracerRenderer->GetRenderTarget();
	if (renderTarget)
		return renderTarget->GetHeight();
	return 0;
}

extern "C" __declspec(dllexport) void GetRayTracingResultPixel(int x, int y, float& r, float& g, float& b, float& a)
{
	if (!isInitialized)
		return;
	if (rayTracerRenderer == nullptr)
		return;
	RayTracer::RenderTarget* renderTarget = rayTracerRenderer->GetRenderTarget();
	if (renderTarget)
	{
		Color color;
		renderTarget->GetPixel(x, y, color);
		r = color.r;
		g = color.g;
		b = color.b;
		a = color.a;
	}
}

extern "C" __declspec(dllexport) void SetRenderWidth(int width)
{
	renderWidth = width;

	if (!isInitialized)
		return;
	if (scene)
	{
		Camera* camera = scene->GetCamera();
		if (camera)
		{ 
			camera->SetRenderWidth(width);
		}
	}
}

extern "C" __declspec(dllexport) unsigned int GetRenderWidth()
{
	return renderWidth;
}

extern "C" __declspec(dllexport) void SetRenderHeight(int height)
{
	renderHeight = height;

	if (!isInitialized)
		return;
	if (scene)
	{
		Camera* camera = scene->GetCamera();
		if (camera)
		{
			camera->SetRenderHeight(height);
		}
	}
}

extern "C" __declspec(dllexport) unsigned int GetRenderHeight()
{
	return renderHeight;
}

extern "C" __declspec(dllexport) void SetIntegrateType(int type)
{
	integrateType = type;
}

extern "C" __declspec(dllexport) int GetIntegrateType()
{
	return integrateType;
}

extern "C" __declspec(dllexport) void SetPhotonMapperBounce(int bounce)
{
	if (photonMapper != nullptr)
		photonMapper->maxBounce = bounce;
}

extern "C" __declspec(dllexport) int GetPhotonMapperBounce()
{
	if (photonMapper != nullptr)
		return photonMapper->maxBounce;
	return 0;
}

extern "C" __declspec(dllexport) void SetPhotonMapperSampler(int sampler)
{
	if (photonMapper != nullptr)
		photonMapper->samplerType = (RayTracer::SamplerType)sampler;
}

extern "C" __declspec(dllexport) int GetPhotonMapperSampler()
{
	if (photonMapper != nullptr)
		return (int)photonMapper->samplerType;
	return 0;
}

extern "C" __declspec(dllexport) void SetPhotonMapperEmissions(float emissions)
{
	if (photonMapper != nullptr)
		photonMapper->photonEmissions = emissions;
}

extern "C" __declspec(dllexport) float GetPhotonMapperEmissions()
{
	if (photonMapper != nullptr)
		return photonMapper->photonEmissions;
	return 0;
}

extern "C" __declspec(dllexport) void SetPhotonMapperCausticFactor(float causticFactor)
{
	if (photonMapper != nullptr)
		photonMapper->causticFactor = causticFactor;
}

extern "C" __declspec(dllexport) float GetPhotonMapperCausticFactor()
{
	if (photonMapper != nullptr)
		return photonMapper->causticFactor;
	return 0;
}

extern "C" __declspec(dllexport) void SetPhotonMapperMaxRadius(float maxRadius)
{
	if (photonMapper != nullptr)
		photonMapper->maxRadius = maxRadius;
}

extern "C" __declspec(dllexport) float GetPhotonMapperMaxRadius()
{
	if (photonMapper != nullptr)
		return photonMapper->maxRadius;
	return 0;
}

extern "C" __declspec(dllexport) void SetPhotonMapperMaxCausticRadius(float maxCausticRadius)
{
	if (photonMapper != nullptr)
		photonMapper->maxCausticRadius = maxCausticRadius;
}

extern "C" __declspec(dllexport) float GetPhotonMapperMaxCausticRadius()
{
	if (photonMapper != nullptr)
		return photonMapper->maxCausticRadius;
	return 0;
}

extern "C" __declspec(dllexport) void SetPhotonMapperNearestPhotons(int nearestPhotons)
{
	if (photonMapper != nullptr)
		photonMapper->nearestPhotons = nearestPhotons;
}

extern "C" __declspec(dllexport) int GetPhotonMapperNearestPhotons()
{
	if (photonMapper != nullptr)
		return photonMapper->nearestPhotons;
	return 0;
}

extern "C" __declspec(dllexport) void SetPathTracerBounce(int bounce)
{
	if (pathTracer != nullptr)
		pathTracer->bounce = bounce;
}

extern "C" __declspec(dllexport) int GetPathTracerBounce()
{
	if (pathTracer != nullptr)
		return pathTracer->bounce;
	return 0;
}

extern "C" __declspec(dllexport) void SetPathTracerSampleNums(int sampleNums)
{
	if (pathTracer != nullptr)
		pathTracer->sampleNums = sampleNums;
}

extern "C" __declspec(dllexport) int GetPathTracerSampleNums()
{
	if (pathTracer != nullptr)
		return pathTracer->sampleNums;
	return 0;
}

extern "C" __declspec(dllexport) void SetPathTracerSampler(int sampler)
{
	if (pathTracer != nullptr)
		pathTracer->samplerType = (RayTracer::SamplerType)sampler;
}

extern "C" __declspec(dllexport) int GetPathTracerSampler()
{
	if (pathTracer != nullptr)
		return (int)pathTracer->samplerType;
	return 0;
}

extern "C" __declspec(dllexport) int CreateEnvironmentMapSkyLight()
{
	if (!isInitialized)
		return -1;
	EnvironmentMapSkyLight* skyLight = new EnvironmentMapSkyLight(glContext);

	scene->SetSkyLight(skyLight);

	return skylightPool->Add(skyLight);
}

extern "C" __declspec(dllexport) void SetEnvironmentMapSkyLightTexture(int skylightID, int textureID)
{
	if (!isInitialized)
		return;
	SkyLight* skylight = skylightPool->Get(skylightID);
	if (skylight != nullptr)
	{
		Texture* texture = GlobalResource::GetTexture(textureID);
		((EnvironmentMapSkyLight*)skylight)->SetEnvironmentMap(texture);
	}
}

extern "C" __declspec(dllexport) void SetEnvironmentMapSkyLightColor(int skylightID, float r, float g, float b, float a)
{
	if (!isInitialized)
		return;
	SkyLight* skylight = skylightPool->Get(skylightID);
	if (skylight != nullptr)
	{
		((EnvironmentMapSkyLight*)skylight)->SetEnvironmentColor(Color(r, g, b, a));
	}
}

extern "C" __declspec(dllexport) void SetEnvironmentMapSkyLightIntensity(int skylightID, float intensity)
{
	if (!isInitialized)
		return;
	SkyLight* skylight = skylightPool->Get(skylightID);
	if (skylight != nullptr)
	{
		((EnvironmentMapSkyLight*)skylight)->SetEnvironmentIntensity(intensity);
	}
}

extern "C" __declspec(dllexport) void RemoveSkyLight(int skyLightID)
{
	if (!isInitialized)
		return;
	SkyLight* skyLight = skylightPool->Get(skyLightID);
	if (skyLight)
	{
		SkyLight* activeSkyLight = scene->GetSkyLight();
		if (activeSkyLight == skyLight)
			scene->SetSkyLight(nullptr);
		skylightPool->Delete(skyLightID);
	}
}

extern "C" __declspec(dllexport) int CreateSunLight()
{
	if (!isInitialized)
		return -1;
	SunLight* sunLight = new SunLight(glContext);

	scene->SetSunLight(sunLight);

	return sunlightPool->Add(sunLight);
}

extern "C" __declspec(dllexport) void RemoveSunLight(int sunLightID)
{
	if (!isInitialized)
		return;
	SunLight* sunLight = sunlightPool->Get(sunLightID);
	if (sunLight)
	{
		SunLight* activeSunLight = scene->GetSunLight();
		if (activeSunLight == sunLight)
			scene->SetSunLight(nullptr);
		sunlightPool->Delete(sunLightID);
	}
}

extern "C" __declspec(dllexport) void SetSunLightEuler(int sunlightID, double x, double y, double z)
{
	if (!isInitialized)
		return;
	SunLight* sunLight = sunlightPool->Get(sunlightID);
	if (sunLight == nullptr)
		return;
	sunLight->SetEuler(Vector3(x,y,z));
}

extern "C" __declspec(dllexport) void GetSunLightEuler(int sunlightID, double& x, double& y, double& z)
{
	if (!isInitialized)
		return;
	SunLight* sunLight = sunlightPool->Get(sunlightID);
	if (sunLight == nullptr)
		return;
	Vector3 euler;
	euler = sunLight->GetEuler();
	x = euler.x;
	y = euler.y;
	z = euler.z;
}

extern "C" __declspec(dllexport) void SetSunLightIntensity(int sunlightID, float intensity)
{
	if (!isInitialized)
		return;
	SunLight* sunLight = sunlightPool->Get(sunlightID);
	if (sunLight == nullptr)
		return;
	sunLight->sunIntensity = intensity;
}

extern "C" __declspec(dllexport) float GetSunLightIntensity(int sunlightID)
{
	if (!isInitialized)
		return 0;
	SunLight* sunLight = sunlightPool->Get(sunlightID);
	if (sunLight == nullptr)
		return 0;
	return sunLight->sunIntensity;
}

extern "C" __declspec(dllexport) void SetSunLightShadowBias(int sunlightID, float shadowBias)
{
	if (!isInitialized)
		return;
	SunLight* sunLight = sunlightPool->Get(sunlightID);
	if (sunLight == nullptr)
		return;
	sunLight->shadowBias = shadowBias;
}

extern "C" __declspec(dllexport) float GetSunLightShadowBias(int sunlightID)
{
	if (!isInitialized)
		return 0;
	SunLight* sunLight = sunlightPool->Get(sunlightID);
	if (sunLight == nullptr)
		return 0;
	return sunLight->shadowBias;
}

extern "C" __declspec(dllexport) void SetSunLightColor(int sunlightID, float r, float g, float b)
{
	if (!isInitialized)
		return;
	SunLight* sunLight = sunlightPool->Get(sunlightID);
	if (sunLight == nullptr)
		return;
	sunLight->sunColor = Color(r,g,b,1);
}

extern "C" __declspec(dllexport) void GetSunLightColor(int sunlightID, float& r, float& g, float& b)
{
	if (!isInitialized)
		return;
	SunLight* sunLight = sunlightPool->Get(sunlightID);
	if (sunLight == nullptr)
		return;
	r = sunLight->sunColor.r;
	g = sunLight->sunColor.g;
	b = sunLight->sunColor.b;
}

extern "C" __declspec(dllexport) int CreateCamera()
{
	if (!isInitialized)
		return -1;
	Camera* camera = new Camera(renderWidth, renderHeight);

	scene->SetCamera(camera);

	return cameraPool->Add(camera);
}

extern "C" __declspec(dllexport) void RemoveCamera(int cameraID)
{
	if (!isInitialized)
		return;
	Camera* camera = cameraPool->Get(cameraID);
	if (camera)
	{
		Camera* activeCamera = scene->GetCamera();
		if (activeCamera == camera)
			scene->SetCamera(nullptr);
		cameraPool->Delete(cameraID);
	}
}

extern "C" __declspec(dllexport) int GetActiveCamera(int originID)
{
	if (!isInitialized)
		return originID;
	if (scene != nullptr)
	{
		Camera* camera = scene->GetCamera();
		Camera* originCamera = cameraPool->Get(originID);
		if (camera != nullptr && originCamera != nullptr && camera != originCamera)
		{
			return cameraPool->GetID(camera);
		}
	}

	return originID;
}

extern "C" __declspec(dllexport) void SetCameraPosition(int cameraID, double x, double y, double z)
{
	if (!isInitialized)
		return;
	Camera* camera = cameraPool->Get(cameraID);
	if (camera)
		camera->SetPosition(Vector3(x, y, z));
}

extern "C" __declspec(dllexport) void GetCameraPosition(int cameraID, double& x, double& y, double& z)
{
	if (!isInitialized)
		return;
	Camera* camera = cameraPool->Get(cameraID);
	if (camera)
	{
		Vector3 position = camera->GetPosition();
		x = position.x;
		y = position.y;
		z = position.z;
	}
}

extern "C" __declspec(dllexport) void SetCameraEuler(int cameraID, double x, double y, double z)
{
	if (!isInitialized)
		return;
	Camera* camera = cameraPool->Get(cameraID);
	if (camera)
		camera->SetEuler(Vector3(x, y, z));
}

extern "C" __declspec(dllexport) void GetCameraEuler(int cameraID, double& x, double& y, double& z)
{
	if (!isInitialized)
		return;
	Camera* camera = cameraPool->Get(cameraID);
	if (camera)
	{
		Vector3 euler = camera->GetEuler();
		x = euler.x;
		y = euler.y;
		z = euler.z;
	}
}

extern "C" __declspec(dllexport) void SetCameraFOV(int cameraID, double fov)
{
	if (!isInitialized)
		return;
	Camera* camera = cameraPool->Get(cameraID);
	if (camera)
		camera->SetFOV(fov);
}

extern "C" __declspec(dllexport) float GetCameraFOV(int cameraID)
{
	if (!isInitialized)
		return 0;
	Camera* camera = cameraPool->Get(cameraID);
	if (camera)
	{
		return camera->GetFOV();
	}
	return 0;
}

extern "C" __declspec(dllexport) void SetCameraUseThinLens(int cameraID, int useThinLens)
{
	if (!isInitialized)
		return;
	Camera* camera = cameraPool->Get(cameraID);
	if (camera)
		camera->useThinLens = useThinLens == 1;
}

extern "C" __declspec(dllexport) int GetCameraUseThinLens(int cameraID)
{
	if (!isInitialized)
		return 0;
	Camera* camera = cameraPool->Get(cameraID);
	if (camera)
	{
		return camera->useThinLens ? 1 : 0;
	}
	return 0;
}

extern "C" __declspec(dllexport) void SetCameraFocal(int cameraID, float focal)
{
	if (!isInitialized)
		return;
	Camera* camera = cameraPool->Get(cameraID);
	if (camera)
		camera->focal = focal;
}

extern "C" __declspec(dllexport) float GetCameraFocal(int cameraID)
{
	if (!isInitialized)
		return 0;
	Camera* camera = cameraPool->Get(cameraID);
	if (camera)
	{
		return camera->focal;
	}
	return 0;
}

extern "C" __declspec(dllexport) void SetCameraRadius(int cameraID, float radius)
{
	if (!isInitialized)
		return;
	Camera* camera = cameraPool->Get(cameraID);
	if (camera)
		camera->radius = radius;
}

extern "C" __declspec(dllexport) float GetCameraRadius(int cameraID)
{
	if (!isInitialized)
		return 0;
	Camera* camera = cameraPool->Get(cameraID);
	if (camera)
	{
		return camera->radius;
	}
	return 0;
}

extern "C" __declspec(dllexport) void GetCameraRight(int cameraID, double& x, double& y, double& z)
{
	if (!isInitialized)
		return;
	Camera* camera = cameraPool->Get(cameraID);
	if (camera)
	{
		Vector3 right = camera->GetRight();
		x = right.x;
		y = right.y;
		z = right.z;
	}
}

extern "C" __declspec(dllexport) void GetCameraUp(int cameraID, double& x, double& y, double& z)
{
	if (!isInitialized)
		return;
	Camera* camera = cameraPool->Get(cameraID);
	if (camera)
	{
		Vector3 up = camera->GetUp();
		x = up.x;
		y = up.y;
		z = up.z;
	}
}

extern "C" __declspec(dllexport) void GetCameraForward(int cameraID, double& x, double& y, double& z)
{
	if (!isInitialized)
		return;
	Camera* camera = cameraPool->Get(cameraID);
	if (camera)
	{
		Vector3 forward = camera->GetForward();
		x = forward.x;
		y = forward.y;
		z = forward.z;
	}
}

extern "C" __declspec(dllexport) void RefreshCameraTransform(int cameraID)
{
	if (!isInitialized)
		return;
	Camera* camera = cameraPool->Get(cameraID);
	if (camera)
		camera->ForceRefreshTransform();
}

extern "C" __declspec(dllexport) int CreatePBRStandardShader()
{
	if (!isInitialized)
		return -1;
	int shaderID = -1;
	GlobalResource::CreateShader<PBR_StardardShader>(shaderID);
	return shaderID;
}

extern "C" __declspec(dllexport) int CreateEmissiveShader()
{
	if (!isInitialized)
		return -1;
	int shaderID = -1;
	GlobalResource::CreateShader<EmissiveShader>(shaderID);
	return shaderID;
}

extern "C" __declspec(dllexport) int CreateTransparencyShader()
{
	if (!isInitialized)
		return -1;
	int shaderID = -1;
	GlobalResource::CreateShader<TransparencyShader>(shaderID);
	return shaderID;
}

extern "C" __declspec(dllexport) bool DestroyShader(int shaderID)
{
	if (!isInitialized)
		return false;
	return GlobalResource::RemoveShader(shaderID);
}

extern "C" __declspec(dllexport) void SetShaderTexture(int shaderID, const char* key, int textureID)
{
	if (!isInitialized)
		return;
	PBRShaderBase* shader = GlobalResource::GetShader<PBRShaderBase>(shaderID);
	if (shader)
	{
		Texture* texture = GlobalResource::GetTexture(textureID);
		shader->SetTexture(key, texture);
	}
}

extern "C" __declspec(dllexport) void GetShaderVector2(int shaderID, const char* key, double& x, double& y)
{
	if (!isInitialized)
		return;
	ShaderBase* shader = GlobalResource::GetShader<ShaderBase>(shaderID);
	if (shader)
	{
		Vector2 vector = shader->GetVector2(key);
		x = vector.x;
		y = vector.y;
	}
}

extern "C" __declspec(dllexport) void SetShaderVector2(int shaderID, const char* key, double x, double y)
{
	if (!isInitialized)
		return;
	ShaderBase* shader = GlobalResource::GetShader<ShaderBase>(shaderID);
	if (shader)
	{
		shader->SetVector2(key, Vector2(x, y));
	}
}

extern "C" __declspec(dllexport) void GetShaderVector3(int shaderID, const char* key, double& x, double& y, double& z)
{
	if (!isInitialized)
		return;
	ShaderBase* shader = GlobalResource::GetShader<ShaderBase>(shaderID);
	if (shader)
	{
		Vector3 vector = shader->GetVector3(key);
		x = vector.x;
		y = vector.y;
		z = vector.z;
	}
}

extern "C" __declspec(dllexport) void SetShaderVector3(int shaderID, const char* key, double x, double y, double z)
{
	if (!isInitialized)
		return;
	ShaderBase* shader = GlobalResource::GetShader<ShaderBase>(shaderID);
	if (shader)
	{
		shader->SetVector3(key, Vector3(x, y, z));
	}
}

extern "C" __declspec(dllexport) void GetShaderVector4(int shaderID, const char* key, double& x, double& y, double& z, double& w)
{
	if (!isInitialized)
		return;
	ShaderBase* shader = GlobalResource::GetShader<ShaderBase>(shaderID);
	if (shader)
	{
		Vector4 vector = shader->GetVector4(key);
		x = vector.x;
		y = vector.y;
		z = vector.z;
		w = vector.w;
	}
}

extern "C" __declspec(dllexport) void SetShaderVector4(int shaderID, const char* key, double x, double y, double z, double w)
{
	if (!isInitialized)
		return;
	ShaderBase* shader = GlobalResource::GetShader<ShaderBase>(shaderID);
	if (shader)
	{
		shader->SetVector4(key, Vector4(x, y, z, w));
	}
}

extern "C" __declspec(dllexport) void GetShaderColor(int shaderID, const char* key, float& r, float& g, float& b, float& a)
{
	if (!isInitialized)
		return;
	ShaderBase* shader = GlobalResource::GetShader<ShaderBase>(shaderID);
	if (shader)
	{
		Color color = shader->GetColor(key);
		r = color.r;
		g = color.g;
		b = color.b;
		a = color.a;
	}
}

extern "C" __declspec(dllexport) void SetShaderColor(int shaderID, const char* key, float r, float g, float b, float a)
{
	if (!isInitialized)
		return;
	ShaderBase* shader = GlobalResource::GetShader<ShaderBase>(shaderID);
	if (shader)
	{
		shader->SetColor(key, Color(r,g,b,a));
	}
}

extern "C" __declspec(dllexport) float GetShaderFloat(int shaderID, const char* key)
{
	if (!isInitialized)
		return 0.0f;
	ShaderBase* shader = GlobalResource::GetShader<ShaderBase>(shaderID);
	if (shader)
	{
		return shader->GetFloat(key);
	}
	return 0.0f;
}

extern "C" __declspec(dllexport) void SetShaderFloat(int shaderID, const char* key, float v)
{
	if (!isInitialized)
		return;
	ShaderBase* shader = GlobalResource::GetShader<ShaderBase>(shaderID);
	if (shader)
	{
		shader->SetFloat(key, v);
	}
}

extern "C" __declspec(dllexport) int CreateTexture(unsigned int width, unsigned int height)
{
	if (!isInitialized)
		return -1;
	int textureID = -1;
	Texture* texture = GlobalResource::CreateTexture(width, height, textureID);
	if (texture == nullptr)
		return -1;
	return textureID;
}

extern "C" __declspec(dllexport) int CreateTextureFromFile(const char* path, bool isLinear)
{
	if (!isInitialized)
		return -1;
	int textureID = -1;
	Texture* texture = GlobalResource::CreateTexture(path, isLinear, textureID);
	if (texture == nullptr)
		return -1;
	return textureID;
}

extern "C" __declspec(dllexport) bool DestroyTexture(int textureID)
{
	if (!isInitialized)
		return false;
	return GlobalResource::RemoveTexture(textureID);
}

extern "C" __declspec(dllexport) unsigned int GetTextureWidth(int textureID)
{
	if (!isInitialized)
		return 0;
	Texture* texture = GlobalResource::GetTexture(textureID);
	if (texture == nullptr)
		return 0;
	return texture->GetWidth();
}

extern "C" __declspec(dllexport) unsigned int GetTextureHeight(int textureID)
{
	if (!isInitialized)
		return 0;
	Texture* texture = GlobalResource::GetTexture(textureID);
	if (texture == nullptr)
		return 0;
	return texture->GetWidth();
}

extern "C" __declspec(dllexport) void FillTexture(int textureID, float r, float g, float b, float a)
{
	if (!isInitialized)
		return;
	Texture* texture = GlobalResource::GetTexture(textureID);
	if (texture == nullptr)
		return;
	texture->Fill(Color(r,g,b,a));
}

extern "C" __declspec(dllexport) void SetTexturePixel(int textureID, int x, int y, float r, float g, float b, float a)
{
	if (!isInitialized)
		return;
	Texture* texture = GlobalResource::GetTexture(textureID);
	if (texture == nullptr)
		return;
	texture->SetPixel(x, y, Color(r, g, b, a));
}

extern "C" __declspec(dllexport) void GetTexturePixel(int textureID, int x, int y, float& r, float& g, float& b, float& a)
{
	if (!isInitialized)
		return;
	Texture* texture = GlobalResource::GetTexture(textureID);
	if (texture == nullptr)
		return;
	Color col;
	texture->GetPixel(x, y, col);
	r = col.r;
	g = col.g;
	b = col.b;
	a = col.a;
}

extern "C" __declspec(dllexport) void ApplyTextureData(int textureID)
{
	if (!isInitialized)
		return;
	Texture* texture = GlobalResource::GetTexture(textureID);
	if (texture == nullptr)
		return;
	texture->ApplyData();
}

extern "C" __declspec(dllexport) void SetTextureFilterMode(int textureID, int filterMode)
{
	if (!isInitialized)
		return;
	Texture* texture = GlobalResource::GetTexture(textureID);
	if (texture == nullptr)
		return;
	texture->SetFilterMode((FilterMode)filterMode);
}

extern "C" __declspec(dllexport) int GetTextureFilterMode(int textureID)
{
	if (!isInitialized)
		return 0;
	Texture* texture = GlobalResource::GetTexture(textureID);
	if (texture == nullptr)
		return 0;
	return (int)texture->GetFilterMode();
}

extern "C" __declspec(dllexport) void SetTextureWrapMode(int textureID, int wrapMode)
{
	if (!isInitialized)
		return;
	Texture* texture = GlobalResource::GetTexture(textureID);
	if (texture == nullptr)
		return;
	texture->SetWrapMode((WrapMode)wrapMode);
}

extern "C" __declspec(dllexport) int GetTextureWrapMode(int textureID)
{
	if (!isInitialized)
		return 0;
	Texture* texture = GlobalResource::GetTexture(textureID);
	if (texture == nullptr)
		return 0;
	return (int)texture->GetWrapMode();
}

extern "C" __declspec(dllexport) int CreateMesh(unsigned int vertexCount, unsigned int indexCount)
{
	if (!isInitialized)
		return -1;
	int meshID = -1;
	Mesh* mesh = GlobalResource::CreateMesh(vertexCount, indexCount, meshID);
	if (mesh == nullptr)
		return -1;
	return meshID;
}

extern "C" __declspec(dllexport) void CreateMeshesFromFile(const char* path, int& startMeshID, int& endMeshID)
{
	startMeshID = -1;
	endMeshID = -1;
	if (!isInitialized)
		return;
	Mesh::CreateMeshesFromFile(path, startMeshID, endMeshID);
}

extern "C" __declspec(dllexport) bool DestroyMesh(int meshID)
{
	if (!isInitialized)
		return false;
	return GlobalResource::RemoveMesh(meshID);
}

extern "C" __declspec(dllexport) unsigned int GetMeshVertexCount(int meshID)
{
	if (!isInitialized)
		return 0;
	Mesh* mesh = GlobalResource::GetMesh(meshID);
	if (mesh)
	{
		return mesh->GetVertexCount();
	}
	return 0;
}

extern "C" __declspec(dllexport) unsigned int GetMeshIndexCount(int meshID)
{
	if (!isInitialized)
		return 0;
	Mesh* mesh = GlobalResource::GetMesh(meshID);
	if (mesh)
	{
		return mesh->GetIndexCount();
	}
	return 0;
}

extern "C" __declspec(dllexport) void SetMeshVertex(int meshID, int i, double x, double y, double z)
{
	if (!isInitialized)
		return;
	Mesh* mesh = GlobalResource::GetMesh(meshID);
	if (mesh)
	{
		mesh->SetVertex(Vector3(x,y,z), i);
	}
}

extern "C" __declspec(dllexport) void SetMeshNormal(int meshID, int i, double x, double y, double z)
{
	if (!isInitialized)
		return;
	Mesh* mesh = GlobalResource::GetMesh(meshID);
	if (mesh)
	{
		mesh->SetNormal(Vector3(x, y, z), i);
	}
}

extern "C" __declspec(dllexport) void SetMeshTangent(int meshID, int i, double x, double y, double z, double w)
{
	if (!isInitialized)
		return;
	Mesh* mesh = GlobalResource::GetMesh(meshID);
	if (mesh)
	{
		mesh->SetTangent(Vector4(x, y, z, w), i);
	}
}

extern "C" __declspec(dllexport) void SetMeshUV(int meshID, int i, int channel, double x, double y)
{
	if (!isInitialized)
		return;
	Mesh* mesh = GlobalResource::GetMesh(meshID);
	if (mesh)
	{
		mesh->SetUV(Vector2(x, y), channel, i);
	}
}

extern "C" __declspec(dllexport) void SetMeshIndex(int meshID, int i, unsigned int index)
{
	if (!isInitialized)
		return;
	Mesh* mesh = GlobalResource::GetMesh(meshID);
	if (mesh)
	{
		mesh->SetIndex(index, i);
	}
}

extern "C" __declspec(dllexport) int CreateCubePrimitive()
{
	if (!isInitialized)
		return -1;

	CubePrimitive* cube = new CubePrimitive(glContext);

	scene->AddPrimitive(cube);

	return primitiveObjectPool->Add(cube);
}

extern "C" __declspec(dllexport) int CreateSpherePrimitive()
{
	if (!isInitialized)
		return -1;

	SpherePrimitive* sphere = new SpherePrimitive(glContext);

	scene->AddPrimitive(sphere);

	return primitiveObjectPool->Add(sphere);
}

extern "C" __declspec(dllexport) int CreatePlanePrimitive()
{
	if (!isInitialized)
		return -1;

	PlanePrimitive* plane = new PlanePrimitive(glContext);

	scene->AddPrimitive(plane);

	return primitiveObjectPool->Add(plane);
}

extern "C" __declspec(dllexport) int CreateMeshPrimitive()
{
	if (!isInitialized)
		return -1;

	MeshPrimitive* primitive = new MeshPrimitive(glContext);

	scene->AddPrimitive(primitive);

	return primitiveObjectPool->Add(primitive);
}

extern "C" __declspec(dllexport) void SetMeshToMeshPrimitive(int primitiveID, int meshID)
{
	if (!isInitialized)
		return;
	PrimitiveBase* primitive = primitiveObjectPool->Get(primitiveID);
	if (primitive == nullptr)
		return;
	Mesh* mesh = GlobalResource::GetMesh(meshID);
	MeshPrimitive* meshPrimitive = (MeshPrimitive*)primitive;
	meshPrimitive->SetMesh(mesh);
}

extern "C" __declspec(dllexport) void SelectPrimitive(int primitiveID)
{
	if (!isInitialized)
		return;

	PrimitiveBase* selectedPrimitive = primitiveObjectPool->Get(selectedPrimitiveID);
	if (selectedPrimitive != nullptr)
		selectedPrimitive->displayBounds = false;
	PrimitiveBase* primitive = primitiveObjectPool->Get(primitiveID);
	if (primitive != nullptr)
		primitive->displayBounds = true;
	selectedPrimitiveID = primitiveID;
}

extern "C" __declspec(dllexport) void SetPrimitiveMaterialShader(int primitiveID, int materialShaderID)
{
	if (!isInitialized)
		return;
	PrimitiveBase* primitive = primitiveObjectPool->Get(primitiveID);
	MaterialShader* shader = GlobalResource::GetShader<MaterialShader>(materialShaderID);
	if (primitive)
	{
		if (shader)
			primitive->material = shader;
		else
			primitive->material = nullptr;
	}
}

extern "C" __declspec(dllexport) void SetPrimitivePosition(int primitiveID, double x, double y, double z)
{
	if (!isInitialized)
		return;
	PrimitiveBase* primitive = primitiveObjectPool->Get(primitiveID);
	if (primitive)
		primitive->position = Vector3(x, y, z);
}

extern "C" __declspec(dllexport) void GetPrimitivePosition(int primitiveID, double& x, double& y, double& z)
{
	if (!isInitialized)
		return;
	PrimitiveBase* primitive = primitiveObjectPool->Get(primitiveID);
	if (primitive)
	{
		x = primitive->position.x;
		y = primitive->position.y;
		z = primitive->position.z;
	}
}

extern "C" __declspec(dllexport) void SetPrimitiveEuler(int primitiveID, double x, double y, double z)
{
	if (!isInitialized)
		return;
	PrimitiveBase* primitive = primitiveObjectPool->Get(primitiveID);
	if (primitive)
		primitive->euler = Vector3(x, y, z);
}

extern "C" __declspec(dllexport) void GetPrimitiveEuler(int primitiveID, double& x, double& y, double& z)
{
	if (!isInitialized)
		return;
	PrimitiveBase* primitive = primitiveObjectPool->Get(primitiveID);
	if (primitive)
	{
		x = primitive->euler.x;
		y = primitive->euler.y;
		z = primitive->euler.z;
	}
}

extern "C" __declspec(dllexport) void SetPrimitiveScale(int primitiveID, double x, double y, double z)
{
	if (!isInitialized)
		return;
	PrimitiveBase* primitive = primitiveObjectPool->Get(primitiveID);
	if (primitive)
		primitive->scale = Vector3(x, y, z);
}

extern "C" __declspec(dllexport) void GetPrimitiveScale(int primitiveID, double& x, double& y, double& z)
{
	if (!isInitialized)
		return;
	PrimitiveBase* primitive = primitiveObjectPool->Get(primitiveID);
	if (primitive)
	{
		x = primitive->scale.x;
		y = primitive->scale.y;
		z = primitive->scale.z;
	}
}

extern "C" __declspec(dllexport) void RemovePrimitive(int objectID)
{
	if (!isInitialized)
		return;

	PrimitiveBase* primitive = primitiveObjectPool->Get(objectID);

	scene->RemovePrimitive(primitive);

	if (primitive != nullptr)
		primitiveObjectPool->Delete(objectID);
}

extern "C" __declspec(dllexport)bool Init(HWND hwnd, int width, int height)
{
	if (isInitialized)
		return false;

	windowWidth = width;
	windowHeight = height;

	renderWidth = width;
	renderHeight = height;

	glContext = new D3D11Context();
	if (!glContext->Init(hwnd, windowWidth, windowHeight))
		return false;

	isInitialized = true;

	FreeImage_Initialise();

	GlobalResource::Init(glContext);

	primitiveObjectPool = new ObjectPool<PrimitiveBase>();
	cameraPool = new ObjectPool<Camera>();
	skylightPool = new ObjectPool<SkyLight>();
	sunlightPool = new ObjectPool<SunLight>();

	scene = new Scene();

	realtimeRenderer = new RealtimeRenderer(glContext, scene);
	rayTracingPreviewRenderer = new RayTracingPreviewRenderer(glContext);
	rayTracerRenderer = new RayTracer::RayTracerRenderer(scene);
	
	pathTracer = new RayTracer::PathTracer(rayTracingPreviewRenderer);
	photonMapper = new RayTracer::PhotonMapper(rayTracingPreviewRenderer);

	return isInitialized;
}

extern "C" __declspec(dllexport)void UpdateRenderer()
{
	if (!isInitialized)
		return;
	if (realtimeRenderer == nullptr || rayTracerRenderer == nullptr)
		return;
	//if (rayTracerRenderer->IsRayTracingFinish() == false)
	//{
	//	rayTracerRenderer->UpdateFrame(windowWidth, windowHeight, renderWidth, renderHeight);
	//	return;
	//}
	realtimeRenderer->RenderFrame(windowWidth, windowHeight, renderWidth, renderHeight);
}

extern "C" __declspec(dllexport)void Destroy()
{
	if (realtimeRenderer)
		delete realtimeRenderer;
	realtimeRenderer = nullptr;
	if (rayTracerRenderer)
		delete rayTracerRenderer;
	rayTracerRenderer = nullptr;
	if (pathTracer)
		delete pathTracer;
	pathTracer = nullptr;
	if (photonMapper)
		delete photonMapper;
	photonMapper = nullptr;
	if (rayTracingPreviewRenderer)
		delete rayTracingPreviewRenderer;
	rayTracingPreviewRenderer = nullptr;
	if (scene)
		delete scene;
	scene = nullptr;
	/*if (camera)
		delete camera;
	camera = nullptr;*/
	if (glContext)
		delete glContext;
	glContext = nullptr;

	GlobalResource::Release();

	if (primitiveObjectPool)
		delete primitiveObjectPool;
	primitiveObjectPool = nullptr;
	if (skylightPool)
		delete skylightPool;
	skylightPool = nullptr;
	if (cameraPool)
		delete cameraPool;
	cameraPool = nullptr;
	if (sunlightPool)
		delete sunlightPool;
	sunlightPool = nullptr;

	FreeImage_DeInitialise();
}

//LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
//{
//	/*if (ImGui_ImplWin32_WndProcHandler(hwnd, umessage, wparam, lparam))
//		return true;*/
//	switch (umessage)
//	{
//		// Check if the window is being destroyed.
//	case WM_DESTROY:
//	{
//		PostQuitMessage(0);
//		return 0;
//	}
//
//	// Check if the window is being closed.
//	case WM_CLOSE:
//	{
//		HMENU hMenu;
//		hMenu = GetMenu(hwnd);
//		if (hMenu != NULL)
//			DestroyMenu(hMenu);
//		DestroyWindow(hwnd);
//		UnregisterClass(L"Test", NULL);
//		PostQuitMessage(0);
//		return 0;
//	}
//
//	// All other messages pass to the message handler in the system class.
//	default:
//	{
//		return DefWindowProc(hwnd, umessage, wparam, lparam);
//	}
//	}
//}
//
//bool Run()
//{
//	MSG msg;
//	ZeroMemory(&msg, sizeof(MSG));
//
//	bool done = false;
//	int result = 0;
//	while (!done)
//	{
//		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
//		{
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		}
//
//		if (msg.message == WM_QUIT)
//		{
//			done = true;
//		}
//		else
//		{
//			//result = Loop();
//			result = 1;
//			UpdateRenderer();
//			if (!result)
//			{
//				done = true;
//			}
//		}
//	}
//
//	return false;
//}
//
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int nCmdShow)
//{
//	WNDCLASSEX wc;
//	DEVMODE dmScreenSettings;
//	int posX, posY;
//	 
//	// Setup the windows class with default settings.
//	wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
//	wc.lpfnWndProc = WndProc;
//	wc.cbClsExtra = 0;
//	wc.cbWndExtra = 0;
//	wc.hInstance = hInstance;
//	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
//	wc.hIconSm = wc.hIcon;
//	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
//	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
//	wc.lpszMenuName = NULL;
//	wc.lpszClassName = L"Test";
//	wc.cbSize = sizeof(WNDCLASSEX);
//
//	// Register the window class.
//	RegisterClassEx(&wc);
//
//
//	int	screenWidth = 800;
//	int	screenHeight = 600;
//
//	/*int	screenWidth = 1000;
//	int	screenHeight = 600;*/
//
//	// Place the window in the middle of the screen.
//	posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
//	posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
//
//	RECT rc = { 0, 0, screenWidth, screenHeight };
//	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
//
//	// Create the window with the screen settings and get the handle to it.
//	HWND hwnd = CreateWindow(L"Test", L"Test",
//		WS_OVERLAPPEDWINDOW,
//		posX, posY, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hInstance, NULL);
//
//	// Bring the window up on the screen and set it as main focus.
//	ShowWindow(hwnd, SW_SHOW);
//	//SetForegroundWindow(hwnd);
//	SetFocus(hwnd);
//
//	if (Init(hwnd, screenWidth, screenHeight))
//	{
//		SetRenderSize(1000,600);
//
//		Run();
//	}
//	Destroy();
//
//	DestroyWindow(hwnd);
//
//	return 0;
//}