#include "D3D11ComputeBuffer.h"

D3D11ComputeBuffer::D3D11ComputeBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, unsigned int elementSize, unsigned int count, void* data)
{
	m_computeBuffer = 0;
	m_tempBuffer = 0;
	m_uav = 0;

	m_isValid = false;

	m_isMapped = false;

	m_device = device;
	m_deviceContext = deviceContext;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.ByteWidth = elementSize * count;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = elementSize;

	HRESULT result;

	if (data)
	{
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = data;
		result = m_device->CreateBuffer(&desc, &InitData, &m_computeBuffer);
	}
	else
		result = m_device->CreateBuffer(&desc, NULL, &m_computeBuffer);

	if (FAILED(result))
		return;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	m_computeBuffer->GetDesc(&bufferDesc);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(uavDesc));
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.FirstElement = 0;

	if (bufferDesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
	{
		uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
		uavDesc.Buffer.NumElements = bufferDesc.ByteWidth / 4;
	}
	else
	{
		if (bufferDesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
		{
			uavDesc.Format = DXGI_FORMAT_UNKNOWN;
			uavDesc.Buffer.NumElements = bufferDesc.ByteWidth / bufferDesc.StructureByteStride;
		}
		else
		{
			m_isValid = false;
			return;
		}
	}

	result = m_device->CreateUnorderedAccessView(m_computeBuffer, &uavDesc, &m_uav);
	if (FAILED(result))
	{
		return;
	}

	m_isValid = true;
}

D3D11ComputeBuffer::~D3D11ComputeBuffer()
{
	if (m_computeBuffer)
		m_computeBuffer->Release();
	m_computeBuffer = nullptr;
	if (m_uav)
		m_uav->Release();
	m_uav = nullptr;
	if (m_tempBuffer)
		m_tempBuffer->Release();
	m_tempBuffer = nullptr;
}

bool D3D11ComputeBuffer::IsValid()
{
	return m_isValid;
}

void* D3D11ComputeBuffer::Map()
{
	if (!IsValid())
		return nullptr;
	if (m_isMapped)
		return nullptr;

	if (m_tempBuffer)
		m_tempBuffer->Release();
	m_tempBuffer = nullptr;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	m_computeBuffer->GetDesc(&desc);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;
	HRESULT hr = m_device->CreateBuffer(&desc, NULL, &m_tempBuffer);
	if (FAILED(hr))
	{
		if (m_tempBuffer)
			m_tempBuffer->Release();
		m_tempBuffer = nullptr;
		return nullptr;
	}
	m_deviceContext->CopyResource(m_tempBuffer, m_computeBuffer);

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	HRESULT result = m_deviceContext->Map(m_tempBuffer, 0, D3D11_MAP_READ, 0, &mappedResource);
	if (FAILED(result))
	{
		return nullptr;
	}
	m_isMapped = true;

	return mappedResource.pData;
}

void D3D11ComputeBuffer::Unmap()
{
	if (!IsValid())
		return;
	if (!m_isMapped)
		return;

	m_isMapped = false;

	m_deviceContext->Unmap(m_tempBuffer, 0);

	if (m_tempBuffer)
		m_tempBuffer->Release();
	m_tempBuffer = nullptr;
}

void D3D11ComputeBuffer::CSSetUAV(unsigned int slot)
{
	if (!IsValid())
		return;
	if (m_isMapped)
		return;
	if (m_uav)
		m_deviceContext->CSSetUnorderedAccessViews(slot, 1, &m_uav, NULL);
}
