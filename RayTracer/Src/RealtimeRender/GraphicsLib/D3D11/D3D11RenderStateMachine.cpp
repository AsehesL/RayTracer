#include "D3D11RenderStateMachine.h"

D3D11_COMPARISON_FUNC ToD3D11ComparisonFunc(CompareFunc func)
{
	switch (func)
	{
	case CompareFunc::Always:
		return D3D11_COMPARISON_ALWAYS;
	case CompareFunc::Equal:
		return D3D11_COMPARISON_EQUAL;
	case CompareFunc::GEqual:
		return D3D11_COMPARISON_GREATER_EQUAL;
	case CompareFunc::Greater:
		return D3D11_COMPARISON_GREATER;
	case CompareFunc::LEqual:
		return D3D11_COMPARISON_LESS_EQUAL;
	case CompareFunc::Less:
		return D3D11_COMPARISON_LESS;
	case CompareFunc::Never:
		return D3D11_COMPARISON_NEVER;
	case CompareFunc::NotEqual:
		return D3D11_COMPARISON_NOT_EQUAL;
	default:
		return D3D11_COMPARISON_LESS_EQUAL;
	}
}

D3D11_STENCIL_OP ToD3D11StencilOp(StencilOp op)
{
	switch (op)
	{
	case StencilOp::Keep:
		return D3D11_STENCIL_OP_KEEP;
	case StencilOp::Zero:
		return D3D11_STENCIL_OP_ZERO;
	case StencilOp::Replace:
		return D3D11_STENCIL_OP_REPLACE;
	case StencilOp::IncrementSaturate:
		return D3D11_STENCIL_OP_INCR_SAT;
	case StencilOp::DecrementSaturate:
		return D3D11_STENCIL_OP_DECR_SAT;
	case StencilOp::Invert:
		return D3D11_STENCIL_OP_INVERT;
	case StencilOp::IncrementWrap:
		return D3D11_STENCIL_OP_INCR;
	case StencilOp::DecrementWrap:
		return D3D11_STENCIL_OP_DECR;
	default:
		return D3D11_STENCIL_OP_KEEP;
	}
}

D3D11_FILL_MODE ToD3D11FillMode(FillMode fillMode)
{
	if (fillMode == FillMode::Solid)
		return D3D11_FILL_SOLID;
	else if (fillMode == FillMode::WireFrame)
		return D3D11_FILL_WIREFRAME;
	return D3D11_FILL_SOLID;
}

D3D11_CULL_MODE ToD3D11CullMode(CullMode cullMode)
{
	if (cullMode == CullMode::Off)
		return D3D11_CULL_NONE;
	else if (cullMode == CullMode::Back)
		return D3D11_CULL_BACK;
	else if (cullMode == CullMode::Front)
		return D3D11_CULL_FRONT;
	return D3D11_CULL_BACK;
}

D3D11_BLEND_OP ToD3D11BlendOp(BlendOp op)
{
	switch (op)
	{
	case BlendOp::Add:
		return D3D11_BLEND_OP_ADD;
	case BlendOp::Sub:
		return D3D11_BLEND_OP_SUBTRACT;
	case BlendOp::RevSub:
		return D3D11_BLEND_OP_REV_SUBTRACT;
	case BlendOp::Min:
		return D3D11_BLEND_OP_MIN;
	case BlendOp::Max:
		return D3D11_BLEND_OP_MAX;
	default:
		return D3D11_BLEND_OP_ADD;
	}
}

