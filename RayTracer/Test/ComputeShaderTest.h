#pragma once
#include <dxgi.h>
#include <d3d11.h>

// a test for compile compute shader
class ComputeShaderTest
{
public:
	ComputeShaderTest(LPCWSTR shaderPath, class GLContext* glContext);
	~ComputeShaderTest();

	int Dispach();

private:
	ID3D11DeviceContext* m_deviceContext;
	ID3D11Device* m_device;
	ID3D11ComputeShader* m_computeShader;
};