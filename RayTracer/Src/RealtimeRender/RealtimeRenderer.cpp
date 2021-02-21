#include "RealtimeRenderer.h"
#include "GraphicsLib/GLContext.h"
#include "../Scene/Scene.h"
#include "../Texture/RenderTexture.h"
#include "GraphicsLib/RenderBuffer.h"
#include "../Common/GlobalResource.h"
#include "../Camera/Camera.h"
#include "../Light/SunLight.h"
#include "../Gizmos/GizmosRenderer.h"
#include "../Shader/ShaderConstants.h"

RealtimeRenderer::RealtimeRenderer(GLContext* glContext, Scene* scene)
{
	m_glContext = glContext;
	m_scene = scene;
	m_linearToGammaShader = GlobalResource::GetLinearToGammaShader();
	m_screenQuadShader = GlobalResource::GetScreenQuadShader();
	m_screenQuadMesh = GlobalResource::GetScreenQuadMesh();
	m_renderTexture = nullptr;
	m_opaqueSceneTexture = nullptr;
	m_gizmosRenderer = new GizmosRenderer(glContext);
}

RealtimeRenderer::~RealtimeRenderer()
{
	m_glContext = nullptr;
	m_scene = nullptr;
	delete m_gizmosRenderer;
	if (m_renderTexture)
		delete m_renderTexture;
	m_renderTexture = nullptr;
	if (m_opaqueSceneTexture)
		delete m_opaqueSceneTexture;
	m_opaqueSceneTexture = nullptr;
}

void RealtimeRenderer::RenderFrame(int windowWidth, int windowHeight, int renderWidth, int renderHeight)
{
	if (!m_glContext || !m_scene)
		return;

	if (m_renderTexture != nullptr && (m_renderTexture->GetWidth() != renderWidth || m_renderTexture->GetHeight() != renderHeight))
	{
		delete m_renderTexture;
		m_renderTexture = nullptr;
	}
	if (m_renderTexture == nullptr)
		m_renderTexture = new RenderTexture(renderWidth, renderHeight, m_glContext);

	RenderShadowPass();

	RenderColorPass(renderWidth, renderHeight);

	RenderGizmos();

	RenderPostProcess(windowWidth, windowHeight, renderWidth, renderHeight);

	m_glContext->Present();
}

RenderTexture* RealtimeRenderer::GetOpaqueSceneScreenCapture()
{
	if (!m_screenQuadMesh || !m_renderTexture || !m_screenQuadShader)
		return nullptr;
	if (m_opaqueSceneTexture != nullptr && (m_opaqueSceneTexture->GetWidth() != m_renderTexture->GetWidth() || m_opaqueSceneTexture->GetHeight() != m_renderTexture->GetHeight()))
	{
		delete m_opaqueSceneTexture;
		m_opaqueSceneTexture = nullptr;
	}
	if (m_opaqueSceneTexture == nullptr)
		m_opaqueSceneTexture = new RenderTexture(m_renderTexture->GetWidth(), m_renderTexture->GetHeight(), m_glContext);

	m_glContext->SetRenderBuffer(m_opaqueSceneTexture->GetColorBuffer(), m_opaqueSceneTexture->GetDepthBuffer());

	m_glContext->SetViewport(0, 0, m_opaqueSceneTexture->GetWidth(), m_opaqueSceneTexture->GetHeight());

	m_glContext->ClearColor(0, 0, 0, 0);
	m_glContext->Clear((int)ClearFlags::Color | (int)ClearFlags::Depth);

	m_screenQuadShader->SetRenderSize(m_opaqueSceneTexture->GetWidth(), m_opaqueSceneTexture->GetHeight(), m_opaqueSceneTexture->GetWidth(), m_opaqueSceneTexture->GetHeight());
	m_screenQuadShader->SetTexture(SHADER_TEXTURE_TEXTURE, m_renderTexture);
	if (m_screenQuadShader->Execute())
	{
		m_screenQuadMesh->Commit();
	}

	return m_opaqueSceneTexture;
}

void RealtimeRenderer::RenderColorPass(int renderWidth, int renderHeight)
{
	int opaqueCount = 0;
	int transparencyCount = 0;
	m_scene->PrepareForColorPass(opaqueCount, transparencyCount);
	
	m_glContext->SetRenderBuffer(m_renderTexture->GetColorBuffer(), m_renderTexture->GetDepthBuffer());

	m_glContext->SetViewport(0, 0, renderWidth, renderHeight);

	Color defaultColor = COLOR_DEFAULT_SKY;
	m_glContext->ClearColor(defaultColor.r, defaultColor.g, defaultColor.b, defaultColor.a);
	m_glContext->Clear((int)ClearFlags::Color | (int)ClearFlags::Depth | (int)ClearFlags::Stencil);

	m_scene->RenderOpaqueScene();

	if (transparencyCount > 0)
	{
		RenderTexture* opaqueSceneCapture = GetOpaqueSceneScreenCapture();

		m_glContext->SetRenderBuffer(m_renderTexture->GetColorBuffer(), m_renderTexture->GetDepthBuffer());

		m_glContext->SetViewport(0, 0, renderWidth, renderHeight);

		m_scene->RenderTransparencyScene(opaqueSceneCapture);
	}
}

void RealtimeRenderer::RenderShadowPass()
{
	if (m_scene)
	{
		Camera* camera = m_scene->GetCamera();
		SunLight* sunlight = m_scene->GetSunLight();
		if (camera && sunlight)
		{
			RenderTexture* shadowMap = sunlight->GetShadowMap();
			if (shadowMap)
			{
				m_glContext->SetRenderBuffer(shadowMap->GetColorBuffer(), shadowMap->GetDepthBuffer());

				m_glContext->SetViewport(0, 0, shadowMap->GetWidth(), shadowMap->GetHeight());

				m_glContext->ClearColor(1, 1, 1, 1);
				m_glContext->Clear((int)ClearFlags::Color | (int)ClearFlags::Depth);

				m_scene->RenderShadowScene();
			}
		}
	}
}

void RealtimeRenderer::RenderPostProcess(int windowWidth, int windowHeight, int renderWidth, int renderHeight)
{
	m_glContext->SetRenderBuffer(nullptr, nullptr);

	m_glContext->SetViewport(0, 0, windowWidth, windowHeight);

	m_glContext->ClearColor(0, 0, 0, 0);
	m_glContext->Clear((int)ClearFlags::Color | (int)ClearFlags::Depth);

	if (m_screenQuadMesh && m_linearToGammaShader && m_renderTexture)
	{
		m_linearToGammaShader->SetRenderSize(windowWidth, windowHeight, renderWidth, renderHeight);
		m_linearToGammaShader->SetTexture(SHADER_TEXTURE_TEXTURE, m_renderTexture);
		if (m_linearToGammaShader->Execute())
		{
			m_screenQuadMesh->Commit();
		}
	}
}

void RealtimeRenderer::RenderGizmos()
{
	if (m_scene && m_gizmosRenderer)
	{
		m_gizmosRenderer->Clear();
		m_scene->RenderGizmos(m_gizmosRenderer);
		m_gizmosRenderer->Render();
	}
}
