#include "RenderTexture.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../RealtimeRender/GraphicsLib/TextureBuffer.h"

RenderTexture::RenderTexture(unsigned int width, unsigned int height, GLContext* glContext) : RenderTexture(width, height, false, glContext)
{
}

RenderTexture::RenderTexture(unsigned int width, unsigned int height, bool isShadowMap, GLContext* glContext) : TextureBase()
{
	m_width = width;
	m_height = height;

	m_colorBuffer = nullptr;
	m_depthBuffer = nullptr;
	m_textureBuffer = nullptr;
	glContext->CreateRenderBuffer(width, height, isShadowMap, &m_colorBuffer, &m_depthBuffer, &m_textureBuffer);

	if (m_textureBuffer)
	{
		m_textureBuffer->SetFilterMode(FilterMode::Point);
		m_textureBuffer->SetWrapMode(WrapMode::Repeat);
	}
	m_isShadowMap = isShadowMap;
}

RenderTexture::~RenderTexture()
{
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

CubeMapRenderTexture::CubeMapRenderTexture(unsigned int size, GLContext* glContext) : TextureBase()
{
	m_size = size;

	m_cubeMapColorBuffer = nullptr;
	m_depthBuffer = nullptr;
	m_textureBuffer = nullptr;
	glContext->CreateCubeRenderBuffer(size, &m_cubeMapColorBuffer, &m_depthBuffer, &m_textureBuffer);

	if (m_textureBuffer)
	{
		m_textureBuffer->SetFilterMode(FilterMode::Trilinear);
		m_textureBuffer->SetWrapMode(WrapMode::Repeat);
	}

	m_glContext = glContext;
}

CubeMapRenderTexture::~CubeMapRenderTexture()
{
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

void CubeMapRenderTexture::GenerateMipMap()
{
	if (m_textureBuffer)
	{
		m_glContext->GenerateMipmap(m_textureBuffer);
	}
}
