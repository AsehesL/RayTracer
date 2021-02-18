#pragma once
#include "../Core/ConcurrentQueue.hpp"
#include "../RayTracer/Render/RayTracerRenderTile.h"

namespace RayTracer
{
	class RenderTarget;
}

class RayTracingPreviewRenderer
{
public:
	RayTracingPreviewRenderer(class GLContext* glContext);
	~RayTracingPreviewRenderer();

	void SetRenderSizeForPreview(int windowWidth, int windowHeight, int renderWidth, int renderHeight)
	{
		m_windowWidth = windowWidth;
		m_windowHeight = windowHeight;
		m_renderWidth = renderWidth;
		m_renderHeight = renderHeight;
	}

	void UpdateRenderTarget(ConcurrentQueue<RayTracer::RenderTile>& tiles, RayTracer::RenderTarget* renderTarget);

private:
	int m_windowWidth, m_windowHeight, m_renderWidth, m_renderHeight;

	class GLContext* m_glContext;
	class Mesh* m_screenQuadMesh;
	class LinearToGammaShader* m_screenQuadShader;
	class ITextureBuffer* m_textureBuffer;
};