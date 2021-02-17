#pragma once
#include "../RealtimeRender/GraphicsLib/ShaderResource.h"
#include "../RealtimeRender/GraphicsLib/GLDefines.h"

class RenderTexture : public IShaderResource
{
public:
	RenderTexture(unsigned int width, unsigned int height, class GLContext* glContext);
	RenderTexture(unsigned int width, unsigned int height, bool isShadowMap, class GLContext* glContext);
	~RenderTexture();
	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
	bool IsShadowMap() const;
	void SetFilterMode(FilterMode filterMode);
	FilterMode GetFilterMode();
	void SetWrapMode(WrapMode wrapMode);
	WrapMode GetWrapMode();
	class IColorBuffer* GetColorBuffer() const { return m_colorBuffer; }
	class IDepthBuffer* GetDepthBuffer() const { return m_depthBuffer; }
	virtual void SetShaderResource(unsigned int slot) override;

private:
	WrapMode m_wrapMode;
	FilterMode m_filterMode;

	int m_width, m_height;
	bool m_isShadowMap;
	class ITextureBuffer* m_textureBuffer;
	class IColorBuffer* m_colorBuffer;
	class IDepthBuffer* m_depthBuffer;
};

class CubeMapRenderTexture : public IShaderResource
{
public:
	CubeMapRenderTexture(unsigned int size, class GLContext* glContext);
	~CubeMapRenderTexture();
	unsigned int GetSize() const;
	void SetFilterMode(FilterMode filterMode);
	FilterMode GetFilterMode();
	void SetWrapMode(WrapMode wrapMode);
	WrapMode GetWrapMode();
	class ICubeMapColorBuffer* GetCubeMapColorBuffer() const { return m_cubeMapColorBuffer; }
	class IDepthBuffer* GetDepthBuffer() const { return m_depthBuffer; }
	virtual void SetShaderResource(unsigned int slot) override;
	void GenerateMipMap();

private:
	WrapMode m_wrapMode;
	FilterMode m_filterMode;

	int m_size;
	class ITextureBuffer* m_textureBuffer;
	class ICubeMapColorBuffer* m_cubeMapColorBuffer;
	class IDepthBuffer* m_depthBuffer;
	class GLContext* m_glContext;
};