D3D11_BLEND ToD3D11BlendFactor(BlendFactor factor)
{
	switch (factor)
	{
	case BlendFactor::One:
		return D3D11_BLEND_ONE;
	case BlendFactor::Zero:
		return D3D11_BLEND_ZERO;
	case BlendFactor::SrcColor:
		return D3D11_BLEND_SRC_COLOR;
	case BlendFactor::SrcAlpha:
		return D3D11_BLEND_SRC_ALPHA;
	case BlendFactor::DstColor:
		return D3D11_BLEND_DEST_COLOR;
	case BlendFactor::DstAlpha:
		return D3D11_BLEND_DEST_ALPHA;
	case BlendFactor::OneMinusSrcColor:
		return D3D11_BLEND_INV_SRC_COLOR;
	case BlendFactor::OneMinusSrcAlpha:
		return D3D11_BLEND_INV_SRC_ALPHA;
	case BlendFactor::OneMinusDstColor:
		return D3D11_BLEND_INV_DEST_COLOR;
	case BlendFactor::OneMinusDstAlpha:
		return D3D11_BLEND_INV_DEST_ALPHA;
	default:
		return D3D11_BLEND_ZERO;
	}
}

unsigned D3D11RenderStateMachine::D3D11RasterizerState::GetKey()
{
	unsigned int key = 0;
	key = (unsigned int)fillMode << 2;
	key = key | (unsigned int)cullMode;
	return key;
}

unsigned long D3D11RenderStateMachine::D3D11DepthStencilState::GetKey()
{
	unsigned long key = 0;
	if (zwrite)
		key = 1L << 31;
	unsigned long func = (unsigned int)ztest << 28;
	key = key | func;
	if (enableStencil)
	{
		key = key | ((unsigned int)stencilWriteMask << 20);
		key = key | ((unsigned int)stencilReadMask << 12);
		key = key | ((unsigned int)stencilComp << 9);
		key = key | ((unsigned int)stencilPassOp << 6);
		key = key | ((unsigned int)stencilFailOp << 3);
		key = key | ((unsigned int)stencilZFailOp);
	}

	return key;
}

unsigned long D3D11RenderStateMachine::D3D11BlendState::GetKey()
{
	unsigned long key = 0;
	key = (unsigned int)blendOp << 8;
	key = key | ((unsigned int)srcfactor << 4);
	key = key | (unsigned int)dstfactor;
	return key;
}

unsigned int D3D11RenderStateMachine::D3D11SamplerState::GetKey()
{
	unsigned int key = (unsigned int)filterMode << 8;
	key = key | ((unsigned int)wrapMode);
	return key;
}

D3D11RenderStateMachine::D3D11RenderStateMachine(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	m_device = device;
	m_deviceContext = deviceContext;
	m_hasSampler = false;
}

D3D11RenderStateMachine::~D3D11RenderStateMachine()
{
	m_device = NULL;
	m_deviceContext = NULL;
	std::map<unsigned int, ID3D11RasterizerState*>::iterator  riter;
	for (riter = m_rasterizerStates.begin(); riter != m_rasterizerStates.end(); riter++)
	{
		if (riter->second != NULL)
		{
			riter->second->Release();
		}
	}
	m_rasterizerStates.clear();

	std::map<unsigned long, ID3D11DepthStencilState*>::iterator  diter;
	for (diter = m_depthStencilStates.begin(); diter != m_depthStencilStates.end(); diter++)
	{
		if (diter->second != NULL)
		{
			diter->second->Release();
		}
	}
	m_depthStencilStates.clear();

	std::map<unsigned long, ID3D11BlendState*>::iterator  biter;
	for (biter = m_blendStates.begin(); biter != m_blendStates.end(); biter++)
	{
		if (biter->second != NULL)
		{
			biter->second->Release();
		}
	}
	m_blendStates.clear();

	if (m_disableBlendState != NULL)
	{
		m_disableBlendState->Release();
		m_disableBlendState = NULL;
	}

	std::map<unsigned int, ID3D11SamplerState*>::iterator  siter;
	for (siter = m_samplerStates.begin(); siter != m_samplerStates.end(); siter++)
	{
		if (siter->second != NULL)
		{
			siter->second->Release();
		}
	}
	m_samplerStates.clear();
}

void D3D11RenderStateMachine::SetRasterizerState(CullMode cullMode, FillMode fillMode, bool forceApply)
{
	if (m_device == NULL || m_deviceContext == NULL)
		return;
	if (!forceApply && m_currentRasterizerState.cullMode == cullMode && m_currentRasterizerState.fillMode == fillMode)
		return;
	m_currentRasterizerState.cullMode = cullMode;
	m_currentRasterizerState.fillMode = fillMode;
	RefreshRasterizerState();
}

