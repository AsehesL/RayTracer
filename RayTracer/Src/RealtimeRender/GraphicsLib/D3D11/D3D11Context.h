#pragma once
#include "../GLContext.h"

#include <dxgi.h>
#include <d3d11.h>

class D3D11Context : public GLContext
{
public:
	D3D11Context();
	~D3D11Context() override;

	void Present() override;

	void ClearColor(float r, float g, float b, float a) override;
	void Clear(int clearFlags) override;
	void SetViewport(float x, float y, float width, float height) override;
	class IVertexBuffer* CreateVertexBuffer(bool isDynamic) override;
	class IShaderProgram* CreateShaderProgram() override;
	class IShaderUniformBuffer* CreateShaderUniformBuffer(unsigned int bufferSize) override;
	class ITextureBuffer* CreateTextureBuffer() override;
	void CreateRenderBuffer(unsigned int width, unsigned int height, bool isShadowMap, class IColorBuffer** outColorBuffer, class IDepthBuffer** outDepthBuffer, class ITextureBuffer** outTextureBuffer) override;
	void CreateCubeRenderBuffer(unsigned int size, class ICubeMapColorBuffer** outCubeMapColorBuffer, class IDepthBuffer** outDepthBuffer, class ITextureBuffer** outTextureBuffer) override;
	void SetRenderBuffer(class IColorBuffer* colorBuffer, class IDepthBuffer* depthBuffer) override;
	void SetCubeRenderBuffer(class ICubeMapColorBuffer* cubeMapColorBuffer, class IDepthBuffer* depthBuffer, int face) override;
	void GenerateMipmap(class ITextureBuffer* textureBuffer) override;

protected:
	void ApplyRenderStates(bool forceApply) override;
	bool InitGL(HWND& hwnd, unsigned int screenWidth, unsigned int screenHeight) override;

private:
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	D3D11_VIEWPORT m_viewPort;
	float m_clearColor[4];
	class IColorBuffer* m_colorBuffer;
	class IDepthBuffer* m_depthBuffer;
	//class IColorBuffer* m_currentColorBuffer;
	//class IDepthBuffer* m_currentDepthBuffer;
	ID3D11RenderTargetView* m_currentColorRenderTargetView;
	ID3D11DepthStencilView* m_currentDepthStencilView;
	class D3D11RenderStateMachine* m_stateMachine;
};

