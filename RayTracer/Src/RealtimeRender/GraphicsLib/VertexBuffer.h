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

class IVertexBuffer
{
public:
	virtual ~IVertexBuffer() {}
	virtual void SetVertices(float* vertices, unsigned int* indices, int vertexcount, int indexcount) = 0;
	virtual void Bind(Topology topology, int indexCount = 0) = 0;
	virtual bool IsValid() = 0;
};