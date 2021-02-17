#include "RayTracerRenderer.h"
#include "Integrator/IntegratorBase.h"
#include "../Camera/Camera.h"
#include "../Scene/Scene.h"
#include "Render/RayTracerRenderTarget.h"
#include <thread>
#include <functional>

RayTracer::RayTracerRenderer::RayTracerRenderer(Scene* scene)
{
	m_scene = scene;
	m_renderTarget = nullptr;
	m_integrator = nullptr;
	//m_isRayTracing = false;
	m_rayTracerScene = nullptr;
	//m_currentTileIndex = 0;
}

//RayTracer::RayTracerRenderer::RayTracerRenderer(Scene* scene)
//{
//	//m_glContext = glContext;
//	m_scene = scene;
//	//m_screenQuadShader = GlobalResource::GetScreenQuadShader();
//	//m_screenQuadMesh = GlobalResource::GetScreenQuadMesh();
//	m_renderTarget = nullptr;
//	m_integrator = nullptr;
//	m_isRayTracing = false;
//	m_rayTracerScene = nullptr;
//	//m_currentTileIndex = 0;
//}

RayTracer::RayTracerRenderer::~RayTracerRenderer()
{
	if (m_renderTarget)
		delete m_renderTarget;
	m_renderTarget = nullptr;
	if (m_rayTracerScene)
		delete m_rayTracerScene;
	m_rayTracerScene = nullptr;
}

bool RayTracer::RayTracerRenderer::StartRayTracing(IntegratorBase* integrator)
{
	if (!integrator)
		return false;
	if (!m_scene)
		return false;

	Camera* camera = m_scene->GetCamera();
	if (!camera)
		return false;
	camera->ForceRefreshTransform();
	unsigned int width = camera->GetRenderWidth();
	unsigned int height = camera->GetRenderHeight();

	if (m_rayTracerScene)
		delete m_rayTracerScene;
	m_rayTracerScene = m_scene->BuildRayTracerScene();

	m_integrator = integrator;

	if (m_renderTarget != nullptr && (m_renderTarget->GetWidth() != width || m_renderTarget->GetHeight() != height))
	{
		delete m_renderTarget;
		m_renderTarget = nullptr;
	}
	if (m_renderTarget == nullptr)
		m_renderTarget = new RenderTarget(width, height);
	m_renderTarget->Fill(Color(0, 0, 0, 0));

	integrator->Render(m_rayTracerScene, m_renderTarget);


	/*std::function<void(RayTracerRenderer*, RenderTarget*)> renderTargetUpdateFunc = &RayTracerRenderer::UpdateRenderTarget;
	std::thread renderTargetUpdateThread(renderTargetUpdateFunc, this, m_renderTarget);

	renderTargetUpdateThread.join();*/


	//m_isRayTracing = true;
	return true;
}

//bool RayTracer::RayTracerRenderer::IsRayTracingFinish()
//{
//	return !m_isRayTracing;
//}
//
//void RayTracer::RayTracerRenderer::UpdateFrame(int windowWidth, int windowHeight, int renderWidth, int renderHeight)
//{
//	if (!m_glContext)
//		return;
//	m_glContext->ClearColor(0, 0, 0, 0);
//	m_glContext->Clear((int)ClearFlags::Color | (int)ClearFlags::Depth);
//
//	if (m_screenQuadMesh && m_screenQuadShader && m_renderTarget)
//	{
//		Texture* texture = m_renderTarget->GetTexture();
//		m_screenQuadShader->texture = texture;
//		m_screenQuadShader->SetRenderSize(windowWidth, windowHeight, renderWidth, renderHeight);
//		if (m_screenQuadShader->Execute())
//		{
//			m_screenQuadMesh->Commit();
//		}
//	}
//
//	m_glContext->Present();
//}

//void RayTracer::RayTracerRenderer::UpdateRenderTarget(RenderTarget* renderTarget)
//{
//	if (!m_glContext)
//		return;
//	m_glContext->ClearColor(0, 0, 0, 0);
//	m_glContext->Clear((int)ClearFlags::Color | (int)ClearFlags::Depth);
//
//	if (m_screenQuadMesh && m_screenQuadShader && m_renderTarget)
//	{
//		Texture* texture = m_renderTarget->GetTexture();
//		m_screenQuadShader->texture = texture;
//		if (m_screenQuadShader->Execute())
//		{
//			m_screenQuadMesh->Commit();
//		}
//	}
//
//	m_glContext->Present();
//
//	Sleep(1000);
//}
