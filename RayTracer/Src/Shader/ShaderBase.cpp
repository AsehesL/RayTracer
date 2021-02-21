#include "ShaderBase.h"
#include "../RealtimeRender/GraphicsLib/GLContext.h"
#include "../RealtimeRender/GraphicsLib/ShaderProgram.h"
#include "../Texture/TextureBase.h"

ShaderBase::ShaderBase(GLContext* glContext)
{
	m_glContext = glContext;
	m_shaderProgram = glContext->CreateShaderProgram();
}

ShaderBase::~ShaderBase()
{
	delete m_shaderProgram;
}

bool ShaderBase::Execute()
{
	if (!IsValid())
		return false;
	if (!OnApplyParameters())
		return false;

	OnApplyRenderStates();

	m_glContext->ApplyRenderStates();
	return m_shaderProgram->Execute();
}

void ShaderBase::SetMatrix(const char* key, Matrix4x4 val)
{
	MatrixParameter param = MatrixParameter(Matrix4x4f(val));
	if (m_matrixParams.find(key) != m_matrixParams.end())
		m_matrixParams[key] = param;
	else
		m_matrixParams.insert(std::pair<std::string, MatrixParameter>(key, param));
}

void ShaderBase::SetVector4(const char* key, Vector4 val)
{
	VectorParameter param = VectorParameter(Vector4f(val));
	if (m_vectorParams.find(key) != m_vectorParams.end())
		m_vectorParams[key] = param;
	else
		m_vectorParams.insert(std::pair<std::string, VectorParameter>(key, param));
}

void ShaderBase::SetVector3(const char* key, Vector3 val)
{
	VectorParameter param = VectorParameter(Vector4f(val));
	if (m_vectorParams.find(key) != m_vectorParams.end())
		m_vectorParams[key] = param;
	else
		m_vectorParams.insert(std::pair<std::string, VectorParameter>(key, param));
}

void ShaderBase::SetVector2(const char* key, Vector2 val)
{
	VectorParameter param = VectorParameter(Vector4f(val));
	if (m_vectorParams.find(key) != m_vectorParams.end())
		m_vectorParams[key] = param;
	else
		m_vectorParams.insert(std::pair<std::string, VectorParameter>(key, param));
}

void ShaderBase::SetColor(const char* key, Color val)
{
	VectorParameter param = VectorParameter(Vector4f(val.r, val.g, val.b, val.a));
	if (m_vectorParams.find(key) != m_vectorParams.end())
		m_vectorParams[key] = param;
	else
		m_vectorParams.insert(std::pair<std::string, VectorParameter>(key, param));
}

void ShaderBase::SetFloat(const char* key, float val)
{
	VectorParameter param = VectorParameter(Vector4f(val));
	if (m_vectorParams.find(key) != m_vectorParams.end())
		m_vectorParams[key] = param;
	else
		m_vectorParams.insert(std::pair<std::string, VectorParameter>(key, param));
}

void ShaderBase::SetTexture(const char* key, TextureBase* texture)
{
	if (m_textureParams.find(key) != m_textureParams.end())
		m_textureParams[key] = texture;
	else
		m_textureParams.insert(std::pair<std::string, TextureBase*>(key, texture));
}

Matrix4x4 ShaderBase::GetMatrix(const char* key) const
{
	Matrix4x4f matrixf = GetMatrixf(key);
	return Matrix4x4(matrixf.m00, matrixf.m01, matrixf.m02, matrixf.m03, matrixf.m10, matrixf.m11, matrixf.m12, matrixf.m13,
		matrixf.m20, matrixf.m21, matrixf.m22, matrixf.m23, matrixf.m30, matrixf.m31, matrixf.m32, matrixf.m33);
}

Vector4 ShaderBase::GetVector4(const char* key) const
{
	Vector4f vectorf = GetVectorf(key);
	return Vector4(vectorf.x, vectorf.y, vectorf.z, vectorf.w);
}

Vector3 ShaderBase::GetVector3(const char* key) const
{
	Vector4f vectorf = GetVectorf(key);
	return Vector3(vectorf.x, vectorf.y, vectorf.z);
}

Vector2 ShaderBase::GetVector2(const char* key) const
{
	Vector4f vectorf = GetVectorf(key);
	return Vector2(vectorf.x, vectorf.y);
}

Color ShaderBase::GetColor(const char* key) const
{
	Vector4f vectorf = GetVectorf(key);
	return Color(vectorf.x, vectorf.y, vectorf.z, vectorf.w);
}

float ShaderBase::GetFloat(const char* key) const
{
	Vector4f vectorf = GetVectorf(key);
	return vectorf.x;
}

bool ShaderBase::IsValid()
{
	if (m_shaderProgram)
		return m_shaderProgram->IsValid();
	return false;
}

Matrix4x4f ShaderBase::GetMatrixf(const char* key) const
{
	if (m_matrixParams.find(key) != m_matrixParams.end())
		return m_matrixParams.at(key).matrix;
	return Matrix4x4f();
}

Vector4f ShaderBase::GetVectorf(const char* key) const
{
	if (m_vectorParams.find(key) != m_vectorParams.end())
		return m_vectorParams.at(key).v;
	return Vector4f();
}

TextureBase* ShaderBase::GetTexture(const char* key) const
{
	if (m_textureParams.find(key) != m_textureParams.end())
		return m_textureParams.at(key);
	return nullptr;
}

bool ShaderBase::IsUniformBufferDirty(const char* key) const
{
	if (m_vectorParams.find(key) != m_vectorParams.end())
		if (m_vectorParams.at(key).isDirty) 
			return true;
	if (m_matrixParams.find(key) != m_matrixParams.end())
		if (m_matrixParams.at(key).isDirty)
			return true;
	return false;
}

void ShaderBase::ClearDirtyFlag(const char* key)
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

void ShaderBase::OnApplyRenderStates()
{
	m_glContext->ztestFunc = CompareFunc::LEqual;
}
