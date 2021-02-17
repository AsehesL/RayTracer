#include "Texture.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../RealtimeRender/GraphicsLib/TextureBuffer.h"
#include "FreeImage.h"

Texture::Texture(unsigned int width, unsigned int height, GLContext* glContext)
{
	m_width = width;
	m_height = height;
	m_filterMode = FilterMode::Point;
	m_wrapMode = WrapMode::Repeat;

	m_uDelta = 1.0f / width;
	m_vDelta = 1.0f / height;

	m_colors = new float[width * height * 4];

	m_textureBuffer = glContext->CreateTextureBuffer();

	m_textureBuffer->SetFilterMode(m_filterMode);
	m_textureBuffer->SetWrapMode(m_wrapMode);
}

Texture::Texture(const char* path, bool isLinear, GLContext* glContext)
{
	m_width = 0;
	m_height = 0;

	m_filterMode = FilterMode::Point;
	m_wrapMode = WrapMode::Repeat;

	m_colors = 0;

	m_textureBuffer = nullptr;
	m_uDelta = 0;
	m_vDelta = 0;

	float gamma = isLinear ? 1.0f : 2.2f;

	FIBITMAP* dib(0);

	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	fif = FreeImage_GetFileType(path, 0);
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(path);
	if (fif == FIF_UNKNOWN)
		return;

	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, path);

	if (!dib)
		return;

	unsigned int bpp = FreeImage_GetBPP(dib);
	if (bpp != 24 && bpp != 32 && bpp != 96 && bpp != 8)
	{
		FreeImage_Unload(dib);
		return;
	}

	m_width = FreeImage_GetWidth(dib);
	m_height = FreeImage_GetHeight(dib);

	m_uDelta = 1.0f / m_width;
	m_vDelta = 1.0f / m_height;


	m_colors = new float[m_width * m_height * 4];

	m_textureBuffer = glContext->CreateTextureBuffer();

	m_textureBuffer->SetFilterMode(m_filterMode);
	m_textureBuffer->SetWrapMode(m_wrapMode);

	for (unsigned int i = 0; i < m_height; ++i)
	{
		auto bytes = FreeImage_GetScanLine(dib, i);
		for (unsigned int j = 0;j < m_width; ++j)
		{ 
			if (bpp == 8)
			{
				BYTE gray = bytes[j];
				Color color = Color::Color32(gray, gray, gray);
				color.Gamma(gamma);
				SetPixel(j, i, color);
			}
			else if (bpp == 24)
			{
				BYTE red = bytes[j * 3 + 2];
				BYTE green = bytes[j * 3 + 1];
				BYTE blue = bytes[j * 3];
				Color color = Color::Color32(red, green, blue);
				color.Gamma(gamma);
				SetPixel(j, i, color);
			}
			else if (bpp == 32)
			{
				BYTE red = bytes[j * 4 + 2];
				BYTE green = bytes[j * 4 + 1];
				BYTE blue = bytes[j * 4];
				BYTE alpha = bytes[j * 4 + 3];
				Color color = Color::Color32(red, green, blue, alpha);
				color.Gamma(gamma);
				SetPixel(j, i, color);
			}
			else if (bpp == 96)
			{
				tagFIRGBF* data = (tagFIRGBF*)bytes;
				float red = data[j].red;
				float green = data[j].green;
				float blue = data[j].blue;
				Color color = Color(red, green, blue);
				SetPixel(j, i, color);
			}
		}
	}

	ApplyData();
}

Texture::~Texture()
{
	if (m_textureBuffer)
		delete m_textureBuffer;
	m_textureBuffer = nullptr;

	if (m_colors)
		delete[] m_colors;
	m_colors = 0;
}

unsigned int Texture::GetWidth() const
{
	return m_width;
}

unsigned int Texture::GetHeight() const
{
	return m_height;
}

void Texture::Fill(const Color& color)
{
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
}

void Texture::SetPixel(int x, int y, const Color& color)
{
	if (x >= 0 && x < m_width && y >= 0 && y < m_height)
	{
		m_colors[(y * m_width + x) * 4] = color.r;
		m_colors[(y * m_width + x) * 4 + 1] = color.g;
		m_colors[(y * m_width + x) * 4 + 2] = color.b;
		m_colors[(y * m_width + x) * 4 + 3] = color.a;
	}
}

void Texture::GetPixel(int x, int y, Color& out)
{
	if (m_wrapMode == WrapMode::Clamp)
	{
		if (x < 0)
			x = 0;
		else if (x >= m_width)
			x = (int)m_width - 1;
		if (y < 0)
			y = 0;
		else if (y >= m_height)
			y = (int)m_height - 1;
	}
	else if (m_wrapMode == WrapMode::Repeat)
	{
		int w = (int)m_width;
		int h = (int)m_height;
		if (x < 0)
			x = w - 1 + x % w;
		else if (x >= w)
			x = x % w;
		if (y < 0)
			y = h - 1 + y % h;
		else if (y >= h)
			y = y % h;
	}
	out.r = m_colors[(y * m_width + x) * 4];
	out.g = m_colors[(y * m_width + x) * 4 + 1];
	out.b = m_colors[(y * m_width + x) * 4 + 2];
	out.a = m_colors[(y * m_width + x) * 4 + 3];
}

void Texture::Sample(float u, float v, Color& out)
{
	if (m_filterMode == FilterMode::Bilinear)
	{
		int cellx = (int)floor(u / m_uDelta);
		int celly = (int)floor(v / m_vDelta);

		float cx = (u - m_uDelta * cellx) / m_uDelta;
		float cy = (v - m_vDelta * celly) / m_vDelta;

		Color topleft, topright, bottomleft, bottomright;

		GetPixel(cellx, celly, topleft);
		GetPixel(cellx + 1, celly, topright);
		GetPixel(cellx, celly + 1, bottomleft);
		GetPixel(cellx + 1, celly + 1, bottomright);

		out = Color::Lerp(Color::Lerp(topleft, topright, cx), Color::Lerp(bottomleft, bottomright, cx), cy);
	}
	else if (m_filterMode == FilterMode::Point)
	{
		int x = (int)floor(u * m_width);
		int y = (int)floor(v * m_height);
		GetPixel(x, y, out);
	}
}

void Texture::ApplyData()
{
	if (m_textureBuffer)
	{
		m_textureBuffer->Set2DBuffer(m_width, m_height, m_colors);
	}
}

void Texture::SetFilterMode(FilterMode filterMode)
{
	m_filterMode = filterMode;
	m_textureBuffer->SetFilterMode(filterMode);
}

FilterMode Texture::GetFilterMode()
{
	return m_filterMode;
}

void Texture::SetWrapMode(WrapMode wrapMode)
{
	m_wrapMode = wrapMode;
	m_textureBuffer->SetWrapMode(wrapMode);
}

WrapMode Texture::GetWrapMode()
{
	return m_wrapMode;
}

void Texture::SetShaderResource(unsigned int slot)
{
	if (m_textureBuffer)
	{
		m_textureBuffer->SetShaderResource(slot);
	}
}
