#include "RayTracerRenderTarget.h"
#include "../../RealtimeRender/GraphicsLib/TextureBuffer.h"
#include "FreeImage.h"

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

void RayTracer::RenderTarget::Save(const char* path, bool isHDR)
{
	FIBITMAP* dib(0);

	if (isHDR)
	{ 
		dib = FreeImage_AllocateExT(FREE_IMAGE_TYPE::FIT_RGBF, m_width, m_height, 96, 0);
		for (unsigned int j = 0; j < m_height; j++)
		{
			auto bytes = FreeImage_GetScanLine(dib, j);
			for (unsigned int i = 0; i < m_width; ++i)
			{
				float linearR = m_colors[((m_height - 1 - j) * m_width + i) * 4];
				float linearG = m_colors[((m_height - 1 - j) * m_width + i) * 4 + 1];
				float linearB = m_colors[((m_height - 1 - j) * m_width + i) * 4 + 2];

				tagFIRGBF* data = (tagFIRGBF*)bytes;
				data->red = linearR;
				data->green = linearG;
				data->blue = linearB;
			}
		}
		FreeImage_Save(FREE_IMAGE_FORMAT::FIF_HDR, dib, path);
		FreeImage_Unload(dib);
	}
	else
	{
		FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(path);
		dib = FreeImage_AllocateExT(FREE_IMAGE_TYPE::FIT_BITMAP, m_width, m_height, 24, 0);
		for (unsigned int j = 0; j < m_height; j++)
		{
			auto bytes = FreeImage_GetScanLine(dib, j);
			for (unsigned int i = 0; i < m_width; ++i)
			{
				float linearR = m_colors[((m_height - 1 - j) * m_width + i) * 4];
				float linearG = m_colors[((m_height - 1 - j) * m_width + i) * 4 + 1];
				float linearB = m_colors[((m_height - 1 - j) * m_width + i) * 4 + 2];

				linearR = pow(linearR, 0.45f) * 255.0f;
				linearG = pow(linearG, 0.45f) * 255.0f;
				linearB = pow(linearB, 0.45f) * 255.0f;

				BYTE r = linearR;
				BYTE g = linearG;
				BYTE b = linearB;
				if (r < 0) r = 0;
				if (r > 255) r = 255;
				if (g < 0) g = 0;
				if (g > 255) g = 255;
				if (b < 0) b = 0;
				if (b > 255) b = 255;
				bytes[i * 3 + 2] = r;
				bytes[i * 3 + 1] = g;
				bytes[i * 3] = b;
			}
		}
		FreeImage_Save(fif, dib, path);
		FreeImage_Unload(dib);
	}
}
