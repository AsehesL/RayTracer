#include "ComputeShaderBase.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../RealtimeRender/GraphicsLib/ComputeShaderProgram.h"
#include "../Texture/TextureBase.h"

ComputeShaderBase::ComputeShaderBase(GLContext* glContext)
{
	m_glContext = glContext;
	m_computeShaderProgram = glContext->CreateComputeShaderProgram();
}

ComputeShaderBase::~ComputeShaderBase()
{
	delete m_computeShaderProgram;
}

void ComputeShaderBase::Dispatch(int threadGroupX, int threadGroupY, int threadGroupZ)
{
	if (m_computeShaderProgram)
	{
		if (!m_computeShaderProgram->CSSetShader())
		{
			m_computeShaderProgram->CSClearShader();
			return;
		}
		if (!OnApplyParameters())
		{
			m_computeShaderProgram->CSClearShader();
			return;
		}
		m_computeShaderProgram->Dispatch(threadGroupX, threadGroupY, threadGroupZ);
		m_computeShaderProgram->CSClearShader();

		OnGetDatas();
	}
}

void ComputeShaderBase::SetMatrix(const char* key, Matrix4x4 val)
{
	MatrixParameter param = MatrixParameter(Matrix4x4f(val));
	if (m_matrixParams.find(key) != m_matrixParams.end())
		m_matrixParams[key] = param;
	else
		m_matrixParams.insert(std::pair<std::string, MatrixParameter>(key, param));
}

void ComputeShaderBase::SetVector4(const char* key, Vector4 val)
{
	VectorParameter param = VectorParameter(Vector4f(val));
	if (m_vectorParams.find(key) != m_vectorParams.end())
		m_vectorParams[key] = param;
	else
		m_vectorParams.insert(std::pair<std::string, VectorParameter>(key, param));
}

void ComputeShaderBase::SetVector3(const char* key, Vector3 val)
{
	VectorParameter param = VectorParameter(Vector4f(val));
	if (m_vectorParams.find(key) != m_vectorParams.end())
		m_vectorParams[key] = param;
	else
		m_vectorParams.insert(std::pair<std::string, VectorParameter>(key, param));
}

void ComputeShaderBase::SetVector2(const char* key, Vector2 val)
{
	VectorParameter param = VectorParameter(Vector4f(val));
	if (m_vectorParams.find(key) != m_vectorParams.end())
		m_vectorParams[key] = param;
	else
		m_vectorParams.insert(std::pair<std::string, VectorParameter>(key, param));
}

void ComputeShaderBase::SetColor(const char* key, Color val)
{
	VectorParameter param = VectorParameter(Vector4f(val.r, val.g, val.b, val.a));
	if (m_vectorParams.find(key) != m_vectorParams.end())
		m_vectorParams[key] = param;
	else
		m_vectorParams.insert(std::pair<std::string, VectorParameter>(key, param));
}

void ComputeShaderBase::SetFloat(const char* key, float val)
{
	VectorParameter param = VectorParameter(Vector4f(val));
	if (m_vectorParams.find(key) != m_vectorParams.end())
		m_vectorParams[key] = param;
	else
		m_vectorParams.insert(std::pair<std::string, VectorParameter>(key, param));
}

void ComputeShaderBase::SetTexture(const char* key, TextureBase* texture)
{
	if (m_textureParams.find(key) != m_textureParams.end())
		m_textureParams[key] = texture;
	else
		m_textureParams.insert(std::pair<std::string, TextureBase*>(key, texture));
}

Matrix4x4 ComputeShaderBase::GetMatrix(const char* key) const
{
	Matrix4x4f matrixf = GetMatrixf(key);
	return Matrix4x4(matrixf.m00, matrixf.m01, matrixf.m02, matrixf.m03, matrixf.m10, matrixf.m11, matrixf.m12, matrixf.m13,
		matrixf.m20, matrixf.m21, matrixf.m22, matrixf.m23, matrixf.m30, matrixf.m31, matrixf.m32, matrixf.m33);
}

Vector4 ComputeShaderBase::GetVector4(const char* key) const
{
	Vector4f vectorf = GetVectorf(key);
	return Vector4(vectorf.x, vectorf.y, vectorf.z, vectorf.w);
}

Vector3 ComputeShaderBase::GetVector3(const char* key) const
{
	Vector4f vectorf = GetVectorf(key);
	return Vector3(vectorf.x, vectorf.y, vectorf.z);
}

Vector2 ComputeShaderBase::GetVector2(const char* key) const
{
	Vector4f vectorf = GetVectorf(key);
	return Vector2(vectorf.x, vectorf.y);
}

Color ComputeShaderBase::GetColor(const char* key) const
{
	Vector4f vectorf = GetVectorf(key);
	return Color(vectorf.x, vectorf.y, vectorf.z, vectorf.w);
}

float ComputeShaderBase::GetFloat(const char* key) const
{
	Vector4f vectorf = GetVectorf(key);
	return vectorf.x;
}

bool ComputeShaderBase::IsValid()
{
	if (m_computeShaderProgram)
		return m_computeShaderProgram->IsValid();
	return false;
}

Matrix4x4f ComputeShaderBase::GetMatrixf(const char* key) const
{
	if (m_matrixParams.find(key) != m_matrixParams.end())
		return m_matrixParams.at(key).matrix;
	return Matrix4x4f();
}

Vector4f ComputeShaderBase::GetVectorf(const char* key) const
{
	if (m_vectorParams.find(key) != m_vectorParams.end())
		return m_vectorParams.at(key).v;
	return Vector4f();
}

TextureBase* ComputeShaderBase::GetTexture(const char* key) const
{
	if (m_textureParams.find(key) != m_textureParams.end())
		return m_textureParams.at(key);
	return nullptr;
}

bool ComputeShaderBase::IsUniformBufferDirty(const char* key) const
{
	if (m_vectorParams.find(key) != m_vectorParams.end())
		if (m_vectorParams.at(key).isDirty)
			return true;
	if (m_matrixParams.find(key) != m_matrixParams.end())
		if (m_matrixParams.at(key).isDirty)
			return true;
	return false;
}

void ComputeShaderBase::ClearDirtyFlag(const char* key)
{
	if (m_vectorParams.find(key) != m_vectorParams.end())
	{
		VectorParameter& param = m_vectorParams.at(key);
		param.isDirty = false;
	}
	if (m_matrixParams.find(key) != m_matrixParams.end())
	{
		MatrixParameter& param = m_matrixParams.at(key);
		param.isDirty = false;
	}
}