void D3D11RenderStateMachine::SetDepthStencilState(bool zwrite, bool enableStencil, UINT stencilId, CompareFunc ztest,
	CompareFunc stenciltest, StencilOp stencilpass, StencilOp stencilfail, StencilOp zfail, UINT8 sreadmask,
	UINT8 swritemask, bool forceApply)
{
	if (m_device == NULL || m_deviceContext == NULL)
		return;
	if (!forceApply && m_currentDepthStencilState.zwrite == zwrite
		&& m_currentDepthStencilState.enableStencil == enableStencil
		&& m_currentDepthStencilState.stencilId == stencilId
		&& m_currentDepthStencilState.ztest == ztest
		&& m_currentDepthStencilState.stencilComp == stenciltest
		&& m_currentDepthStencilState.stencilPassOp == stencilpass
		&& m_currentDepthStencilState.stencilFailOp == stencilfail
		&& m_currentDepthStencilState.stencilZFailOp == zfail
		&& m_currentDepthStencilState.stencilReadMask == sreadmask
		&& m_currentDepthStencilState.stencilWriteMask == swritemask)
		return;
	m_currentDepthStencilState.zwrite = zwrite;
	m_currentDepthStencilState.enableStencil = enableStencil;
	m_currentDepthStencilState.stencilId = stencilId;
	m_currentDepthStencilState.ztest = ztest;
	m_currentDepthStencilState.stencilComp = stenciltest;
	m_currentDepthStencilState.stencilPassOp = stencilpass;
	m_currentDepthStencilState.stencilFailOp = stencilfail;
	m_currentDepthStencilState.stencilZFailOp = zfail;
	m_currentDepthStencilState.stencilReadMask = sreadmask;
	m_currentDepthStencilState.stencilWriteMask = swritemask;
	RefreshDepthStencilState();
}

void D3D11RenderStateMachine::SetBlendEnable(bool enable, BlendOp blendOp, BlendFactor srcFactor,
	BlendFactor dstFactor, bool forceApply)
{
	if (m_device == NULL || m_deviceContext == NULL)
		return;
	if (forceApply || (!forceApply && m_currentBlendState.enableBlend != enable) || (!forceApply && (m_currentBlendState.blendOp != blendOp || m_currentBlendState.srcfactor != srcFactor || m_currentBlendState.dstfactor != dstFactor)))
	{
		m_currentBlendState.enableBlend = enable;
		m_currentBlendState.blendOp = blendOp;
		m_currentBlendState.srcfactor = srcFactor;
		m_currentBlendState.dstfactor = dstFactor;
		RefreshBlendState();
	}
}

void D3D11RenderStateMachine::SetSamplerState(UINT startSlot, FilterMode filterMode, WrapMode wrapMode)
{
	if (m_device == NULL || m_deviceContext == NULL)
		return;
	if (!m_hasSampler || (m_hasSampler && (m_currentSamplerState.filterMode != filterMode || m_currentSamplerState.wrapMode != wrapMode)))
	{
		m_hasSampler = true;
		m_currentSamplerState.filterMode = filterMode;
		m_currentSamplerState.wrapMode = wrapMode;
		RefreshSamplerState(startSlot);
	}
}

void D3D11RenderStateMachine::RefreshRasterizerState()
{
	unsigned int key = m_currentRasterizerState.GetKey();
	if (m_rasterizerStates.find(key) != m_rasterizerStates.end())
	{
		ID3D11RasterizerState* state = m_rasterizerStates.at(key);
		m_deviceContext->RSSetState(state);
	}
	else
	{
		ID3D11RasterizerState* state;
		HRESULT result = CreateRasterizerState(m_currentRasterizerState, &state);
		if (!FAILED(result))
		{
			m_rasterizerStates.insert(std::pair<unsigned int, ID3D11RasterizerState*>(key, state));
			m_deviceContext->RSSetState(state);
		}
	}
}

