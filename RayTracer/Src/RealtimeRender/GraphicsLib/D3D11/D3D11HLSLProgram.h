#pragma once

#include "../ShaderProgram.h"

#include <d3d11.h>
#include <d3dx11async.h>

class D3D11ShaderUniformBuffer : public IShaderUniformBuffer
{
public:
	D3D11ShaderUniformBuffer(ID3D11Device* device, ID3D11DeviceContext* deviceContext, unsigned int bufferSize);
	~D3D11ShaderUniformBuffer() override;

	bool IsValid() override;
	void* Map() override;
	void Unmap() override;
	void ApplyVSUniformBuffer(int position) override;
	void ApplyPSUniformBuffer(int position) override;

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11Buffer* m_uniformBuffer;
	bool m_isValid;
	bool m_isMapped;
};

class D3D11HLSLProgram : public IShaderProgram
{
public:
	D3D11HLSLProgram(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	virtual ~D3D11HLSLProgram() override;

	bool Compile(const WCHAR* vsPath, const WCHAR* psPath) override;
	bool Execute() override;
	bool IsValid() override;

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11InputLayout* m_layout;
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	bool m_isValid;
};