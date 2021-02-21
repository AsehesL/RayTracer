#pragma once
#include "../Core/Vector.h"
#include "ComputeShaderBase.h"

class SkyLightToSHComputeShader : public ComputeShaderBase
{
public:
	SkyLightToSHComputeShader(class GLContext* glContext);
	virtual ~SkyLightToSHComputeShader() override;

public:
	void GetSkyLightSH0(Vector4& sh0R, Vector4& sh0G, Vector4& sh0B) const
	{
		sh0R = m_skyLightSH0R;
		sh0G = m_skyLightSH0G;
		sh0B = m_skyLightSH0B;
	}
	void GetSkyLightSH1(Vector4& sh1R, Vector4& sh1G, Vector4& sh1B) const
	{
		sh1R = m_skyLightSH1R;
		sh1G = m_skyLightSH1G;
		sh1B = m_skyLightSH1B;
	}
	void GetSkyLightSH2(Vector3& sh2) const
	{
		sh2 = m_skyLightSH2;
	}
	void GetSkyLightSH3(Vector3& sh3) const
	{
		sh3 = m_skyLightSH3;
	}

protected:
	virtual bool OnApplyParameters() override;
	virtual void OnGetDatas() override;

private:
	class ComputeBuffer* m_computeBuffer;

	Vector4 m_skyLightSH0R;
	Vector4 m_skyLightSH0G;
	Vector4 m_skyLightSH0B;
	Vector4 m_skyLightSH1R;
	Vector4 m_skyLightSH1G;
	Vector4 m_skyLightSH1B;
	Vector3 m_skyLightSH2;
	Vector3 m_skyLightSH3;
};