void D3D11RenderStateMachine::RefreshDepthStencilState()
{
	unsigned long key = m_currentDepthStencilState.GetKey();
	if (m_depthStencilStates.find(key) != m_depthStencilStates.end())
	{
		ID3D11DepthStencilState* state = m_depthStencilStates.at(key);
		m_deviceContext->OMSetDepthStencilState(state, m_currentDepthStencilState.stencilId);
	}
	else
	{
		ID3D11DepthStencilState* state;
		HRESULT result = CreateDepthStencilState(m_currentDepthStencilState, &state);
		if (!FAILED(result))
		{
			m_depthStencilStates.insert(std::pair<unsigned long, ID3D11DepthStencilState*>(key, state));
			m_deviceContext->OMSetDepthStencilState(state, m_currentDepthStencilState.stencilId);
		}
	}
}

void D3D11RenderStateMachine::RefreshBlendState()
{
	float blendFactor[4];

	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	if (m_currentBlendState.enableBlend == false)
	{
		if (m_disableBlendState == NULL)
		{
			ID3D11BlendState* state = 0;
			HRESULT result = CreateDisableBlendState(&state);
			if (!FAILED(result))
			{
				m_disableBlendState = state;
				m_deviceContext->OMSetBlendState(m_disableBlendState, blendFactor, 0xffffffff);
			}
		}
		else
		{
			m_deviceContext->OMSetBlendState(m_disableBlendState, blendFactor, 0xffffffff);
		}
	}
	else
	{

		unsigned long key = m_currentBlendState.GetKey();

		if (m_blendStates.find(key) != m_blendStates.end())
		{
			ID3D11BlendState* state = m_blendStates.at(key);
			m_deviceContext->OMSetBlendState(state, blendFactor, 0xffffffff);
		}
		else
		{
			ID3D11BlendState* state;
			HRESULT result = CreateBlendState(m_currentBlendState, &state);
			if (!FAILED(result))
			{
				m_blendStates.insert(std::pair<unsigned long, ID3D11BlendState*>(key, state));
				m_deviceContext->OMSetBlendState(state, blendFactor, 0xffffffff);
			}
		}
	}
}

void D3D11RenderStateMachine::RefreshSamplerState(UINT startSlot)
{
	unsigned int key = m_currentSamplerState.GetKey();
	if (m_samplerStates.find(key) != m_samplerStates.end())
	{
		ID3D11SamplerState* state = m_samplerStates.at(key);
		m_deviceContext->PSSetSamplers(startSlot, 1, &state);
	}
	else
	{
		ID3D11SamplerState* state;
		HRESULT result = CreateSamplerState(m_currentSamplerState, &state);
		if (!FAILED(result))
		{
			m_samplerStates.insert(std::pair<unsigned int, ID3D11SamplerState*>(key, state));
			m_deviceContext->PSSetSamplers(startSlot, 1, &state);
		}
	}
}

HRESULT D3D11RenderStateMachine::CreateRasterizerState(D3D11RasterizerState stateDesc, ID3D11RasterizerState** state)
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.AntialiasedLineEnable = false;
	desc.CullMode = ToD3D11CullMode(stateDesc.cullMode);
	desc.DepthBias = 0;
	desc.DepthBiasClamp = 0.0f;
	desc.DepthClipEnable = true;
	desc.FillMode = ToD3D11FillMode(stateDesc.fillMode);
	desc.FrontCounterClockwise = false;
	desc.MultisampleEnable = false;
	desc.ScissorEnable = false;
	desc.SlopeScaledDepthBias = 0.0f;

	HRESULT result = m_device->CreateRasterizerState(&desc, state);

	return result;
}

