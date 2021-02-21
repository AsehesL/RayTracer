#include "D3D11TextureBuffer.h"
#include "D3D11RenderStateMachine.h"

D3D11SamplerState::D3D11SamplerState(ID3D11Device* device, ID3D11DeviceContext* deviceContext, FilterMode filterMode, WrapMode wrapMode)
{
	D3D11_TEXTURE_ADDRESS_MODE u = D3D11_TEXTURE_ADDRESS_WRAP;
	D3D11_TEXTURE_ADDRESS_MODE v = D3D11_TEXTURE_ADDRESS_WRAP;
	if (wrapMode == WrapMode::Clamp)
	{
		u = D3D11_TEXTURE_ADDRESS_CLAMP;
		v = D3D11_TEXTURE_ADDRESS_CLAMP;
	}

	D3D11_FILTER filter;
	if (filterMode == FilterMode::Point)
		filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	else if (filterMode == FilterMode::Bilinear)
		filter = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
	else
		filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = filter;
	samplerDesc.AddressU = u;
	samplerDesc.AddressV = v;
	samplerDesc.AddressW = u;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	device->CreateSamplerState(&samplerDesc, &m_samplerState);

	m_deviceContext = deviceContext;
}

D3D11SamplerState::~D3D11SamplerState()
{
	m_samplerState->Release();
}

void D3D11SamplerState::VSSetSampler(unsigned int slot)
{
	m_deviceContext->VSSetSamplers(slot, 1, &m_samplerState);
}

void D3D11SamplerState::PSSetSampler(unsigned int slot)
{
	m_deviceContext->PSSetSamplers(slot, 1, &m_samplerState);
}

void D3D11SamplerState::CSSetSampler(unsigned int slot)
{
	m_deviceContext->CSSetSamplers(slot, 1, &m_samplerState);
}


D3D11TextureBuffer::D3D11TextureBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, D3D11RenderStateMachine* stateMachine)
{
	m_deviceContext = deviceContext;
	m_device = device;
	m_shaderResourceView = nullptr;
	//m_texture = nullptr;
	m_stateMachine = stateMachine;
	m_filterMode = FilterMode::Point;
	m_wrapMode = WrapMode::Repeat;
	m_isWritable = true;
	//m_isCreated = false;
}

D3D11TextureBuffer::D3D11TextureBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* resourceView, D3D11RenderStateMachine* stateMachine)
{
	m_device = device;
	m_deviceContext = deviceContext;
	m_shaderResourceView = resourceView;
	m_stateMachine = stateMachine;
	m_filterMode = FilterMode::Point;
	m_wrapMode = WrapMode::Repeat;
	m_isWritable = false;
	//m_isCreated = true;
	//m_texture = nullptr;
}

D3D11TextureBuffer::~D3D11TextureBuffer()
{
	if (m_shaderResourceView != NULL)
		m_shaderResourceView->Release();
	//if (m_texture != NULL)
	//	m_texture->Release();
	m_shaderResourceView = NULL;
	m_deviceContext = NULL;
}

