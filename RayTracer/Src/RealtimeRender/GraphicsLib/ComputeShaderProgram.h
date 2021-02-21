#pragma once
#include "../../../framework.h"

class ComputeShaderProgram
{
public:
	virtual ~ComputeShaderProgram() {};
	virtual bool Compile(const WCHAR* csPath) = 0;
	virtual bool CSSetShader() = 0;
	virtual void CSClearShader() = 0;
	virtual void Dispatch(int threadGroupsX, int threadGroupsY, int threadGroupsZ) = 0;
	virtual bool IsValid() = 0;
};