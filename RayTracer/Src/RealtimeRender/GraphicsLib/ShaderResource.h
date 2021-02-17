#pragma once

struct IShaderResource
{
public:
	virtual void SetShaderResource(unsigned int slot) = 0;
};