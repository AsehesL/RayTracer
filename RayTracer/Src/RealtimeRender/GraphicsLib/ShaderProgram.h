#pragma once
#include "../../../framework.h"

class ShaderProgram
{
public:
	virtual ~ShaderProgram() {}
	virtual bool Compile(const WCHAR* vsPath, const WCHAR* psPath) = 0;
	virtual bool Execute() = 0;
	virtual bool IsValid() = 0;
	virtual class UniformBuffer* GetUniformBuffer(const char* key) = 0;
};