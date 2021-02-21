#pragma once
#include "../ComputeBuffer.h"

#include <d3d11.h>

class D3D11ComputeBuffer : public ComputeBuffer
{
public:
	D3D11ComputeBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, unsigned int elementSize, unsigned int count, void* data);
	virtual ~D3D11ComputeBuffer() override;

	bool IsValid() override;
	void* Map() override;
	void Unmap() override;
	void CSSetUAV(unsigned int slot) override;

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11Buffer* m_computeBuffer;
	ID3D11Buffer* m_tempBuffer;
	ID3D11UnorderedAccessView* m_uav;

	bool m_isMapped;
	bool m_isValid;
};