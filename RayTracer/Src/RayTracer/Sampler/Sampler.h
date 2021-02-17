#pragma once
#include "../../Core/Vector.h"

namespace RayTracer
{
	enum class SamplerType
	{
		Random,
		Jittered,
		Hammersley,
		Regular,
	};

	
	class SamplerBase
	{
	public:
		SamplerBase();
		virtual ~SamplerBase();
		int GetNumSamples() const;
		int GetCurrentSample() const;
		bool NextSample();
		void ResetSampler();
		double GetRandom();
		void SampleHemiSphere(float e, Vector3& out);
		void SampleSphere(Vector3& out);
		void SampleUnitDisk(Vector2& out);
		void Sample(Vector2& out);

		void Init(int numSamples, int numSets = 83);

	protected:
		virtual void InitSampler(int numSamples, int numSets) = 0;

	private:
		void SetupShuffledIndices();

	protected:
		int m_numSamples;
		int m_currentSample;
		int m_numSets;
		int m_index;
		int m_jump;
		int* m_shuffledIndices;
		Vector2* m_samples;
	};

	class RandomSampler : public SamplerBase
	{
	public:
		RandomSampler();
		virtual ~RandomSampler() override;
	protected:
		virtual void InitSampler(int numSamples, int numSets) override;
	};

	class JitteredSampler : public SamplerBase
	{
	public:
		JitteredSampler();
		virtual ~JitteredSampler() override;
	protected:
		virtual void InitSampler(int numSamples, int numSets) override;
	};

	class HammersleySampler : public SamplerBase
	{
	public:
		HammersleySampler();
		virtual ~HammersleySampler() override;
	protected:
		virtual void InitSampler(int numSamples, int numSets) override;

	private:
		float Phi(int j);
	};

	class RegularSampler : public SamplerBase
	{
	public:
		RegularSampler();
		virtual ~RegularSampler() override;
	protected:
		virtual void InitSampler(int numSamples, int numSets) override;
	};


	SamplerBase* CreateSampler(SamplerType samplerType, int numSamples, int numSets = 83);
}