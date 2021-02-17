#pragma once
#include "../RenderBuffer.h"
#include "../TextureBuffer.h"
#include <D3D11.h>

class D3D11ColorBuffer : public IColorBuffer
{
	friend class D3D11Context;
public:
	D3D11ColorBuffer(ID3D11Device* device, unsigned int width, unsigned int height, bool isShadowMap);
	D3D11ColorBuffer(ID3D11Device* device, ID3D11Texture2D* texture);
	~D3D11ColorBuffer() override;

private:
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_renderTexture;
	DXGI_FORMAT m_format;
};

class D3D11DepthBuffer : public IDepthBuffer
{
	friend class D3D11Context;
public:
	D3D11DepthBuffer(ID3D11Device* device, unsigned int width, unsigned int height);
	~D3D11DepthBuffer() override;

private:
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11Texture2D* m_depthTexture;
	DXGI_FORMAT m_format;
};

class D3D11CubeMapBuffer : public ICubeMapColorBuffer
{
	friend class D3D11Context;

public:
	D3D11CubeMapBuffer(ID3D11Device* device, unsigned int size);
	~D3D11CubeMapBuffer() override;

private:
	ID3D11RenderTargetView* m_renderTargetView[6];
	ID3D11Texture2D* m_renderTexture;
	DXGI_FORMAT m_format;
};
