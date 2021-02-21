#include "Mesh.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../RealtimeRender/GraphicsLib/VertexBuffer.h"
#include "../Common/GlobalResource.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

void ProcessAssimpNode(aiNode* node, const aiScene* scene, int& startID, int& count);

int ProcessAssimpMesh(aiMesh* mesh, const aiScene* scene)
{
	if (mesh == nullptr || mesh->mNumVertices == 0 || mesh->mNumFaces == 0)
		return -1;

	int meshId = -1;
	unsigned int vCount = mesh->mNumVertices;
	unsigned int iCount = mesh->mNumFaces * 3;
	Mesh* createdMesh = GlobalResource::CreateMesh(vCount, iCount, meshId); //only support one face

	for (unsigned int i = 0; i < vCount; ++i)
	{
		Vector3 vertex = Vector3((double)mesh->mVertices[i].x, (double)mesh->mVertices[i].y, (double)mesh->mVertices[i].z);
		createdMesh->SetVertex(vertex, i);
		if (mesh->mNormals)
		{ 
			Vector3 normal = Vector3((double)mesh->mNormals[i].x, (double)mesh->mNormals[i].y, (double)mesh->mNormals[i].z);
			createdMesh->SetNormal(normal, i);
		}
		if (mesh->mTextureCoords[0])
		{
			Vector2 uv0 = Vector2((double)mesh->mTextureCoords[0][i].x, (double)mesh->mTextureCoords[0][i].y);
			createdMesh->SetUV(uv0, 0, i);
		}
		if (mesh->mTangents)
		{
			Vector4 tangent = Vector4((double)mesh->mTangents[i].x, (double)mesh->mTangents[i].y, (double)mesh->mTangents[i].z, -1);
			createdMesh->SetTangent(tangent, i);
		}
	}
	int k = 0;
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; ++j)
		{ 
			unsigned int index = mesh->mFaces[i].mIndices[j];
			createdMesh->SetIndex(index, k);
			k++;
		}
	}
	return meshId;
}

void ProcessAssimpNode(aiNode* node, const aiScene* scene, int& startID, int& count)
{
	for (unsigned i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		int meshID = ProcessAssimpMesh(mesh, scene);
		if (meshID != -1)
		{
			if (startID < 0)
				startID = meshID;
			count += 1;
		}
	}

	for (unsigned i = 0; i < node->mNumChildren; i++)
	{
		ProcessAssimpNode(node->mChildren[i], scene, startID, count);
	}
}

Mesh::Mesh(unsigned int vertexCount, unsigned int indexCount, GLContext* glContext)
{
	m_glContext = glContext;
	m_vertexCount = vertexCount;
	m_indexCount = indexCount;

	m_vertices = new Vector3[vertexCount];
	m_uv0s = new Vector2[vertexCount];
	m_normals = new Vector3[vertexCount];
	m_tangents = new Vector4[vertexCount];
	m_indices = new unsigned int[indexCount];

	m_vertexBuffer = glContext->CreateVertexBuffer(false);

	m_meshHasModified = false;

	m_min = Vector3(DBL_MAX, DBL_MAX, DBL_MAX);
	m_max = Vector3(DBL_MIN, DBL_MIN, DBL_MIN);
}

Mesh::~Mesh()
{
	delete m_vertexBuffer;
	delete[] m_vertices;
	delete[] m_uv0s;
	delete[] m_normals;
	delete[] m_tangents;
	delete[] m_indices;
	m_glContext = nullptr;
}

unsigned int Mesh::GetVertexCount() const
{
	return m_vertexCount;
}

unsigned int Mesh::GetIndexCount() const
{
	return m_indexCount;
}

void Mesh::SetVertex(const Vector3& vertex, const Vector3& normal, const Vector4& tangent, const Vector2& uv0, int i)
{
	if (i < 0 || i >= m_vertexCount)
		return;
	m_vertices[i] = vertex;
	m_normals[i] = normal;
	m_tangents[i] = tangent;
	m_uv0s[i] = uv0;
	m_meshHasModified = true;

	if (vertex.x < m_min.x)
		m_min.x = vertex.x;
	if (vertex.x > m_max.x)
		m_max.x = vertex.x;

	if (vertex.y < m_min.y)
		m_min.y = vertex.y;
	if (vertex.y > m_max.y)
		m_max.y = vertex.y;

	if (vertex.z < m_min.z)
		m_min.z = vertex.z;
	if (vertex.z > m_max.z)
		m_max.z = vertex.z;
}

