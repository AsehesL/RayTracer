#include "D3D11RenderBuffer.h"

D3D11ColorBuffer::D3D11ColorBuffer(ID3D11Device* device, unsigned int width, unsigned int height, bool isShadowMap)
{
	m_renderTexture = NULL;
	m_renderTargetView = NULL;

	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;

	ZeroMemory(&textureDesc, sizeof(textureDesc));

	m_format = isShadowMap ? DXGI_FORMAT_R32_FLOAT : DXGI_FORMAT_R32G32B32A32_FLOAT;

	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = m_format;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	HRESULT result = device->CreateTexture2D(&textureDesc, NULL, &m_renderTexture);
	if (FAILED(result))
		return;

	ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	result = device->CreateRenderTargetView(m_renderTexture, &renderTargetViewDesc, &m_renderTargetView);
	if (FAILED(result))
	{
		return;
	}
}

D3D11ColorBuffer::D3D11ColorBuffer(ID3D11Device* device, ID3D11Texture2D* texture)
{
	m_renderTexture = texture;
	m_renderTargetView = NULL;

	HRESULT result = device->CreateRenderTargetView(m_renderTexture, NULL, &m_renderTargetView);
	if (FAILED(result))
	{
		return;
	}
}

D3D11ColorBuffer::~D3D11ColorBuffer()
{
	if (m_renderTexture)
		m_renderTexture->Release();
	m_renderTexture = NULL;
	if (m_renderTargetView)
		m_renderTargetView->Release();
	m_renderTargetView = NULL;
}

D3D11DepthBuffer::D3D11DepthBuffer(ID3D11Device* device, unsigned int width, unsigned int height)
{
	m_depthTexture = NULL;
	m_depthStencilView = NULL;

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	m_format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = m_format;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	HRESULT result = device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthTexture);
	if (FAILED(result))
		return;

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = m_format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = device->CreateDepthStencilView(m_depthTexture, &depthStencilViewDesc, &m_depthStencilView);
	if (FAILED(result))
		return;
}

D3D11DepthBuffer::~D3D11DepthBuffer()
{
	if (m_depthTexture)
		m_depthTexture->Release();
	m_depthTexture = NULL;
	if (m_depthStencilView)
		m_depthStencilView->Release();
	m_depthStencilView = NULL;
}

D3D11CubeMapBuffer::D3D11CubeMapBuffer(ID3D11Device* device, unsigned int size)
{
	m_renderTargetView[0] = nullptr;
	m_renderTargetView[1] = nullptr;
	m_renderTargetView[2] = nullptr;
	m_renderTargetView[3] = nullptr;
	m_renderTargetView[4] = nullptr;
	m_renderTargetView[5] = nullptr;

	m_renderTexture = nullptr;

	D3D11_TEXTURE2D_DESC textureDesc;

	m_format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = size;
	textureDesc.Height = size;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 6;
	textureDesc.Format = m_format;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE | D3D11_RESOURCE_MISC_GENERATE_MIPS;

	HRESULT result = device->CreateTexture2D(&textureDesc, NULL, &m_renderTexture);
	if (FAILED(result))
		return;

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	renderTargetViewDesc.Texture2DArray.MipSlice = 0;
	renderTargetViewDesc.Texture2DArray.ArraySize = 1;

	for (int i = 0; i < 6; i++)
	{
		renderTargetViewDesc.Texture2DArray.FirstArraySlice = i;
		result = device->CreateRenderTargetView(m_renderTexture, &renderTargetViewDesc, &m_renderTargetView[i]);
		if (FAILED(result))
		{
			return;
		}
	}
}

D3D11CubeMapBuffer::~D3D11CubeMapBuffer()
{
	if (m_renderTexture)
		m_renderTexture->Release();
	m_renderTexture = NULL;
	if (m_renderTargetView[0])
		m_renderTargetView[0]->Release();
	if (m_renderTargetView[1])
		m_renderTargetView[1]->Release();
	if (m_renderTargetView[2])
		m_renderTargetView[2]->Release();
	if (m_renderTargetView[3])
		m_renderTargetView[3]->Release();
	if (m_renderTargetView[4])
		m_renderTargetView[4]->Release();
	if (m_renderTargetView[5])
		m_renderTargetView[5]->Release();
}
