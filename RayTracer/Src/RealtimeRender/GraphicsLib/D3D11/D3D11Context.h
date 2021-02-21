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
	class VertexBuffer* CreateVertexBuffer(bool isDynamic) override;

	class ShaderProgram* CreateShaderProgram() override;

	class ComputeShaderProgram* CreateComputeShaderProgram() override;

	class UniformBuffer* CreateUniformBuffer(unsigned int bufferSize) override;

	class ComputeBuffer* CreateComputeBuffer(unsigned int elementSize, unsigned int count, void* data) override;

	class TextureBuffer* CreateTextureBuffer() override;

	class SamplerState* GetSamplerState(FilterMode filterMode, WrapMode wrapMode) override;

	void CreateRenderBuffer(unsigned int width, unsigned int height, bool isShadowMap, class ColorBuffer** outColorBuffer, class DepthBuffer** outDepthBuffer, class TextureBuffer** outTextureBuffer) override;

	void CreateCubeRenderBuffer(unsigned int size, class CubeMapColorBuffer** outCubeMapColorBuffer, class DepthBuffer** outDepthBuffer, class TextureBuffer** outTextureBuffer) override;

	void SetRenderBuffer(class ColorBuffer* colorBuffer, class DepthBuffer* depthBuffer) override;

	void SetCubeRenderBuffer(class CubeMapColorBuffer* cubeMapColorBuffer, class DepthBuffer* depthBuffer, int face) override;

	void GenerateMipmap(class TextureBuffer* textureBuffer) override;

	ID3D11DeviceContext* GetDeviceContext() const { return m_deviceContext; }
	ID3D11Device* GetDevice() const { return m_device; }

protected:
	void ApplyRenderStates(bool forceApply) override;
	bool InitGL(HWND& hwnd, unsigned int screenWidth, unsigned int screenHeight) override;

private:
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	D3D11_VIEWPORT m_viewPort;
	float m_clearColor[4];
	class ColorBuffer* m_colorBuffer;
	class DepthBuffer* m_depthBuffer;
	//class IColorBuffer* m_currentColorBuffer;
	//class IDepthBuffer* m_currentDepthBuffer;
	ID3D11RenderTargetView* m_currentColorRenderTargetView;
	ID3D11DepthStencilView* m_currentDepthStencilView;
	class D3D11RenderStateMachine* m_stateMachine;
};

