#include "GLContext.h"

GLContext::GLContext()
{
}

GLContext::~GLContext()
{
}

bool GLContext::Init(HWND& hwnd, unsigned int screenWidth, unsigned int screenHeight)
{
	if (!InitGL(hwnd, screenWidth, screenHeight))
		return false;
	InitStates();
	return true;
}

void GLContext::ApplyRenderStates()
{
	ApplyRenderStates(false);
}

void GLContext::InitStates()
{
	cullEnable = true;
	cullMode = CullMode::Back;
	fillMode = FillMode::Solid;
	ztestFunc = CompareFunc::LEqual;
	zwrite = true;
	blendEnable = false;
	blendSrc = BlendFactor::One;
	blendDst = BlendFactor::Zero;
	blendOp = BlendOp::Add;
	stencilEnable = false;
	stencilId = 0;
	stencilTestFunc = CompareFunc::Always;
	stencilReadMask = 0xff;
	stencilWriteMask = 0xff;
	stencilPassOp = StencilOp::Keep;
	stencilFailOp = StencilOp::Keep;
	stencilZfailOp = StencilOp::Keep;
	ApplyRenderStates(true);
}