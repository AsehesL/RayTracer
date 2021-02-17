#include "D3D11HLSLProgram.h"

D3D11ShaderUniformBuffer::D3D11ShaderUniformBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, unsigned int bufferSize)
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

D3D11ShaderUniformBuffer::~D3D11ShaderUniformBuffer()
{
	if (m_uniformBuffer)
		m_uniformBuffer->Release();
	m_uniformBuffer = nullptr;
}

bool D3D11ShaderUniformBuffer::IsValid()
{
	return m_isValid;
}

void* D3D11ShaderUniformBuffer::Map()
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

void D3D11ShaderUniformBuffer::Unmap()
{
	if (!IsValid())
		return;
	if (!m_isMapped)
		return;
	m_isMapped = false;

	m_deviceContext->Unmap(m_uniformBuffer, 0);
}

void D3D11ShaderUniformBuffer::ApplyVSUniformBuffer(int position)
{
	if (!IsValid())
		return;
	if (m_isMapped)
		return;
	m_deviceContext->VSSetConstantBuffers(position, 1, &m_uniformBuffer);
}

void D3D11ShaderUniformBuffer::ApplyPSUniformBuffer(int position)
{
	if (!IsValid())
		return;
	if (m_isMapped)
		return;
	m_deviceContext->PSSetConstantBuffers(position, 1, &m_uniformBuffer);
}

D3D11HLSLProgram::D3D11HLSLProgram(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	m_vertexShader = NULL;
	m_pixelShader = NULL;
	m_layout = NULL;
	m_deviceContext = deviceContext;
	m_device = device;

	m_isValid = false;
}

D3D11HLSLProgram::~D3D11HLSLProgram()
{
	if (m_layout)
	{
		m_layout->Release();
		m_layout = 0;
	}

	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = 0;
	}

	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = 0;
	}
}

bool D3D11HLSLProgram::Compile(const WCHAR* vsPath, const WCHAR* psPath)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[4];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	errorMessage = 0;
	vertexShaderBuffer = 0;
	pixelShaderBuffer = 0;

#if _DEBUG
	result = D3DX11CompileFromFile(vsPath, NULL, NULL, "VertexFunc", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_SKIP_OPTIMIZATION | D3D10_SHADER_DEBUG, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);
#else
	result = D3DX11CompileFromFile(vsPath, NULL, NULL, "VertexFunc", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);
#endif

	if (FAILED(result))
	{
		return false;
	}

#if _DEBUG
	result = D3DX11CompileFromFile(psPath, NULL, NULL, "PixelFunc", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_SKIP_OPTIMIZATION | D3D10_SHADER_DEBUG, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
#else
	result = D3DX11CompileFromFile(psPath, NULL, NULL, "PixelFunc", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
#endif
	if (FAILED(result))
	{
		return false;
	}

	result = m_device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

	result = m_device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		return false;
	}

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "NORMAL";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "TEXCOORD";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	polygonLayout[3].SemanticName = "TANGENT";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	result = m_device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		return false;
	}

	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	m_isValid = true;

	return true;
}

bool D3D11HLSLProgram::Execute()
{
	if (!IsValid())
		return false;
	if (m_vertexShader && m_pixelShader)
	{ 
		m_deviceContext->IASetInputLayout(m_layout);

		m_deviceContext->VSSetShader(m_vertexShader, NULL, 0);
		m_deviceContext->PSSetShader(m_pixelShader, NULL, 0);
		return true;
	}
	return false;
}

bool D3D11HLSLProgram::IsValid()
{
	return m_isValid;
}
