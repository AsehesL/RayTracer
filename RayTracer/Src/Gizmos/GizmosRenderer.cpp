#include "GizmosRenderer.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../RealtimeRender/GraphicsLib/VertexBuffer.h"
#include "../Shader/GizmosShader.h"
#include "../Common/GlobalResource.h"

GizmosRenderer::GizmosRenderer(GLContext* glContext)
{
	m_glContext = glContext;
	m_vertexBuffer = m_glContext->CreateVertexBuffer(true);
	m_gizmosShader = GlobalResource::CreateShader<GizmosShader>();

	m_vertexBufferLength = 100;
	m_indexBufferLength = 100;

	m_vertices = new float[m_vertexBufferLength * VERTEX_ATTRIBUTE_SIZE];
	m_indices = new unsigned int[m_indexBufferLength];
	m_indexCount = 0;
	m_vertexCount = 0;
	
	m_isBufferDirty = false;
}

GizmosRenderer::~GizmosRenderer()
{
	delete[] m_vertices;
	delete[] m_indices;
	delete m_vertexBuffer;
}

void GizmosRenderer::SetWorldToViewMatrix(const Matrix4x4& worldToViewMatrix)
{
	if (m_gizmosShader)
		m_gizmosShader->SetWorldToViewMatrix(worldToViewMatrix);
	//m_worldToViewMatrix = worldToViewMatrix;
}

void GizmosRenderer::SetProjectionMatrix(const Matrix4x4& projectionMatrix)
{
	if (m_gizmosShader)
		m_gizmosShader->SetProjectionMatrix(projectionMatrix);
	//m_projectionMatrix = projectionMatrix;
}

void GizmosRenderer::DrawWorldSpaceLine(const Vector3& begin, const Vector3& end)
{	
	/*Vector3 beginViewPos = m_worldToViewMatrix.TransformPoint(begin);
	Vector3 beginProjPos = m_projectionMatrix.TransformPoint(beginViewPos);

	Vector3 endViewPos = m_worldToViewMatrix.TransformPoint(end);
	Vector3 endProjPos = m_projectionMatrix.TransformPoint(endViewPos);*/

	if (m_vertexCount >= m_vertexBufferLength)
	{
		int newBufferSize = m_vertexBufferLength + 100;
		//delete[] m_vertices;
		//m_vertices = nullptr;
		float* tmpVertices = new float[newBufferSize * VERTEX_ATTRIBUTE_SIZE];
		for (int i=0;i<m_vertexBufferLength;i++)
			tmpVertices[i] = m_vertices[i];
		delete[] m_vertices;
		m_vertices = nullptr;
		m_vertices = tmpVertices;
		m_vertexBufferLength = newBufferSize;
	}
	if (m_indexCount >= m_indexBufferLength)
	{
		int newBufferSize = m_indexBufferLength + 100;
		//delete[] m_indices;
		//m_indices = nullptr;
		unsigned int* tmpIndices = new unsigned int[newBufferSize];
		for (int i = 0; i < m_indexBufferLength; i++)
			tmpIndices[i] = m_indices[i];
		delete[] m_indices;
		m_indices = nullptr;
		m_indices = tmpIndices;
		m_indexBufferLength = newBufferSize;
	}
	m_isBufferDirty = true;

	m_vertices[m_vertexCount * VERTEX_ATTRIBUTE_SIZE] = begin.x;
	m_vertices[m_vertexCount * VERTEX_ATTRIBUTE_SIZE + 1] = begin.y;
	m_vertices[m_vertexCount * VERTEX_ATTRIBUTE_SIZE + 2] = begin.z;

	m_vertices[m_vertexCount * VERTEX_ATTRIBUTE_SIZE + 8] = m_color.r;
	m_vertices[m_vertexCount * VERTEX_ATTRIBUTE_SIZE + 9] = m_color.g;
	m_vertices[m_vertexCount * VERTEX_ATTRIBUTE_SIZE + 10] = m_color.b;
	m_vertices[m_vertexCount * VERTEX_ATTRIBUTE_SIZE + 11] = m_color.a;

	m_vertices[(m_vertexCount + 1) * VERTEX_ATTRIBUTE_SIZE] = end.x;
	m_vertices[(m_vertexCount + 1) * VERTEX_ATTRIBUTE_SIZE + 1] = end.y;
	m_vertices[(m_vertexCount + 1) * VERTEX_ATTRIBUTE_SIZE + 2] = end.z;

	m_vertices[(m_vertexCount + 1) * VERTEX_ATTRIBUTE_SIZE + 8] = m_color.r;
	m_vertices[(m_vertexCount + 1) * VERTEX_ATTRIBUTE_SIZE + 9] = m_color.g;
	m_vertices[(m_vertexCount + 1) * VERTEX_ATTRIBUTE_SIZE + 10] = m_color.b;
	m_vertices[(m_vertexCount + 1) * VERTEX_ATTRIBUTE_SIZE + 11] = m_color.a;

	m_indices[m_indexCount] = m_vertexCount;
	m_indices[m_indexCount + 1] = m_vertexCount + 1;

	m_vertexCount += 2;
	m_indexCount += 2;
}

void GizmosRenderer::DrawBounds(const Bounds& bounds)
{
	Vector3 min = bounds.GetMin();
	Vector3 max = bounds.GetMax();

	DrawWorldSpaceLine(Vector3(min.x, min.y, min.z), Vector3(max.x, min.y, min.z));
	DrawWorldSpaceLine(Vector3(max.x, min.y, min.z), Vector3(max.x, min.y, max.z));
	DrawWorldSpaceLine(Vector3(max.x, min.y, max.z), Vector3(min.x, min.y, max.z));
	DrawWorldSpaceLine(Vector3(min.x, min.y, max.z), Vector3(min.x, min.y, min.z));

	DrawWorldSpaceLine(Vector3(min.x, max.y, min.z), Vector3(max.x, max.y, min.z));
	DrawWorldSpaceLine(Vector3(max.x, max.y, min.z), Vector3(max.x, max.y, max.z));
	DrawWorldSpaceLine(Vector3(max.x, max.y, max.z), Vector3(min.x, max.y, max.z));
	DrawWorldSpaceLine(Vector3(min.x, max.y, max.z), Vector3(min.x, max.y, min.z));

	DrawWorldSpaceLine(Vector3(min.x, min.y, min.z), Vector3(min.x, max.y, min.z));
	DrawWorldSpaceLine(Vector3(max.x, min.y, min.z), Vector3(max.x, max.y, min.z));
	DrawWorldSpaceLine(Vector3(max.x, min.y, max.z), Vector3(max.x, max.y, max.z));
	DrawWorldSpaceLine(Vector3(min.x, min.y, max.z), Vector3(min.x, max.y, max.z));
}

void GizmosRenderer::Clear()
{
	m_vertexCount = 0;
	m_indexCount = 0;
	m_isBufferDirty = true;
}

void GizmosRenderer::Render()
{
	if (m_indexCount <= 0)
		return;

	if (m_gizmosShader != nullptr && m_gizmosShader->Execute())
	{ 
		Matrix4x4 identity;
		Matrix4x4::Identity(&identity);
		m_gizmosShader->SetLocalToWorldMatrix(identity);
		if (m_isBufferDirty)
		{
			m_vertexBuffer->SetVertices(m_vertices, m_indices, m_vertexBufferLength, m_indexBufferLength);
			m_isBufferDirty = false;
		}
		if (m_vertexBuffer && m_vertexBuffer->IsValid())
			m_vertexBuffer->Bind(Topology::LineList, m_indexCount);
	}
}
