#pragma once
#include "../Core/Vector.h"
#include "../Core/Bounds.h"

class Mesh
{
public:
	Mesh(unsigned int vertexCount, unsigned int indexCount, class GLContext* glContext);
	~Mesh();
	unsigned int GetVertexCount() const;
	unsigned int GetIndexCount() const;
	void SetVertex(const Vector3& vertex, const Vector3& normal, const Vector4& tangent, const Vector2& uv0, int i);
	void SetVertex(const Vector3& vertex, int i);
	Vector3 GetVertex(int i);
	void SetUV(const Vector2& uv, int channel, int i);
	Vector2 GetUV(int channel, int i);
	void SetNormal(const Vector3& normal, int i);
	Vector3 GetNormal(int i);
	void SetTangent(const Vector4& tangent, int i);
	Vector4 GetTangent(int i);
	void GetBounds(Bounds& bounds);

	void SetIndex(unsigned int index, int i);
	unsigned int GetIndex(int i);

	void Commit();

	static void CreateMeshesFromFile(const char* path, int& startMeshID, int& endMeshID);

private:
	Vector3* m_vertices;
	Vector2* m_uv0s;
	Vector3* m_normals;
	Vector4* m_tangents;
	Vector3 m_min;
	Vector3 m_max;
	unsigned int m_vertexCount;
	unsigned int m_indexCount;
	unsigned int* m_indices;
	class VertexBuffer* m_vertexBuffer;
	class GLContext* m_glContext;
	bool m_meshHasModified;
};