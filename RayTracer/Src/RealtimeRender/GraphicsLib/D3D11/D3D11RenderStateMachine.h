#pragma once

#include <map>
#include <d3d11.h>

#include "../GLDefines.h"

class D3D11RenderStateMachine
{
	struct D3D11RasterizerState
	{
	public:
		unsigned int GetKey();

	public:
		CullMode cullMode;
		FillMode fillMode;
	};

	struct D3D11DepthStencilState
	{
	public:
		unsigned long GetKey();

	public:
		bool zwrite;
		bool enableStencil;
		UINT stencilId;
		CompareFunc ztest;
		CompareFunc stencilComp;
		StencilOp stencilPassOp;
		StencilOp stencilFailOp;
		StencilOp stencilZFailOp;
		UINT8 stencilReadMask;
		UINT8 stencilWriteMask;
	};

	struct D3D11BlendState
	{
	public:
		unsigned long GetKey();

	public:
		bool enableBlend;
		BlendOp blendOp;
		BlendFactor srcfactor;
		BlendFactor dstfactor;
	};
public:
	D3D11RenderStateMachine(ID3D11Device*, ID3D11DeviceContext*);
	~D3D11RenderStateMachine();

	void SetRasterizerState(CullMode cullMode, FillMode fillMode, bool forceApply);

	void SetDepthStencilState(bool zwrite, bool enableStencil, UINT stencilId, CompareFunc ztest, CompareFunc stenciltest, StencilOp stencilpass, StencilOp stencilfail, StencilOp zfail, UINT8 sreadmask, UINT8 swritemask, bool forceApply);

	void SetBlendEnable(bool enable, BlendOp blendOp, BlendFactor srcFactor, BlendFactor dstFactor, bool forceApply);

	class SamplerState* GetSamplerState(FilterMode filterMode, WrapMode wrapMode);

private:
	void RefreshRasterizerState();
	void RefreshDepthStencilState();
	void RefreshBlendState();
	HRESULT CreateRasterizerState(D3D11RasterizerState, ID3D11RasterizerState**);
	HRESULT CreateDepthStencilState(D3D11DepthStencilState, ID3D11DepthStencilState**);
	HRESULT CreateBlendState(D3D11BlendState, ID3D11BlendState**);
	HRESULT CreateDisableBlendState(ID3D11BlendState**);

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	std::map<unsigned long, ID3D11DepthStencilState*> m_depthStencilStates;
	std::map<unsigned long, ID3D11BlendState*> m_blendStates;
	std::map<unsigned int, ID3D11RasterizerState*> m_rasterizerStates;
	std::map<unsigned int, class SamplerState*> m_samplerStates;
	ID3D11BlendState* m_disableBlendState;

	D3D11RasterizerState m_currentRasterizerState;
	D3D11DepthStencilState m_currentDepthStencilState;
	D3D11BlendState m_currentBlendState;
};
