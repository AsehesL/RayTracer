#pragma once
#include "../Core/Color.h"
#include "TextureBase.h"

class Texture : public TextureBase
{
public:
	Texture(unsigned int width, unsigned int height, class GLContext* glContext);
	Texture(const char* path, bool isLinear, class GLContext* glContext);
	virtual ~Texture() override;
	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
	void Fill(const Color& color);
	void SetPixel(int x, int y, const Color& color);
	void GetPixel(int x, int y, Color& out);
	void Sample(float u, float v, Color& out);
	void ApplyData();

private:
	unsigned int m_width, m_height;
	float m_uDelta, m_vDelta;
	float* m_colors;
};