void D3D11TextureBuffer::Set2DBuffer(unsigned int width, unsigned int height, const void* data)
{
	if (!m_isWritable)
		return;

	if (m_shaderResourceView)
		m_shaderResourceView->Release();
	m_shaderResourceView = nullptr;

	//if (!m_isCreated)
	{ 
		//DXGI_FORMAT f = TransferFormat(format);
		//if (format = DTextureFormat_RGBAFloat)
	   //     f = DXGI_FORMAT_R16G16B16A16_FLOAT;
		//int size = GetFormatSize(format);

		DXGI_FORMAT f = DXGI_FORMAT_R32G32B32A32_FLOAT;
		int size = sizeof(float) * 4;

		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = f;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//0;

		D3D11_SUBRESOURCE_DATA subResource;
		subResource.pSysMem = data;
		subResource.SysMemPitch = desc.Width * size;
		subResource.SysMemSlicePitch = 0;
		ID3D11Texture2D* pTexture;
		HRESULT hr = m_device->CreateTexture2D(&desc, &subResource, &pTexture);
		if (FAILED(hr))
		{
			return;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = f;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		hr = m_device->CreateShaderResourceView(pTexture, &srvDesc, &m_shaderResourceView);
		if (FAILED(hr))
		{
			pTexture->Release();
			return;
		}
		pTexture->Release();
		//m_isCreated = true;
	}
	/*else
	{
		D3D11_MAPPED_SUBRESOURCE imappedResource;
		void* idataPtr;
		m_deviceContext->Map(m_texture, 0, D3D11_MAP_WRITE, 0, &imappedResource);
		idataPtr = imappedResource.pData;
		memcpy(idataPtr, data, sizeof(float) * 4 * width);

		m_deviceContext->Unmap(m_texture, 0);
	}*/
}

void D3D11TextureBuffer::SetCubeBuffer(TextureBuffer* right, TextureBuffer* left, TextureBuffer* top, TextureBuffer* bottom, TextureBuffer* front, TextureBuffer* back)
{
	ID3D11Resource* srcTex[6];

	((D3D11TextureBuffer*)right)->m_shaderResourceView->GetResource(&srcTex[1]);
	((D3D11TextureBuffer*)left)->m_shaderResourceView->GetResource(&srcTex[0]);
	((D3D11TextureBuffer*)top)->m_shaderResourceView->GetResource(&srcTex[2]);
	((D3D11TextureBuffer*)bottom)->m_shaderResourceView->GetResource(&srcTex[3]);
	((D3D11TextureBuffer*)front)->m_shaderResourceView->GetResource(&srcTex[4]);
	((D3D11TextureBuffer*)back)->m_shaderResourceView->GetResource(&srcTex[5]);


	D3D11_TEXTURE2D_DESC texElementDesc;
	((ID3D11Texture2D*)srcTex[0])->GetDesc(&texElementDesc);

	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width = texElementDesc.Width;
	texArrayDesc.Height = texElementDesc.Height;
	texArrayDesc.MipLevels = 0;//texElementDesc.MipLevels;
	texArrayDesc.ArraySize = 6;
	texArrayDesc.Format = texElementDesc.Format;
	texArrayDesc.SampleDesc.Count = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE | D3D11_RESOURCE_MISC_GENERATE_MIPS;

	ID3D11Texture2D* texArray = 0;
	if (FAILED(m_device->CreateTexture2D(&texArrayDesc, 0, &texArray)))
		return;

	D3D11_BOX sourceRegion;

	D3D11_SUBRESOURCE_DATA pData[6];
	for (UINT x = 0; x < 6; x++)
	{
		//for (UINT mipLevel = 0; mipLevel < 1; mipLevel++)
		{
			sourceRegion.left = 0;
			sourceRegion.right = (texArrayDesc.Width >> 0);
			sourceRegion.top = 0;
			sourceRegion.bottom = (texArrayDesc.Height >> 0);
			sourceRegion.front = 0;
			sourceRegion.back = 1;

			if (sourceRegion.bottom == 0 || sourceRegion.right == 0)
				break;
			m_deviceContext->CopySubresourceRegion(texArray, D3D11CalcSubresource(0, 0, 0), 1, 0, 0, srcTex[x], 0, &sourceRegion);
		}
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texArrayDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	viewDesc.TextureCube.MostDetailedMip = 0;
	viewDesc.Texture2D.MipLevels = -1;//texArrayDesc.MipLevels;
	viewDesc.TextureCube.MipLevels = -1;//texArrayDesc.MipLevels;

	if (FAILED(m_device->CreateShaderResourceView(texArray, &viewDesc, &m_shaderResourceView)))
		return;

	m_deviceContext->GenerateMips(m_shaderResourceView);
}

SamplerState* D3D11TextureBuffer::GetSampler()
{
	if (m_stateMachine)
		return m_stateMachine->GetSamplerState(m_filterMode, m_wrapMode);
	return nullptr;
}

void D3D11TextureBuffer::SetFilterMode(FilterMode filterMode)
{
	m_filterMode = filterMode;
}

FilterMode D3D11TextureBuffer::GetFilterMode()
{
	return m_filterMode;
}

void D3D11TextureBuffer::SetWrapMode(WrapMode wrapMode)
{
	m_wrapMode = wrapMode;
}

WrapMode D3D11TextureBuffer::GetWrapMode()
{
	return m_wrapMode;
}

void D3D11TextureBuffer::VSSetTexture(unsigned int slot)
{
	if (m_shaderResourceView)
	{
		auto sampler = GetSampler();
		if (sampler)
			sampler->VSSetSampler(slot);
		m_deviceContext->VSSetShaderResources(slot, 1, &m_shaderResourceView);
	}
}

void D3D11TextureBuffer::PSSetTexture(unsigned int slot)
{
	if (m_shaderResourceView)
	{
		auto sampler = GetSampler();
		if (sampler)
			sampler->PSSetSampler(slot);
		m_deviceContext->PSSetShaderResources(slot, 1, &m_shaderResourceView);
	}
}

void D3D11TextureBuffer::CSSetTexture(unsigned int slot)
{
	if (m_shaderResourceView)
	{
		auto sampler = GetSampler();
		if (sampler)
			sampler->CSSetSampler(slot);
		m_deviceContext->CSSetShaderResources(slot, 1, &m_shaderResourceView);
	}
}