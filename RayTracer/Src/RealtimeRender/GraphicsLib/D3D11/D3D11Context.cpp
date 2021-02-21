#include "D3D11Context.h"
#include "D3D11RenderStateMachine.h"
#include "D3D11VertexBuffer.h"
#include "D3D11HLSLProgram.h"
#include "D3D11RenderBuffer.h"
#include "D3D11TextureBuffer.h"
#include "D3D11UniformBuffer.h"
#include "D3D11ComputeBuffer.h"

D3D11Context::D3D11Context()
{
	m_device = 0;
	m_deviceContext = 0;
	m_swapChain = 0;
	m_depthBuffer = 0;
	m_colorBuffer = 0;
	m_stateMachine = nullptr;
	//m_currentColorBuffer = 0;
	//m_currentDepthBuffer = 0;
	m_currentColorRenderTargetView = 0;
	m_currentDepthStencilView = 0;
}

D3D11Context::~D3D11Context()
{
	if (m_stateMachine != nullptr)
		delete m_stateMachine;
	m_stateMachine = nullptr;

	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}

	delete m_depthBuffer;
	delete m_colorBuffer;

	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = 0;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = 0;
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = 0;
	}
}

void D3D11Context::Present()
{
	if (m_swapChain != NULL) {
		m_swapChain->Present(0, 0);
	}
}

void D3D11Context::ClearColor(float r, float g, float b, float a)
{
	m_clearColor[0] = r;
	m_clearColor[1] = g;
	m_clearColor[2] = b;
	m_clearColor[3] = a;
}

void D3D11Context::Clear(int clearFlags)
{
	if ((clearFlags & (int)ClearFlags::Color) != 0)
		m_deviceContext->ClearRenderTargetView(m_currentColorRenderTargetView, m_clearColor);

	UINT clearmask = 0;
	if ((clearFlags & (int)ClearFlags::Depth) != 0)
		clearmask |= D3D11_CLEAR_DEPTH;
	if ((clearFlags & (int)ClearFlags::Stencil) != 0)
		clearmask |= D3D11_CLEAR_STENCIL;
	if (clearmask != 0)
		m_deviceContext->ClearDepthStencilView(m_currentDepthStencilView, clearmask, 1.0f, 0);
}

void D3D11Context::SetViewport(float x, float y, float width, float height)
{
	m_viewPort.Width = width;
	m_viewPort.Height = height;
	m_viewPort.TopLeftX = x;
	m_viewPort.TopLeftY = y;

	m_deviceContext->RSSetViewports(1, &m_viewPort);
}

VertexBuffer* D3D11Context::CreateVertexBuffer(bool isDynamic)
{
	return new D3D11VertexBuffer(m_device, m_deviceContext, isDynamic);
}

ShaderProgram* D3D11Context::CreateShaderProgram()
{
	return new D3D11HLSLProgram(m_device, m_deviceContext);
}

ComputeShaderProgram* D3D11Context::CreateComputeShaderProgram()
{
	return new D3D11ComputeShaderProgram(m_device, m_deviceContext);
}

UniformBuffer* D3D11Context::CreateUniformBuffer(unsigned int bufferSize)
{
	return new D3D11UniformBuffer(m_device, m_deviceContext, bufferSize);
}

ComputeBuffer* D3D11Context::CreateComputeBuffer(unsigned int elementSize, unsigned int count, void* data)
{
	return new D3D11ComputeBuffer(m_device, m_deviceContext, elementSize, count, data);
}

TextureBuffer* D3D11Context::CreateTextureBuffer()
{
	return new D3D11TextureBuffer(m_device, m_deviceContext, m_stateMachine);
}

SamplerState* D3D11Context::GetSamplerState(FilterMode filterMode, WrapMode wrapMode)
{
	if (m_stateMachine)
		return m_stateMachine->GetSamplerState(filterMode, wrapMode);
	return nullptr;
}

