#pragma once
#include "GLDefines.h"

class SamplerState
{
public:
	virtual ~SamplerState() {}
	virtual void VSSetSampler(unsigned int slot) = 0;
	virtual void PSSetSampler(unsigned int slot) = 0;
	virtual void CSSetSampler(unsigned int slot) = 0;
};

class TextureBuffer
{
public:
	virtual ~TextureBuffer() {}
	virtual void Set2DBuffer(unsigned int width, unsigned int height, const void* data) = 0;
	virtual void SetCubeBuffer(TextureBuffer* left, TextureBuffer* right, TextureBuffer* top, TextureBuffer* bottom, TextureBuffer* front, TextureBuffer* back) = 0;
	virtual SamplerState* GetSampler() = 0;
	virtual void SetFilterMode(FilterMode filterMode) = 0;
	virtual FilterMode GetFilterMode() = 0;
	virtual void SetWrapMode(WrapMode wrapMode) = 0;
	virtual WrapMode GetWrapMode() = 0;
	virtual void VSSetTexture(unsigned int slot) = 0;
	virtual void PSSetTexture(unsigned int slot) = 0;
	virtual void CSSetTexture(unsigned int slot) = 0;
};