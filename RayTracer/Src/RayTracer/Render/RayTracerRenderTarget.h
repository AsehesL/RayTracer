#pragma once
#include "../../Core/ConcurrentQueue.hpp"
#include "../../Core/Color.h"

class ITextureBuffer;

namespace RayTracer
{
	class RenderTarget
	{
	public:
		RenderTarget(unsigned int width, unsigned int height);
		~RenderTarget();

		void SetPixel(int x, int y, const Color& color);
		void GetPixel(int x, int y, Color& color);
		void Fill(const Color& color);
		void ApplyData(ITextureBuffer* textureBuffer);
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