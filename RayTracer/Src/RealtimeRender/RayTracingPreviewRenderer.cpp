#include "RayTracingPreviewRenderer.h"
#include "../Common/GlobalResource.h"
#include "GraphicsLib/GLContext.h"
#include "../RayTracer/Render/RayTracerRenderTarget.h"
#include "GraphicsLib/TextureBuffer.h"
#include "../Shader/ShaderConstants.h"

RayTracingPreviewRenderer::RayTracingPreviewRenderer(GLContext* glContext)
{
	m_glContext = glContext;
	m_screenQuadShader = GlobalResource::GetLinearToGammaShader();
	m_screenQuadMesh = GlobalResource::GetScreenQuadMesh();
	auto textureBuffer = glContext->CreateTextureBuffer();
	m_renderTexture = new RayTracer::RTRenderTexture(textureBuffer);
}

RayTracingPreviewRenderer::~RayTracingPreviewRenderer()
{
	delete m_renderTexture;
}

void RayTracingPreviewRenderer::UpdateRenderTarget(ConcurrentQueue<RayTracer::RenderTile>& tiles, RayTracer::RenderTarget* renderTarget)
{
	if (!m_glContext)
		return;

	while (!tiles.IsEmpty())
	{
		m_glContext->ClearColor(0, 0, 0, 0);
		m_glContext->Clear((int)ClearFlags::Color | (int)ClearFlags::Depth);

		if (m_screenQuadMesh && m_screenQuadShader && renderTarget)
		{
			//Texture* texture = renderTarget->GetTexture();
			renderTarget->ApplyData(m_renderTexture);
			//m_screenQuadShader->texture = texture;
			m_screenQuadShader->SetTexture(SHADER_TEXTURE_TEXTURE, m_renderTexture);
			m_screenQuadShader->SetRenderSize(m_windowWidth, m_windowHeight, m_renderWidth, m_renderHeight);
			if (m_screenQuadShader->Execute())
			{
				m_screenQuadMesh->Commit();
			}
		}

		m_glContext->Present();

		Sleep(2000);
	}
}
