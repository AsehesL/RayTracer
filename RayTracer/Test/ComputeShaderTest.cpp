#include "ComputeShaderTest.h"
#include "../Src/RealtimeRender/GraphicsLib/D3D11/D3D11Context.h"
#include <D3Dcompiler.h>
#include <d3dx11async.h>

ComputeShaderTest::ComputeShaderTest(LPCWSTR shaderPath, GLContext* glContext)
{
	D3D11Context* d3dContext = (D3D11Context*)glContext;

	m_computeShader = nullptr;

	m_deviceContext = d3dContext->GetDeviceContext();
	m_device = d3dContext->GetDevice();

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT result = D3DX11CompileFromFile(shaderPath, NULL, NULL, "CSMain", "cs_5_0", D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_SKIP_OPTIMIZATION | D3D10_SHADER_DEBUG, 0, NULL, &shaderBlob, &errorBlob, NULL);

	if (FAILED(result))
	{
		if (errorBlob)
		{
			char* errormessage = (char*)errorBlob->GetBufferPointer();
			errorBlob->Release();
		}

		if (shaderBlob)
			shaderBlob->Release();
		return;
	}

	result = m_device->CreateComputeShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), nullptr, &m_computeShader);
	shaderBlob->Release();
	if (FAILED(result))
		return;
}

ComputeShaderTest::~ComputeShaderTest()
{
	if (m_computeShader)
		m_computeShader->Release();
	m_computeShader = nullptr;
}

int ComputeShaderTest::Dispach()
{
	m_deviceContext->CSSetShader(m_computeShader, NULL, 0);

	m_deviceContext->Dispatch(16,1,1);

	m_deviceContext->CSSetShader(NULL, NULL, 0);
	return 0;
}
