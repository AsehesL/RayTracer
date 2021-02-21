#pragma once

#include "../UniformBuffer.h"

#include <d3d11.h>

class D3D11UniformBuffer : public UniformBuffer
{
public:
	D3D11UniformBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, unsigned int bufferSize);
	~D3D11UniformBuffer() override;

	bool IsValid() override;
	void* Map() override;
	void Unmap() override;
	void VSSetUniformBuffer(int slot) override;
	void PSSetUniformBuffer(int slot) override;
	void CSSetUniformBuffer(int slot) override;

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11Buffer* m_uniformBuffer;
	bool m_isValid;
	bool m_isMapped;
};