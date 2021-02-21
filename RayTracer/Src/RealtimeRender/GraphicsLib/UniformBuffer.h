#pragma once

class UniformBuffer
{
public:
	virtual ~UniformBuffer() {}
	virtual bool IsValid() = 0;
	virtual void* Map() = 0;
	virtual void Unmap() = 0;
	virtual void VSSetUniformBuffer(int slot) = 0;
	virtual void PSSetUniformBuffer(int slot) = 0;
	virtual void CSSetUniformBuffer(int slot) = 0;
};