void D3D11Context::CreateRenderBuffer(unsigned int width, unsigned int height, bool isShadowMap, ColorBuffer** outColorBuffer, DepthBuffer** outDepthBuffer, TextureBuffer** outTextureBuffer)
{
	D3D11ColorBuffer* colorBuffer = new D3D11ColorBuffer(m_device, width, height, isShadowMap);
	D3D11DepthBuffer* depthBuffer = new D3D11DepthBuffer(m_device, width, height);

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = colorBuffer->m_format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	ID3D11ShaderResourceView* texture;

	HRESULT result = m_device->CreateShaderResourceView(colorBuffer->m_renderTexture, &shaderResourceViewDesc, &texture);
	if (FAILED(result)) 
	{
		delete colorBuffer;
		delete depthBuffer;
		return;
	}

	TextureBuffer* textureBuffer = new D3D11TextureBuffer(m_device, m_deviceContext, texture, m_stateMachine);

	*outColorBuffer = colorBuffer;
	*outDepthBuffer = depthBuffer;
	*outTextureBuffer = textureBuffer;
}

void D3D11Context::CreateCubeRenderBuffer(unsigned int size, CubeMapColorBuffer** outCubeMapColorBuffer, DepthBuffer** outDepthBuffer, TextureBuffer** outTextureBuffer)
{
	D3D11CubeMapBuffer* cubeMapBuffer = new D3D11CubeMapBuffer(m_device, size);
	D3D11DepthBuffer* depthBuffer = new D3D11DepthBuffer(m_device, size, size);

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	shaderResourceViewDesc.Format = cubeMapBuffer->m_format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	shaderResourceViewDesc.TextureCube.MostDetailedMip = 0;
	shaderResourceViewDesc.TextureCube.MipLevels = -1;

	ID3D11ShaderResourceView* texture;

	HRESULT result = m_device->CreateShaderResourceView(cubeMapBuffer->m_renderTexture, &shaderResourceViewDesc, &texture);
	if (FAILED(result))
	{
		delete cubeMapBuffer;
		delete depthBuffer;
		return;
	}

	TextureBuffer* textureBuffer = new D3D11TextureBuffer(m_device, m_deviceContext, texture, m_stateMachine);

	*outCubeMapColorBuffer = cubeMapBuffer;
	*outDepthBuffer = depthBuffer;
	*outTextureBuffer = textureBuffer;
}

void D3D11Context::SetRenderBuffer(ColorBuffer* colorBuffer, DepthBuffer* depthBuffer)
{
	if (colorBuffer == nullptr)
	{
		m_currentColorRenderTargetView = ((D3D11ColorBuffer*)m_colorBuffer)->m_renderTargetView;
	}
	else
	{
		m_currentColorRenderTargetView = ((D3D11ColorBuffer*)colorBuffer)->m_renderTargetView;
	}
	if (depthBuffer == nullptr)
	{
		m_currentDepthStencilView = ((D3D11DepthBuffer*)m_depthBuffer)->m_depthStencilView;
	}
	else
	{
		m_currentDepthStencilView = ((D3D11DepthBuffer*)depthBuffer)->m_depthStencilView;
	}
	//if (colorBuffer == nullptr || depthBuffer == nullptr)
	//{
	//	m_currentColorRenderTargetView = ((D3D11ColorBuffer*)m_colorBuffer)->m_renderTargetView;
	//	m_currentDepthStencilView = ((D3D11DepthBuffer*)m_depthBuffer)->m_depthStencilView;
	//	//m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
	//}
	//else
	//{
	//	m_currentColorRenderTargetView = ((D3D11ColorBuffer*)colorBuffer)->m_renderTargetView;
	//	m_currentDepthStencilView = ((D3D11DepthBuffer*)depthBuffer)->m_depthStencilView;
	//}
	//D3D11ColorBuffer* cb = (D3D11ColorBuffer*)m_currentColorBuffer;
	//D3D11DepthBuffer* db = (D3D11DepthBuffer*)m_currentDepthBuffer;
	m_deviceContext->OMSetRenderTargets(1, &m_currentColorRenderTargetView, m_currentDepthStencilView);
}

