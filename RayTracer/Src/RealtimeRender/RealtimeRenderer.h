#pragma once

class RealtimeRenderer
{
public:
	RealtimeRenderer(class GLContext* glContext, class Scene* scene);
	~RealtimeRenderer();

	void RenderFrame(int windowWidth, int windowHeight, int renderWidth, int renderHeight);

private:
	class RenderTexture* GetOpaqueSceneScreenCapture();
	class RenderTexture* DownSample(class RenderTexture* renderTexture, unsigned int width, unsigned int height);
	class RenderTexture* UpSample(class RenderTexture* renderTexture, unsigned int width, unsigned int height);

	void RenderColorPass(int renderWidth, int renderHeight);
	void RenderShadowPass();
	void RenderPostProcess(int windowWidth, int windowHeight, int renderWidth, int renderHeight);
	void RenderGizmos();

private:
	class GLContext* m_glContext;
	class Scene* m_scene;
	class RenderTexture* m_renderTexture;
	class RenderTexture* m_opaqueSceneTexture;
	class Mesh* m_screenQuadMesh;
	class LinearToGammaShader* m_linearToGammaShader;
	class ScreenQuadShader* m_screenQuadShader;
	class GizmosRenderer* m_gizmosRenderer;
};