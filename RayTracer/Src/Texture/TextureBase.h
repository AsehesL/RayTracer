#pragma once
#include "../RealtimeRender/GraphicsLib/GLDefines.h"

class TextureBase
{
public:
	TextureBase();
	virtual ~TextureBase();
	void SetFilterMode(FilterMode filterMode);
	FilterMode GetFilterMode();
	void SetWrapMode(WrapMode wrapMode);
	WrapMode GetWrapMode();
	void VSSetTexture(unsigned int slot);
	void PSSetTexture(unsigned int slot);
	void CSSetTexture(unsigned int slot);

protected:
	class TextureBuffer* m_textureBuffer;
};