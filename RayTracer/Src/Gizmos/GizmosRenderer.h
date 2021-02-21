#pragma once
#include "../Core/Matrix.h"
#include "../Core/Color.h"
#include "../Core/Bounds.h"

class GizmosRenderer
{
public:
	GizmosRenderer(class GLContext* glContext);
	~GizmosRenderer();

	void SetWorldToViewMatrix(const Matrix4x4& worldToViewMatrix);
	void SetProjectionMatrix(const Matrix4x4& projectionMatrix);

	void SetColor(const Color& color)
	{
		m_color = color;
	}

	void DrawWorldSpaceLine(const Vector3& begin, const Vector3& end);
	void DrawBounds(const Bounds& bounds);
	void Clear();

	void Render();

private:
	float* m_vertices;
	Color m_color;
	unsigned int* m_indices;
	int m_indexCount;
	int m_vertexCount;
	int m_vertexBufferLength;
	int m_indexBufferLength;
	//Matrix4x4 m_worldToViewMatrix;
	//Matrix4x4 m_projectionMatrix;
	class GLContext* m_glContext;
	class VertexBuffer* m_vertexBuffer;
	bool m_isBufferDirty;
	class GizmosShader* m_gizmosShader;
};