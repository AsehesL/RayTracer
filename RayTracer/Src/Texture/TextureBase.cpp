#include "TextureBase.h"
#include "../RealtimeRender/GraphicsLib/TextureBuffer.h"

TextureBase::TextureBase()
{
	m_textureBuffer = nullptr;
}

TextureBase::~TextureBase()
{
	if (m_textureBuffer)
		delete m_textureBuffer;
	m_textureBuffer = nullptr;
}

void TextureBase::SetFilterMode(FilterMode filterMode)
{
	if (m_textureBuffer)
		m_textureBuffer->SetFilterMode(filterMode);
}

FilterMode TextureBase::GetFilterMode()
{
	if (m_textureBuffer)
		return m_textureBuffer->GetFilterMode();
	return FilterMode();
}

void TextureBase::SetWrapMode(WrapMode wrapMode)
{
	if (m_textureBuffer)
		m_textureBuffer->SetWrapMode(wrapMode);
}

WrapMode TextureBase::GetWrapMode()
{
	if (m_textureBuffer)
		return m_textureBuffer->GetWrapMode();
	return WrapMode();
}

void TextureBase::VSSetTexture(unsigned int slot)
{
	if (m_textureBuffer)
		m_textureBuffer->VSSetTexture(slot);
}

void TextureBase::PSSetTexture(unsigned int slot)
{
	if (m_textureBuffer)
		m_textureBuffer->PSSetTexture(slot);
}

void TextureBase::CSSetTexture(unsigned int slot)
{
	if (m_textureBuffer)
		m_textureBuffer->CSSetTexture(slot);
}
