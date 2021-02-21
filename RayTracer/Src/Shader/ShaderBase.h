#pragma once
#include "../Core/Vector.h"
#include "../Core/Color.h"
#include "../Core/Matrix.h"

#include <map>
#include <string>

class ShaderBase
{
	struct MatrixParameter
	{
	public:
		MatrixParameter() : matrix(), isDirty(true) {}
		MatrixParameter(const Matrix4x4f& matrix)
		{
			this->matrix = matrix;
			this->isDirty = true;
		}
		Matrix4x4f matrix;
		bool isDirty = true;
	};

	struct VectorParameter
	{
	public:
		VectorParameter() : v(), isDirty(true) {}
		VectorParameter(const Vector4f& vector)
		{
			this->v = vector;
			this->isDirty = true;
		}
		Vector4f v;
		bool isDirty;
	};

public:
	ShaderBase(class GLContext* glContext);
	virtual ~ShaderBase();

	bool Execute();

	void SetMatrix(const char* key, Matrix4x4 val);
	void SetVector4(const char* key, Vector4 val);
	void SetVector3(const char* key, Vector3 val);
	void SetVector2(const char* key, Vector2 val);
	void SetColor(const char* key, Color val);
	void SetFloat(const char* key, float val);
	void SetTexture(const char* key, class TextureBase* texture);

	Matrix4x4 GetMatrix(const char* key) const;
	Vector4 GetVector4(const char* key) const;
	Vector3 GetVector3(const char* key) const;
	Vector2 GetVector2(const char* key) const;
	Color GetColor(const char* key) const;
	float GetFloat(const char* key) const;

	bool IsValid();

protected:
	Matrix4x4f GetMatrixf(const char* key) const;
	Vector4f GetVectorf(const char* key) const;
	class TextureBase* GetTexture(const char* key) const;

	bool IsUniformBufferDirty(const char* key) const;
	void ClearDirtyFlag(const char* key);
	virtual bool OnApplyParameters() = 0;
	virtual void OnApplyRenderStates();

protected:
	class GLContext* m_glContext;
	class ShaderProgram* m_shaderProgram;

private:
	

	std::map<std::string, MatrixParameter> m_matrixParams;
	std::map<std::string, VectorParameter> m_vectorParams;
	std::map<std::string, class TextureBase*> m_textureParams;
};