void D3D11Context::SetCubeRenderBuffer(CubeMapColorBuffer* cubeMapColorBuffer, DepthBuffer* depthBuffer, int face)
{
	if (cubeMapColorBuffer == nullptr || depthBuffer == nullptr)
	{
		m_currentColorRenderTargetView = ((D3D11ColorBuffer*)m_colorBuffer)->m_renderTargetView;
		m_currentDepthStencilView = ((D3D11DepthBuffer*)m_depthBuffer)->m_depthStencilView;
	}
	else
	{
		//m_currentColorBuffer = colorBuffer;
		//m_currentDepthBuffer = depthBuffer;
		m_currentColorRenderTargetView = ((D3D11CubeMapBuffer*)cubeMapColorBuffer)->m_renderTargetView[face];
		m_currentDepthStencilView = ((D3D11DepthBuffer*)depthBuffer)->m_depthStencilView;
	}
	//D3D11ColorBuffer* cb = (D3D11ColorBuffer*)m_currentColorBuffer;
	//D3D11DepthBuffer* db = (D3D11DepthBuffer*)m_currentDepthBuffer;
	m_deviceContext->OMSetRenderTargets(1, &m_currentColorRenderTargetView, m_currentDepthStencilView);
}

void D3D11Context::GenerateMipmap(TextureBuffer* textureBuffer)
{
	if (!textureBuffer)
		return;
	ID3D11ShaderResourceView* shaderResourceView = ((D3D11TextureBuffer*)textureBuffer)->m_shaderResourceView;
	m_deviceContext->GenerateMips(shaderResourceView);
}

void D3D11Context::ApplyRenderStates(bool forceApply)
{
	m_stateMachine->SetRasterizerState(cullMode, fillMode, forceApply);
	m_stateMachine->SetBlendEnable(blendEnable, blendOp, blendSrc, blendDst, forceApply);
	m_stateMachine->SetDepthStencilState(zwrite, stencilEnable, stencilId, ztestFunc, stencilTestFunc, stencilPassOp, stencilFailOp, stencilZfailOp, stencilReadMask, stencilWriteMask, forceApply);
}

bool D3D11Context::InitGL(HWND& hwnd, unsigned int screenWidth, unsigned int screenHeight)
{
	IDXGIFactory* factory;
	HRESULT result;
	IDXGIAdapter* adapter;
	IDXGIOutput* output;

	UINT numModes = 0;
	DXGI_MODE_DESC* displayModes;
	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

	//D3D11_TEXTURE2D_DESC depthBufferDesc;
	//D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	int i, numerator, denominator;

	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		return false;
	}

	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		return false;
	}

	result = adapter->EnumOutputs(0, &output);
	if (FAILED(result))
	{
		return false;
	}

	result = output->GetDisplayModeList(format, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if (FAILED(result))
	{
		return false;
	}

	displayModes = new DXGI_MODE_DESC[numModes];

	result = output->GetDisplayModeList(format, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModes);
	if (FAILED(result))
	{
		return false;
	}

	for (i = 0; i < numModes; i++)
	{
		if (displayModes[i].Width == (unsigned int)screenWidth)
		{
			if (displayModes[i].Height == (unsigned int)screenHeight)
			{
				numerator = displayModes[i].RefreshRate.Numerator;
				denominator = displayModes[i].RefreshRate.Denominator;
			}
		}
	}


	delete[] displayModes;
	displayModes = 0;

	output->Release();
	output = 0;

	adapter->Release();
	adapter = 0;

	factory->Release();
	factory = 0;


	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));

	sd.BufferCount = 1;
	sd.BufferDesc.Width = screenWidth;
	sd.BufferDesc.Height = screenHeight;
	sd.BufferDesc.Format = format;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.OutputWindow = hwnd;

	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	sd.Flags = 0;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;


	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &sd, &m_swapChain, &m_device, NULL, &m_deviceContext);
	if (FAILED(result))
	{
		return false;
	}

	ID3D11Texture2D* backBuffer;
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	if (FAILED(result))
	{
		return false;
	}

	m_colorBuffer = new D3D11ColorBuffer(m_device, backBuffer);
	m_depthBuffer = new D3D11DepthBuffer(m_device, screenWidth, screenHeight);


	SetRenderBuffer(m_colorBuffer, m_depthBuffer);
	//m_currentColorBuffer = m_colorBuffer;
	//m_currentDepthBuffer = m_depthBuffer;


	m_viewPort.Width = screenWidth;
	m_viewPort.Height = screenHeight;
	m_viewPort.MaxDepth = 1.0f;
	m_viewPort.MinDepth = 0.0f;
	m_viewPort.TopLeftX = 0.0f;
	m_viewPort.TopLeftY = 0.0f;

	m_deviceContext->RSSetViewports(1, &m_viewPort);

	m_stateMachine = new D3D11RenderStateMachine(m_device, m_deviceContext);

	return true;
}
