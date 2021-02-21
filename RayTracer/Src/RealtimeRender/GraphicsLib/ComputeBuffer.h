#pragma once

class ComputeBuffer
{
public:
	virtual ~ComputeBuffer() {};

	virtual bool IsValid() = 0;
	virtual void* Map() = 0;
	virtual void Unmap() = 0;
	virtual void CSSetUAV(unsigned int slot) = 0;
};