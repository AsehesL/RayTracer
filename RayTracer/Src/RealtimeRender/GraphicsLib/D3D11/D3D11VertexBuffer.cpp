#include "D3D11VertexBuffer.h"

D3D11VertexBuffer::D3D11VertexBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, bool isDynamic)
{
	m_device = device;
	m_deviceContext = deviceContext;
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_indexCount = 0;
	m_created = false;
	m_isValid = false;
	m_isDynamic = isDynamic;
}

D3D11VertexBuffer::~D3D11VertexBuffer()
{
	if (m_vertexBuffer != NULL)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = NULL;
	}
	if (m_indexBuffer != NULL)
	{
		m_indexBuffer->Release();
		m_indexBuffer = NULL;
	}
	m_device = NULL;
	m_deviceContext = NULL;
}

void D3D11VertexBuffer::SetVertices(float* vertices, unsigned int* indices, int vertexcount, int indexcount)
{

	HRESULT result;

	if (m_created) 
	{
		if (!m_isDynamic)
			return;
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		float* dataPtr;
		unsigned i;
		m_deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		dataPtr = (float*)mappedResource.pData;
		memcpy(dataPtr, vertices, sizeof(float) * VERTEX_ATTRIBUTE_SIZE * vertexcount);

		m_deviceContext->Unmap(m_vertexBuffer, 0);

		D3D11_MAPPED_SUBRESOURCE imappedResource;
		unsigned int* idataPtr;
		m_deviceContext->Map(m_indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &imappedResource);
		idataPtr = (unsigned int*)imappedResource.pData;
		memcpy(idataPtr, indices, sizeof(unsigned int) * indexcount);

		m_deviceContext->Unmap(m_indexBuffer, 0);
	}
	else
	{

		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;

		vertexBufferDesc.Usage = m_isDynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;;
		vertexBufferDesc.ByteWidth = sizeof(float) * VERTEX_ATTRIBUTE_SIZE * vertexcount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = m_isDynamic ? D3D11_CPU_ACCESS_WRITE : 0;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		result = m_device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
		if (FAILED(result))
		{
			return;
		}

		indexBufferDesc.Usage = m_isDynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;;
		indexBufferDesc.ByteWidth = sizeof(unsigned int) * indexcount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = m_isDynamic ? D3D11_CPU_ACCESS_WRITE : 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		result = m_device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
		if (FAILED(result))
		{
			m_vertexBuffer->Release();
			return;
		}

		m_indexCount = indexcount;

		m_created = true;
		m_isValid = true;
	}
}

void D3D11VertexBuffer::Bind(Topology topology, int indexCount)
{
	if (!IsValid())
		return;
	unsigned int stride;
	unsigned int offset;


	stride = VERTEX_ATTRIBUTE_SIZE * sizeof(float);
	offset = 0;

	ID3D11Buffer* vbuffer = m_vertexBuffer;
	ID3D11Buffer* ibuffer = m_indexBuffer;

	m_deviceContext->IASetVertexBuffers(0, 1, &vbuffer, &stride, &offset);

	m_deviceContext->IASetIndexBuffer(ibuffer, DXGI_FORMAT_R32_UINT, 0);

	if (topology == Topology::LineList)
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	else if (topology == Topology::LineStrip)
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	else if (topology == Topology::PointList)
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	else if (topology == Topology::TriangleList)
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	else if (topology == Topology::TriangleStrip)
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	if (indexCount <= 0)
		m_deviceContext->DrawIndexed(m_indexCount, 0, 0);
	else 
		m_deviceContext->DrawIndexed(indexCount, 0, 0);
}

bool D3D11VertexBuffer::IsValid()
{
	return m_isValid;
}
