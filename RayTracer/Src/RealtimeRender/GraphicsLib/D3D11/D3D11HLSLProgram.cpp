#include "D3D11HLSLProgram.h"
#include "D3D11UniformBuffer.h"
#include <D3Dcompiler.h>
#include <d3dx11async.h>

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
	for (auto kvp : m_uniformBuffers)
	{
		UniformBuffer* uniformBuffer = kvp.second;
		if (uniformBuffer)
			delete uniformBuffer;
	}
	m_uniformBuffers.clear();
}

bool D3D11HLSLProgram::Compile(const WCHAR* vsPath, const WCHAR* psPath)
{
	HRESULT result;
	ID3D10Blob* vertexShaderBlob;
	ID3D10Blob* pixelShaderBlob;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[4];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	vertexShaderBlob = 0;
	pixelShaderBlob = 0;

#if _DEBUG
	result = D3DX11CompileFromFile(vsPath, NULL, NULL, "VertexFunc", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_SKIP_OPTIMIZATION | D3D10_SHADER_DEBUG, 0, NULL, &vertexShaderBlob, NULL, NULL);
#else
	result = D3DX11CompileFromFile(vsPath, NULL, NULL, "VertexFunc", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBlob, NULL, NULL);
#endif

	if (FAILED(result))
	{
		vertexShaderBlob->Release();
		return false;
	}

#if _DEBUG
	result = D3DX11CompileFromFile(psPath, NULL, NULL, "PixelFunc", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_SKIP_OPTIMIZATION | D3D10_SHADER_DEBUG, 0, NULL, &pixelShaderBlob, NULL, NULL);
#else
	result = D3DX11CompileFromFile(psPath, NULL, NULL, "PixelFunc", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBlob, NULL, NULL);
#endif
	if (FAILED(result))
	{
		vertexShaderBlob->Release();
		pixelShaderBlob->Release();
		return false;
	}

	result = m_device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), NULL, &m_vertexShader);
	if (FAILED(result))
	{
		vertexShaderBlob->Release();
		pixelShaderBlob->Release();
		return false;
	}

	result = m_device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), NULL, &m_pixelShader);
	if (FAILED(result))
	{
		vertexShaderBlob->Release();
		pixelShaderBlob->Release();
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

	result = m_device->CreateInputLayout(polygonLayout, numElements, vertexShaderBlob->GetBufferPointer(),
		vertexShaderBlob->GetBufferSize(), &m_layout);
	if (FAILED(result))
	{
		vertexShaderBlob->Release();
		pixelShaderBlob->Release();
		return false;
	}

	ID3D11ShaderReflection* vertexShaderReflection = nullptr;

	if (FAILED(D3DReflect(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&vertexShaderReflection)))
	{
		vertexShaderBlob->Release();
		pixelShaderBlob->Release();
		return false;
	}

	ID3D11ShaderReflection* pixelShaderReflection = nullptr;

	if (FAILED(D3DReflect(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pixelShaderReflection)))
	{
		vertexShaderBlob->Release();
		pixelShaderBlob->Release();
		return false;
	}

	InitUniformBuffers(vertexShaderReflection);
	InitUniformBuffers(pixelShaderReflection);

	vertexShaderBlob->Release();
	vertexShaderBlob = 0;

	pixelShaderBlob->Release();
	pixelShaderBlob = 0;

	vertexShaderReflection->Release();
	pixelShaderReflection->Release();

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

UniformBuffer* D3D11HLSLProgram::GetUniformBuffer(const char* key)
{
	if (m_uniformBuffers.find(key) != m_uniformBuffers.end())
		return m_uniformBuffers.at(key);
	return nullptr;
}

void D3D11HLSLProgram::InitUniformBuffers(ID3D11ShaderReflection* pShaderReflection)
{
	D3D11_SHADER_DESC shaderDesc;
	pShaderReflection->GetDesc(&shaderDesc);

	for (UINT i = 0; i < shaderDesc.ConstantBuffers; ++i)
	{
		ID3D11ShaderReflectionConstantBuffer* bf = pShaderReflection->GetConstantBufferByIndex(i);
		D3D11_SHADER_BUFFER_DESC desc;
		bf->GetDesc(&desc);

		if (m_uniformBuffers.find(desc.Name) == m_uniformBuffers.end())
		{
			D3D11UniformBuffer* uniformBuffer = new D3D11UniformBuffer(m_device, m_deviceContext, desc.Size);
			m_uniformBuffers.insert(std::pair<std::string, UniformBuffer*>(desc.Name, uniformBuffer));
		}
	}
}

D3D11ComputeShaderProgram::D3D11ComputeShaderProgram(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	m_computeShader = NULL;
	m_deviceContext = deviceContext;
	m_device = device;

	m_isValid = false;
}

D3D11ComputeShaderProgram::~D3D11ComputeShaderProgram()
{
	if (m_computeShader)
	{
		m_computeShader->Release();
		m_computeShader = 0;
	}
}

bool D3D11ComputeShaderProgram::Compile(const WCHAR* csPath)
{
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
#if _DEBUG
	HRESULT result = D3DX11CompileFromFile(csPath, NULL, NULL, "CSMain", "cs_5_0", D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_SKIP_OPTIMIZATION | D3D10_SHADER_DEBUG, 0, NULL, &shaderBlob, &errorBlob, NULL);
#else
	HRESULT result = D3DX11CompileFromFile(csPath, NULL, NULL, "CSMain", "cs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &shaderBlob, NULL, NULL);
#endif 

	if (FAILED(result))
	{
		if (errorBlob)
		{
			char* errormessage = (char*)errorBlob->GetBufferPointer();
			errorBlob->Release();
		}

		if (shaderBlob)
			shaderBlob->Release();
		return false;
	}

	result = m_device->CreateComputeShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &m_computeShader);
	
	if (FAILED(result))
		return false;

	shaderBlob->Release();

	m_isValid = true;
}

bool D3D11ComputeShaderProgram::CSSetShader()
{
	if (!IsValid())
		return false;
	if (m_computeShader)
	{
		m_deviceContext->CSSetShader(m_computeShader, NULL, 0);
		return true;
	}
	return false;
}

void D3D11ComputeShaderProgram::CSClearShader()
{
	if (!IsValid())
		return;
	if (m_computeShader)
	{
		m_deviceContext->CSSetShader(NULL, NULL, 0);
		return;
	}
	return;
}

void D3D11ComputeShaderProgram::Dispatch(int threadGroupsX, int threadGroupsY, int threadGroupsZ)
{
	if (!IsValid())
		return;
	if (m_computeShader)
	{
		m_deviceContext->Dispatch(threadGroupsX, threadGroupsY, threadGroupsZ);
	}
}

bool D3D11ComputeShaderProgram::IsValid()
{
	return m_isValid;
}
