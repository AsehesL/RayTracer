#include "D3D11UniformBuffer.h"

D3D11UniformBuffer::D3D11UniformBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, unsigned int bufferSize)
{
	m_uniformBuffer = 0;

	m_device = device;
	m_deviceContext = deviceContext;

	m_isValid = false;

	m_isMapped = false;

	D3D11_BUFFER_DESC uniformBufferDesc;

	uniformBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	uniformBufferDesc.ByteWidth = bufferSize;
	uniformBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	uniformBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	uniformBufferDesc.MiscFlags = 0;
	uniformBufferDesc.StructureByteStride = 0;

	HRESULT result = device->CreateBuffer(&uniformBufferDesc, NULL, &m_uniformBuffer);
	if (FAILED(result))
	{
		return;
	}

	m_isValid = true;
}

D3D11UniformBuffer::~D3D11UniformBuffer()
{
	if (m_uniformBuffer)
		m_uniformBuffer->Release();
	m_uniformBuffer = nullptr;
}

bool D3D11UniformBuffer::IsValid()
{
	return m_isValid;
}

void* D3D11UniformBuffer::Map()
{
	if (!IsValid())
		return nullptr;
	if (m_isMapped)
		return nullptr;

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	HRESULT result = m_deviceContext->Map(m_uniformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return nullptr;
	}
	m_isMapped = true;

	return mappedResource.pData;
}

void D3D11UniformBuffer::Unmap()
{
	if (!IsValid())
		return;
	if (!m_isMapped)
		return;
	m_isMapped = false;

	m_deviceContext->Unmap(m_uniformBuffer, 0);
}

void D3D11UniformBuffer::VSSetUniformBuffer(int position)
{
	if (!IsValid())
		return;
	if (m_isMapped)
		return;
	m_deviceContext->VSSetConstantBuffers(position, 1, &m_uniformBuffer);
}

void D3D11UniformBuffer::PSSetUniformBuffer(int position)
{
	if (!IsValid())
		return;
	if (m_isMapped)
		return;
	m_deviceContext->PSSetConstantBuffers(position, 1, &m_uniformBuffer);
}

void D3D11UniformBuffer::CSSetUniformBuffer(int position)
{
	if (!IsValid())
		return;
	if (m_isMapped)
		return;
	m_deviceContext->CSSetConstantBuffers(position, 1, &m_uniformBuffer);
}