#pragma once
#include "../../Core/ConcurrentQueue.hpp"
#include "../../Core/Color.h"
#include "../../Texture/TextureBase.h"

class TextureBuffer;

namespace RayTracer
{
	class RTRenderTexture : public TextureBase
	{
	public:
		RTRenderTexture(TextureBuffer* textureBuffer);
		virtual ~RTRenderTexture() override;

		TextureBuffer* GetTextureBuffer() const;
	};

	class RenderTarget
	{
	public:
		RenderTarget(unsigned int width, unsigned int height);
		~RenderTarget();

		void SetPixel(int x, int y, const Color& color);
		void GetPixel(int x, int y, Color& color);
		void Fill(const Color& color);
		void ApplyData(RTRenderTexture* renderTexture);
		unsigned int GetWidth();
		unsigned int GetHeight();
		void Save(const char* path, bool isHDR);

	private:
		float* m_colors;
		mutable std::mutex m_lock;
		bool m_isDirty;
		unsigned int m_width, m_height;
	};
}