HRESULT D3D11RenderStateMachine::CreateDepthStencilState(D3D11DepthStencilState stateDesc, ID3D11DepthStencilState** state)
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	if (stateDesc.zwrite)
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	else
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;

	depthStencilDesc.DepthFunc = ToD3D11ComparisonFunc(stateDesc.ztest);

	depthStencilDesc.StencilEnable = stateDesc.enableStencil;
	if (!stateDesc.enableStencil)
	{
		depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	}
	else
	{
		depthStencilDesc.StencilReadMask = stateDesc.stencilReadMask;
		depthStencilDesc.StencilWriteMask = stateDesc.stencilWriteMask;

		depthStencilDesc.FrontFace.StencilFailOp = ToD3D11StencilOp(stateDesc.stencilFailOp);
		depthStencilDesc.FrontFace.StencilDepthFailOp = ToD3D11StencilOp(stateDesc.stencilZFailOp);
		depthStencilDesc.FrontFace.StencilPassOp = ToD3D11StencilOp(stateDesc.stencilPassOp);
		depthStencilDesc.FrontFace.StencilFunc = ToD3D11ComparisonFunc(stateDesc.stencilComp);

		depthStencilDesc.BackFace.StencilFailOp = ToD3D11StencilOp(stateDesc.stencilFailOp);
		depthStencilDesc.BackFace.StencilDepthFailOp = ToD3D11StencilOp(stateDesc.stencilZFailOp);
		depthStencilDesc.BackFace.StencilPassOp = ToD3D11StencilOp(stateDesc.stencilPassOp);
		depthStencilDesc.BackFace.StencilFunc = ToD3D11ComparisonFunc(stateDesc.stencilComp);
	}

	HRESULT result = m_device->CreateDepthStencilState(&depthStencilDesc, state);
	return result;
}

HRESULT D3D11RenderStateMachine::CreateBlendState(D3D11BlendState desc, ID3D11BlendState** state)
{
	D3D11_BLEND_DESC blenddesc;
	ZeroMemory(&blenddesc, sizeof(blenddesc));

	blenddesc.RenderTarget[0].BlendEnable = TRUE;
	blenddesc.RenderTarget[0].BlendOp = ToD3D11BlendOp(desc.blendOp);
	blenddesc.RenderTarget[0].BlendOpAlpha = ToD3D11BlendOp(desc.blendOp);
	blenddesc.RenderTarget[0].DestBlend = ToD3D11BlendFactor(desc.dstfactor);
	blenddesc.RenderTarget[0].DestBlendAlpha = ToD3D11BlendFactor(desc.dstfactor);
	blenddesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	blenddesc.RenderTarget[0].SrcBlend = ToD3D11BlendFactor(desc.srcfactor);
	blenddesc.RenderTarget[0].SrcBlendAlpha = ToD3D11BlendFactor(desc.srcfactor);

	HRESULT result = m_device->CreateBlendState(&blenddesc, state);
	return result;
}

HRESULT D3D11RenderStateMachine::CreateDisableBlendState(ID3D11BlendState** state)
{
	D3D11_BLEND_DESC blenddesc;
	ZeroMemory(&blenddesc, sizeof(blenddesc));

	blenddesc.RenderTarget[0].BlendEnable = FALSE;
	blenddesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT result = m_device->CreateBlendState(&blenddesc, state);
	return result;
}

HRESULT D3D11RenderStateMachine::CreateSamplerState(D3D11SamplerState stateDesc, ID3D11SamplerState** state)
{
	D3D11_TEXTURE_ADDRESS_MODE u = D3D11_TEXTURE_ADDRESS_WRAP;
	D3D11_TEXTURE_ADDRESS_MODE v = D3D11_TEXTURE_ADDRESS_WRAP;
	if (stateDesc.wrapMode == WrapMode::Clamp)
	{ 
		u = D3D11_TEXTURE_ADDRESS_CLAMP;
		v = D3D11_TEXTURE_ADDRESS_CLAMP;
	}

	D3D11_FILTER filter;
	if (stateDesc.filterMode == FilterMode::Point)
		filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	else if (stateDesc.filterMode == FilterMode::Bilinear)
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

	return m_device->CreateSamplerState(&samplerDesc, state);
}



