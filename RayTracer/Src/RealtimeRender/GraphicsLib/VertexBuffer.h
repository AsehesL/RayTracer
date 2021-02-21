#pragma once

#define VERTEX_ATTRIBUTE_SIZE 12

enum class Topology
{
	PointList,
	LineStrip,
	LineList,
	TriangleStrip,
	TriangleList,
};

class VertexBuffer
{
public:
	virtual ~VertexBuffer() {}
	virtual void SetVertices(float* vertices, unsigned int* indices, int vertexcount, int indexcount) = 0;
	virtual void Draw(Topology topology, int indexCount = 0) = 0;
	virtual bool IsValid() = 0;
};