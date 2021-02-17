#pragma once
#include "../TextureBuffer.h"
#include <d3d11.h>

class D3D11RenderStateMachine;

class D3D11TextureBuffer : public ITextureBuffer, public IShaderResource
{
	friend class D3D11Context;
public:
	D3D11TextureBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, D3D11RenderStateMachine* stateMachine);
	D3D11TextureBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11ShaderResourceView* resourceView, D3D11RenderStateMachine* stateMachine);
	~D3D11TextureBuffer() override;
	void Set2DBuffer(unsigned int width, unsigned int height, const void* data) override;
	void SetCubeBuffer(ITextureBuffer* left, ITextureBuffer* right, ITextureBuffer* top, ITextureBuffer* bottom, ITextureBuffer* front, ITextureBuffer* back) override;
	void SetFilterMode(FilterMode filterMode) override;
	FilterMode GetFilterMode() override;
	void SetWrapMode(WrapMode wrapMode) override;
	WrapMode GetWrapMode() override;
	virtual void SetShaderResource(unsigned int slot) override;

private:
	ID3D11ShaderResourceView* m_shaderResourceView;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11Device* m_device;
	FilterMode m_filterMode;
	WrapMode m_wrapMode;
	D3D11RenderStateMachine* m_stateMachine;
	//ID3D11Texture2D* m_texture;
	bool m_isWritable;
	//bool m_isCreated;
};