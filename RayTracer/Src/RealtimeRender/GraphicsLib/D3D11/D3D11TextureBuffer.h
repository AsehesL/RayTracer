#pragma once
#include "../TextureBuffer.h"
#include <d3d11.h>

class D3D11SamplerState : public SamplerState
{
public:
	D3D11SamplerState(ID3D11Device* device, ID3D11DeviceContext* deviceContext, FilterMode filterMode, WrapMode wrapMode);
	~D3D11SamplerState() override;
	void VSSetSampler(unsigned int slot) override;
	void PSSetSampler(unsigned int slot) override;
	void CSSetSampler(unsigned int slot) override;

private:
	ID3D11SamplerState* m_samplerState;
	ID3D11DeviceContext* m_deviceContext;
};

class D3D11TextureBuffer : public TextureBuffer 
{
	friend class D3D11Context;
public:
	D3D11TextureBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, class D3D11RenderStateMachine* stateMachine);
	D3D11TextureBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* resourceView, class D3D11RenderStateMachine* stateMachine);
	~D3D11TextureBuffer() override;
	void Set2DBuffer(unsigned int width, unsigned int height, const void* data) override;
	void SetCubeBuffer(TextureBuffer* left, TextureBuffer* right, TextureBuffer* top, TextureBuffer* bottom, TextureBuffer* front, TextureBuffer* back) override;
	SamplerState* GetSampler() override;
	void SetFilterMode(FilterMode filterMode) override;
	FilterMode GetFilterMode() override;
	void SetWrapMode(WrapMode wrapMode) override;
	WrapMode GetWrapMode() override;
	void VSSetTexture(unsigned int slot) override;
	void PSSetTexture(unsigned int slot) override;
	void CSSetTexture(unsigned int slot) override;

private:
	ID3D11ShaderResourceView* m_shaderResourceView;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11Device* m_device;
	FilterMode m_filterMode;
	WrapMode m_wrapMode;
	class D3D11RenderStateMachine* m_stateMachine;
	bool m_isWritable;
};