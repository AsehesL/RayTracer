#pragma once
#include "../../../framework.h"

class IShaderUniformBuffer
{
public:
	virtual ~IShaderUniformBuffer() {}
	virtual bool IsValid() = 0;
	virtual void* Map() = 0;
	virtual void Unmap() = 0;
	virtual void ApplyVSUniformBuffer(int position) = 0;
	virtual void ApplyPSUniformBuffer(int position) = 0;
};

class IShaderProgram
{
public:
	virtual ~IShaderProgram() {}
	virtual bool Compile(const WCHAR* vsPath, const WCHAR* psPath) = 0;
	virtual bool Execute() = 0;
	virtual bool IsValid() = 0;
};