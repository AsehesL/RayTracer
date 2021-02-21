#pragma once
#include "TextureBase.h"

class RenderTexture : public TextureBase
{
public:
	RenderTexture(unsigned int width, unsigned int height, class GLContext* glContext);
	RenderTexture(unsigned int width, unsigned int height, bool isShadowMap, class GLContext* glContext);
	virtual ~RenderTexture() override;
	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
	bool IsShadowMap() const;
	class ColorBuffer* GetColorBuffer() const { return m_colorBuffer; }
	class DepthBuffer* GetDepthBuffer() const { return m_depthBuffer; }

private:
	int m_width, m_height;
	bool m_isShadowMap;
	class ColorBuffer* m_colorBuffer;
	class DepthBuffer* m_depthBuffer;
};

class CubeMapRenderTexture : public TextureBase
{
public:
	CubeMapRenderTexture(unsigned int size, class GLContext* glContext);
	virtual ~CubeMapRenderTexture() override;
	unsigned int GetSize() const;
	class CubeMapColorBuffer* GetCubeMapColorBuffer() const { return m_cubeMapColorBuffer; }
	class DepthBuffer* GetDepthBuffer() const { return m_depthBuffer; }
	void GenerateMipMap();

private:
	int m_size;
	class CubeMapColorBuffer* m_cubeMapColorBuffer;
	class DepthBuffer* m_depthBuffer;
	class GLContext* m_glContext;
};