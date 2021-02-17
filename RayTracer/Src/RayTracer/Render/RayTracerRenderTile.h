#pragma once

#define TILE_SIZE 16

namespace RayTracer
{
	struct RenderTile
	{
	public:
		int x;
		int y;
		int width;
		int height;
	};
}