void Mesh::SetVertex(const Vector3& vertex, int i)
{
	if (i < 0 || i >= m_vertexCount)
		return;
	m_vertices[i] = vertex;
	m_meshHasModified = true;

	if (vertex.x < m_min.x)
		m_min.x = vertex.x;
	if (vertex.x > m_max.x)
		m_max.x = vertex.x;

	if (vertex.y < m_min.y)
		m_min.y = vertex.y;
	if (vertex.y > m_max.y)
		m_max.y = vertex.y;

	if (vertex.z < m_min.z)
		m_min.z = vertex.z;
	if (vertex.z > m_max.z)
		m_max.z = vertex.z;
}

Vector3 Mesh::GetVertex(int i)
{
	if (i >= 0 && i < m_vertexCount)
	{
		return m_vertices[i];
	}
	return Vector3();
}

void Mesh::SetUV(const Vector2& uv, int channel, int i)
{
	if (i < 0 || i >= m_vertexCount)
		return;
	if (channel == 0)
	{
		m_uv0s[i] = uv;
		m_meshHasModified = true;
	}
}

Vector2 Mesh::GetUV(int channel, int i)
{
	if (i >= 0 && i < m_vertexCount)
	{
		if (channel == 0)
			return m_uv0s[i];
	}
	return Vector2();
}

void Mesh::SetNormal(const Vector3& normal, int i)
{
	if (i < 0 || i >= m_vertexCount)
		return;
	m_normals[i] = normal;
	m_meshHasModified = true;
}

Vector3 Mesh::GetNormal(int i)
{
	if (i >= 0 && i < m_vertexCount)
	{
		return m_normals[i];
	}
	return Vector3();
}

void Mesh::SetTangent(const Vector4& tangent, int i)
{
	if (i < 0 || i >= m_vertexCount)
		return;
	m_tangents[i] = tangent;
	m_meshHasModified = true;
}

Vector4 Mesh::GetTangent(int i)
{
	if (i >= 0 && i < m_vertexCount)
	{
		return m_tangents[i];
	}
	return Vector4();
}

void Mesh::GetBounds(Bounds& bounds)
{
	Vector3 size = m_max - m_min;
	Vector3 center = (m_min + m_max) * 0.5f;
	bounds.center = center;
	bounds.halfSize = size * 0.5f;
}

void Mesh::SetIndex(unsigned int index, int i)
{
	if (i < 0 || i >= m_indexCount)
		return;
	m_indices[i] = index;
	m_meshHasModified = true;
}

unsigned int Mesh::GetIndex(int i)
{
	if (i >= 0 && i < m_indexCount)
	{
		return m_indices[i];
	}
	return 0;
}

void Mesh::Commit()
{
	if (m_meshHasModified)
	{
		float* vertices = new float[m_vertexCount * VERTEX_ATTRIBUTE_SIZE];
		for (int i = 0; i < m_vertexCount; i++)
		{
			vertices[i * VERTEX_ATTRIBUTE_SIZE] = m_vertices[i].x;
			vertices[i * VERTEX_ATTRIBUTE_SIZE + 1] = m_vertices[i].y;
			vertices[i * VERTEX_ATTRIBUTE_SIZE + 2] = m_vertices[i].z;

			vertices[i * VERTEX_ATTRIBUTE_SIZE + 3] = m_normals[i].x;
			vertices[i * VERTEX_ATTRIBUTE_SIZE + 4] = m_normals[i].y;
			vertices[i * VERTEX_ATTRIBUTE_SIZE + 5] = m_normals[i].z;

			vertices[i * VERTEX_ATTRIBUTE_SIZE + 6] = m_uv0s[i].x;
			vertices[i * VERTEX_ATTRIBUTE_SIZE + 7] = m_uv0s[i].y;

			vertices[i * VERTEX_ATTRIBUTE_SIZE + 8] = m_tangents[i].x;
			vertices[i * VERTEX_ATTRIBUTE_SIZE + 9] = m_tangents[i].y;
			vertices[i * VERTEX_ATTRIBUTE_SIZE + 10] = m_tangents[i].z;
			vertices[i * VERTEX_ATTRIBUTE_SIZE + 11] = m_tangents[i].w;
		}
		m_vertexBuffer->SetVertices(vertices, m_indices, m_vertexCount, m_indexCount);
		delete[] vertices;
		m_meshHasModified = false;
	}
	if (m_vertexBuffer && m_vertexBuffer->IsValid())
		m_vertexBuffer->Draw(Topology::TriangleList);
}

void Mesh::CreateMeshesFromFile(const char* path, int& startMeshID, int& endMeshID)
{
	startMeshID = -1;
	endMeshID = -1;

	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(path,
		aiProcess_Triangulate |
		aiProcess_CalcTangentSpace);

	if (pScene == NULL)
		return;

	int startID = -1;
	int count = 0;
	ProcessAssimpNode(pScene->mRootNode, pScene, startID, count);

	if (count > 0 && startID != -1)
	{
		startMeshID = startID;
		endMeshID = startID + count - 1;
	}
}
