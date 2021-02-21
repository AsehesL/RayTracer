#pragma once

#include "../VertexBuffer.h"

#include <d3d11.h>

class D3D11VertexBuffer : public VertexBuffer
{
public:
	D3D11VertexBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, bool isDynamic);
	~D3D11VertexBuffer() override;
	void SetVertices(float* vertices, unsigned int* indices, int vertexcount, int indexcount) override;
	void Draw(Topology topology, int indexCount = 0) override;
	bool IsValid() override;

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	unsigned int m_indexCount;
	bool m_created;
	bool m_isDynamic;
	bool m_isValid;
};