#pragma once

enum class ClearFlags
{
	Depth = 1,
	Stencil = 2,
	Color = 4,
};

enum class CullMode
{
	Back = 0,
	Front = 1,
	Off = 2,
};

enum class FillMode
{
	WireFrame = 0,
	Solid = 1,
};

enum class BlendFactor
{
	One = 0,
	Zero = 1,
	SrcColor = 2,
	SrcAlpha = 3,
	DstColor = 4,
	DstAlpha = 5,
	OneMinusSrcColor = 6,
	OneMinusSrcAlpha = 7,
	OneMinusDstColor = 8,
	OneMinusDstAlpha = 9,
};

enum class BlendOp
{
	Add = 0,
	Sub = 1,
	RevSub = 2,
	Min = 3,
	Max = 4,
};

enum class CompareFunc
{
	Never = 0,
	Less = 1,
	Equal = 2,
	LEqual = 3,
	Greater = 4,
	NotEqual = 5,
	GEqual = 6,
	Always = 7,
	Disable = 8,
};

enum class StencilOp
{
	Keep = 0,
	Zero = 1,
	Replace = 2,
	IncrementSaturate = 3,
	DecrementSaturate = 4,
	Invert = 5,
	IncrementWrap = 6,
	DecrementWrap = 7,
};

enum class WrapMode
{
	Repeat = 0,
	Clamp = 1,
};

enum class FilterMode
{
	Point = 0,
	Bilinear = 1,
	Trilinear = 2,
};