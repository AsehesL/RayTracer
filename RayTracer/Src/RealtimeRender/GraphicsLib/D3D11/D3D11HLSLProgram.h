#pragma once

#include "../ShaderProgram.h"
#include "../ComputeShaderProgram.h"

#include <d3d11.h>
#include <map>
#include <string>

class D3D11HLSLProgram : public ShaderProgram
{
public:
	D3D11HLSLProgram(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	virtual ~D3D11HLSLProgram() override;

	bool Compile(const WCHAR* vsPath, const WCHAR* psPath) override;
	bool Execute() override;
	bool IsValid() override;
	class UniformBuffer* GetUniformBuffer(const char* key) override;

private:
	void InitUniformBuffers(class ID3D11ShaderReflection* pShaderReflection);

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11InputLayout* m_layout;
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	bool m_isValid;

	std::map<std::string, class UniformBuffer*> m_uniformBuffers;
};

class D3D11ComputeShaderProgram : public ComputeShaderProgram
{
public:
	D3D11ComputeShaderProgram(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	virtual ~D3D11ComputeShaderProgram() override;
	bool Compile(const WCHAR* csPath) override;
	bool CSSetShader() override;
	void CSClearShader() override;
	void Dispatch(int threadGroupsX, int threadGroupsY, int threadGroupsZ) override;
	bool IsValid() override;

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11ComputeShader* m_computeShader;
	bool m_isValid;
};