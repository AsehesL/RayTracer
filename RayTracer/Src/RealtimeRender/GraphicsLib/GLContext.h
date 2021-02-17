#pragma once
#include "../../../framework.h"
#include "GLDefines.h"

class GLContext
{
public:
	GLContext();
	virtual ~GLContext();

	bool Init(HWND& hwnd, unsigned int screenWidth, unsigned int screenHeight);

	virtual void Present() = 0;

	virtual void ClearColor(float r, float g, float b, float a) = 0;

	virtual void Clear(int clearFlags) = 0;

	virtual void SetViewport(float x, float y, float width, float height) = 0;

	virtual class IVertexBuffer* CreateVertexBuffer(bool isDynamic) = 0;

	virtual class IShaderProgram* CreateShaderProgram() = 0;

	virtual class IShaderUniformBuffer* CreateShaderUniformBuffer(unsigned int bufferSize) = 0;

	virtual class ITextureBuffer* CreateTextureBuffer() = 0;

	virtual void CreateRenderBuffer(unsigned int width, unsigned int height, bool isShadowMap, class IColorBuffer** outColorBuffer, class IDepthBuffer** outDepthBuffer, class ITextureBuffer** outTextureBuffer) = 0;
	
	virtual void CreateCubeRenderBuffer(unsigned int size, class ICubeMapColorBuffer** outCubeMapColorBuffer, class IDepthBuffer** outDepthBuffer, class ITextureBuffer** outTextureBuffer) = 0;

	virtual void SetRenderBuffer(class IColorBuffer* colorBuffer, class IDepthBuffer* depthBuffer) = 0;

	virtual void SetCubeRenderBuffer(class ICubeMapColorBuffer* cubeMapColorBuffer, class IDepthBuffer* depthBuffer, int face) = 0;

	virtual void GenerateMipmap(class ITextureBuffer* textureBuffer) = 0;

	void ApplyRenderStates();

protected:
	virtual void ApplyRenderStates(bool forceApply) = 0;

	virtual bool InitGL(HWND& hwnd, unsigned int screenWidth, unsigned screenHeight) = 0;

	void InitStates();

public:
	bool cullEnable;
	CullMode cullMode;
	FillMode fillMode;
	bool zwrite;
	CompareFunc ztestFunc;
	bool blendEnable;
	BlendFactor blendSrc, blendDst;
	BlendOp blendOp;
	bool stencilEnable;
	unsigned short stencilId;
	unsigned short stencilReadMask;
	unsigned short stencilWriteMask;
	CompareFunc stencilTestFunc;
	StencilOp stencilPassOp;
	StencilOp stencilZfailOp;
	StencilOp stencilFailOp;
};

