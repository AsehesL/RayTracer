#include "RayTracerRenderTarget.h"
#include "../../RealtimeRender/GraphicsLib/TextureBuffer.h"

RayTracer::RenderTarget::RenderTarget(unsigned int width, unsigned int height) : m_lock()
{
	m_colors = new float[width * height * 4];
	m_isDirty = false;
	m_width = width;
	m_height = height;
}

RayTracer::RenderTarget::~RenderTarget()
{
	delete[] m_colors;
}

void RayTracer::RenderTarget::SetPixel(int x, int y, const Color& color)
{
	std::unique_lock<std::mutex> lock(m_lock);

	if (x >= 0 && x < m_width && y >= 0 && y < m_height)
	{
		m_colors[(y * m_width + x) * 4] = color.r;
		m_colors[(y * m_width + x) * 4 + 1] = color.g;
		m_colors[(y * m_width + x) * 4 + 2] = color.b;
		m_colors[(y * m_width + x) * 4 + 3] = color.a;
	}
	m_isDirty = true;

	lock.unlock();
}

void RayTracer::RenderTarget::GetPixel(int x, int y, Color& color)
{
	std::unique_lock<std::mutex> lock(m_lock);

	if (x >= 0 && x < m_width && y >= 0 && y < m_height)
	{
		color.r = m_colors[(y * m_width + x) * 4];
		color.g = m_colors[(y * m_width + x) * 4 + 1];
		color.b = m_colors[(y * m_width + x) * 4 + 2];
		color.a = m_colors[(y * m_width + x) * 4 + 3];
	}

	lock.unlock();
}

void RayTracer::RenderTarget::Fill(const Color& color)
{
	std::unique_lock<std::mutex> lock(m_lock);

	for (unsigned int i = 0; i < m_width; ++i)
	{
		for (unsigned int j = 0; j < m_height; ++j)
		{
			m_colors[(j * m_width + i) * 4] = color.r;
			m_colors[(j * m_width + i) * 4 + 1] = color.g;
			m_colors[(j * m_width + i) * 4 + 2] = color.b;
			m_colors[(j * m_width + i) * 4 + 3] = color.a;
		}
	}
	m_isDirty = true;

	lock.unlock();
}

void RayTracer::RenderTarget::ApplyData(ITextureBuffer* textureBuffer)
{
	std::unique_lock<std::mutex> lock(m_lock);

	if (m_isDirty && textureBuffer)
	{
		textureBuffer->Set2DBuffer(m_width, m_height, m_colors);
		m_isDirty = false;
	}

	lock.unlock();
}

unsigned int RayTracer::RenderTarget::GetWidth()
{
	return m_width;
}

unsigned int RayTracer::RenderTarget::GetHeight()
{
	return m_height;
}
