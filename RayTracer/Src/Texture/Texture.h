#pragma once
#include "../Core/Color.h"
#include "../RealtimeRender/GraphicsLib/GLDefines.h"
#include "../RealtimeRender/GraphicsLib/ShaderResource.h"

class Texture : public IShaderResource
{
public:
	Texture(unsigned int width, unsigned int height, class GLContext* glContext);
	Texture(const char* path, bool isLinear, class GLContext* glContext);
	~Texture();
	unsigned int GetWidth() const;
	unsigned int GetHeight() const;
	void Fill(const Color& color);
	void SetPixel(int x, int y, const Color& color);
	void GetPixel(int x, int y, Color& out);
	void Sample(float u, float v, Color& out);
	void ApplyData();
	void SetFilterMode(FilterMode filterMode);
	FilterMode GetFilterMode();
	void SetWrapMode(WrapMode wrapMode);
	WrapMode GetWrapMode();
	virtual void SetShaderResource(unsigned int slot) override;

private:
	WrapMode m_wrapMode;
	FilterMode m_filterMode;

	unsigned int m_width, m_height;
	float m_uDelta, m_vDelta;
	float* m_colors;
	class ITextureBuffer* m_textureBuffer;
};