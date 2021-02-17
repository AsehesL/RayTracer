#include "RenderTexture.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../RealtimeRender/GraphicsLib/TextureBuffer.h"

RenderTexture::RenderTexture(unsigned int width, unsigned int height, GLContext* glContext) : RenderTexture(width, height, false, glContext)
{
}

RenderTexture::RenderTexture(unsigned int width, unsigned int height, bool isShadowMap, GLContext* glContext)
{
	m_width = width;
	m_height = height;
	m_filterMode = FilterMode::Point;
	m_wrapMode = WrapMode::Repeat;

	m_colorBuffer = nullptr;
	m_depthBuffer = nullptr;
	m_textureBuffer = nullptr;
	glContext->CreateRenderBuffer(width, height, isShadowMap, &m_colorBuffer, &m_depthBuffer, &m_textureBuffer);

	if (m_textureBuffer)
	{
		m_textureBuffer->SetFilterMode(m_filterMode);
		m_textureBuffer->SetWrapMode(m_wrapMode);
	}
	m_isShadowMap = isShadowMap;
}

RenderTexture::~RenderTexture()
{
	if (m_textureBuffer)
		delete m_textureBuffer;
	m_textureBuffer = nullptr;
	if (m_colorBuffer)
		delete m_colorBuffer;
	m_colorBuffer = nullptr;
	if (m_depthBuffer)
		delete m_depthBuffer;
	m_depthBuffer = nullptr;
}

unsigned int RenderTexture::GetWidth() const
{
	return m_width;
}

unsigned int RenderTexture::GetHeight() const
{
	return m_height;
}

bool RenderTexture::IsShadowMap() const
{
	return m_isShadowMap;
}

void RenderTexture::SetFilterMode(FilterMode filterMode)
{
	m_filterMode = filterMode;
	m_textureBuffer->SetFilterMode(filterMode);
}

FilterMode RenderTexture::GetFilterMode()
{
	return m_filterMode;
}

void RenderTexture::SetWrapMode(WrapMode wrapMode)
{
	m_wrapMode = wrapMode;
	m_textureBuffer->SetWrapMode(wrapMode);
}

WrapMode RenderTexture::GetWrapMode()
{
	return m_wrapMode;
}

void RenderTexture::SetShaderResource(unsigned int slot)
{
	if (m_textureBuffer)
	{
		m_textureBuffer->SetShaderResource(slot);
	}
}

CubeMapRenderTexture::CubeMapRenderTexture(unsigned int size, GLContext* glContext)
{
	m_size = size;
	m_filterMode = FilterMode::Trilinear;
	m_wrapMode = WrapMode::Repeat;

	m_cubeMapColorBuffer = nullptr;
	m_depthBuffer = nullptr;
	m_textureBuffer = nullptr;
	glContext->CreateCubeRenderBuffer(size, &m_cubeMapColorBuffer, &m_depthBuffer, &m_textureBuffer);

	if (m_textureBuffer)
	{
		m_textureBuffer->SetFilterMode(m_filterMode);
		m_textureBuffer->SetWrapMode(m_wrapMode);
	}

	m_glContext = glContext;
}

CubeMapRenderTexture::~CubeMapRenderTexture()
{
	if (m_textureBuffer)
		delete m_textureBuffer;
	m_textureBuffer = nullptr;
	if (m_cubeMapColorBuffer)
		delete m_cubeMapColorBuffer;
	m_cubeMapColorBuffer = nullptr;
	if (m_depthBuffer)
		delete m_depthBuffer;
	m_depthBuffer = nullptr;
}

unsigned int CubeMapRenderTexture::GetSize() const
{
	return m_size;
}

void CubeMapRenderTexture::SetFilterMode(FilterMode filterMode)
{
	m_filterMode = filterMode;
	m_textureBuffer->SetFilterMode(filterMode);
}

FilterMode CubeMapRenderTexture::GetFilterMode()
{
	return m_filterMode;
}

void CubeMapRenderTexture::SetWrapMode(WrapMode wrapMode)
{
	m_wrapMode = wrapMode;
	m_textureBuffer->SetWrapMode(wrapMode);
}

WrapMode CubeMapRenderTexture::GetWrapMode()
{
	return m_wrapMode;
}

void CubeMapRenderTexture::SetShaderResource(unsigned int slot)
{
	if (m_textureBuffer)
	{
		m_textureBuffer->SetShaderResource(slot);
	}
}

void CubeMapRenderTexture::GenerateMipMap()
{
	if (m_textureBuffer)
	{
		m_glContext->GenerateMipmap(m_textureBuffer);
	}
}
