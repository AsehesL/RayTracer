#pragma once
#include "GLDefines.h"
#include "ShaderResource.h"

class ITextureBuffer : public IShaderResource
{
public:
	virtual ~ITextureBuffer() {}
	virtual void Set2DBuffer(unsigned int width, unsigned int height, const void* data) = 0;
	virtual void SetCubeBuffer(ITextureBuffer* left, ITextureBuffer* right, ITextureBuffer* top, ITextureBuffer* bottom, ITextureBuffer* front, ITextureBuffer* back) = 0;
	virtual void SetFilterMode(FilterMode filterMode) = 0;
	virtual FilterMode GetFilterMode() = 0;
	virtual void SetWrapMode(WrapMode wrapMode) = 0;
	virtual WrapMode GetWrapMode() = 0;
};