#pragma once
#include "../Core/ObjectPool.hpp"
#include "../Primitive/Primitive.h"
#include "../Shader/ShaderBase.h"
#include "../Shader/ComputeShaderBase.h"
#include "../Primitive/Mesh.h"
#include "../Texture/Texture.h"
#include "../Texture/RenderTexture.h"
#include "../Shader/ScreenQuadShader.h"
#include "../Shader/LinearToGammaShader.h"
#include "../Shader/IntegrateBRDFShader.h"
#include "../Shader/ErrorShader.h"
#include <map>

class GlobalResource
{
public:
	static void Init(class GLContext* glContext);
	static void Release();

	static Mesh* GetCubeMesh();
	static Mesh* GetSphereMesh();
	static Mesh* GetPlaneMesh();
	static Mesh* GetScreenQuadMesh();
	static Mesh* GetSkySphereMesh();
	static Mesh* CreateMesh(unsigned int vertexCount, unsigned int indexCount, int& meshID);

	static Texture* GetBlackTexture();
	static Texture* GetWhiteTexture();
	static Texture* GetDefaultBumpMap();

	static RenderTexture* GetPreIntegratedBRDFTexture();

	template<class T>
	static T* CreateShader(int& shaderID);
	template<class T>
	static T* CreateShader();
	static bool RemoveShader(int shaderID);

	template<class T>
	static T* CreateComputeShader(int& computeShaderID);
	template<class T>
	static T* CreateComputeShader();
	static bool RemoveComputeShader(int computeShaderID);

	static Texture* CreateTexture(unsigned int width, unsigned int height, int& textureID);
	static Texture* CreateTexture(unsigned int width, unsigned int height);
	static Texture* CreateTexture(const char* path, bool isLinear, int& textureID);
	static Texture* CreateTexture(const char* path, bool isLinear);

	static bool RemoveTexture(int textureID);

	static Mesh* GetMesh(int meshID);

	static bool RemoveMesh(int meshID);

	template<class T>
	static T* GetShader(int shaderID);

	static Texture* GetTexture(int textureID);

	static ScreenQuadShader* GetScreenQuadShader();
	static LinearToGammaShader* GetLinearToGammaShader();
	static ErrorShader* GetErrorShader();

private:
	GlobalResource(class GLContext* glContext);
	~GlobalResource();

	static GlobalResource* GetInstance();

private:
	ObjectPool<ShaderBase>* m_shaderObjectPool = nullptr;
	ObjectPool<ComputeShaderBase>* m_computeShaderObjectPool = nullptr;
	ObjectPool<Mesh>* m_meshObjectPool = nullptr;
	ObjectPool<Texture>* m_texturePool = nullptr;

	Mesh* m_cubeMesh = nullptr;
	Mesh* m_sphereMesh = nullptr;
	Mesh* m_planeMesh = nullptr;
	Mesh* m_skySphereMesh = nullptr;
	Mesh* m_screenQuadMesh = nullptr;

	Texture* m_blackTexture = nullptr;
	Texture* m_whiteTexture = nullptr;
	Texture* m_defaultBumpMap = nullptr;
	RenderTexture* m_preIntegratedBRDFTexture = nullptr;

	ScreenQuadShader* m_screenQuadShader = nullptr;
	LinearToGammaShader* m_linearToGammaShader = nullptr;
	ErrorShader* m_errorShader = nullptr;
	IntegrateBRDFShader* m_integrateBRDFShader = nullptr;

	class GLContext* m_glContext;
};

template<class T>
inline T* GlobalResource::CreateShader(int& shaderID)
{
	shaderID = -1;
	GlobalResource* instance = GetInstance();
	if (instance == nullptr)
		return nullptr;
	T* shader = new T(instance->m_glContext);
	shaderID = instance->m_shaderObjectPool->Add(shader);
	return shader;
}

template<class T>
inline T* GlobalResource::CreateShader()
{
	int shaderID = -1;
	return CreateShader<T>(shaderID);
}

template<class T>
inline T* GlobalResource::CreateComputeShader(int& computeShaderID)
{
	computeShaderID = -1;
	GlobalResource* instance = GetInstance();
	if (instance == nullptr)
		return nullptr;
	T* computeShader = new T(instance->m_glContext);
	computeShaderID = instance->m_computeShaderObjectPool->Add(computeShader);
	return computeShader;
}

template<class T>
inline T* GlobalResource::CreateComputeShader()
{
	int computeShaderID = -1;
	return CreateComputeShader<T>(computeShaderID);
}

template<class T>
inline T* GlobalResource::GetShader(int shaderID)
{
	GlobalResource* instance = GetInstance();
	if (instance == nullptr)
		return nullptr;
	ShaderBase* shader = instance->m_shaderObjectPool->Get(shaderID);
	if (shader != nullptr)
		return (T*)shader;
	return nullptr;
}
