#include "Sampler.h"
#include "../../Core/Math.h"

#include <random>

thread_local std::mt19937_64 random_sampler(std::random_device{}());

double RandomUnit()
{
	static thread_local std::uniform_real_distribution<double> unit_distribution(0.0, std::nextafter(1.0, 0.0));
	return unit_distribution(random_sampler);
}

RayTracer::SamplerBase::SamplerBase()
{
}

RayTracer::SamplerBase::~SamplerBase()
{
	delete[] m_shuffledIndices;
	delete[] m_samples;
}

int RayTracer::SamplerBase::GetNumSamples() const
{
	return m_numSamples;
}

int RayTracer::SamplerBase::GetCurrentSample() const
{
	return m_currentSample;
}

bool RayTracer::SamplerBase::NextSample()
{
	if (m_currentSample < m_numSamples)
	{
		m_currentSample++;
		return true;
	}
	return false;
}

void RayTracer::SamplerBase::ResetSampler()
{
	m_currentSample = 0;
}

double RayTracer::SamplerBase::GetRandom()
{
	return RandomUnit();
}

void RayTracer::SamplerBase::SampleHemiSphere(float e, Vector3& out)
{
	Vector2 sample;
	Sample(sample);
	
	float cos_phi = cos(2.0f * Math::PI * sample.x);
	float sin_phi = sin(2.0f * Math::PI * sample.x);
	float cos_theta = pow(1.0f - sample.y, 1.0f / (e + 1.0f));
	float sin_theta = sqrt(1.0f - cos_theta * cos_theta);
	float pu = sin_theta * cos_phi;
	float pv = sin_theta * sin_phi;
	float pw = cos_theta;
	
	out = Vector3(pu, pv, pw);
}

void RayTracer::SamplerBase::SampleSphere(Vector3& out)
{
	Vector2 sample;
	Sample(sample);

	float x = cos(2.0f * Math::PI * sample.x) * 2.0f * sqrt(sample.y * (1 - sample.y));
	float y = sin(2.0f * Math::PI * sample.x) * 2.0f * sqrt(sample.y * (1 - sample.y));
	float z = 1.0f - 2.0f * sample.y;

	out = Vector3(x, y, z);
}

void RayTracer::SamplerBase::SampleUnitDisk(Vector2& out)
{
	Vector2 sample;
	Sample(sample);

	sample.x = 2.0f * sample.x - 1.0f;
	sample.y = 2.0f * sample.y - 1.0f;
	double r = 0.0;
	double phi = 0.0;
	if (sample.x > -sample.y)
	{
		if (sample.x > sample.y)
		{
			r = sample.x;
			phi = sample.y / sample.x;
		}
		else
		{
			r = sample.y;
			phi = 2.0f - sample.x / sample.y;
		}
	}
	else
	{
		if (sample.x < sample.y)
		{
			r = -sample.x;
			phi = 4.0f + sample.y / sample.x;
		}
		else
		{
			r = -sample.y;
			if (sample.y < -FLT_EPSILON || sample.y > FLT_EPSILON)
			{
				phi = 6.0f - sample.x / sample.y;
			}
			else
			{
				phi = 0.0f;
			}
		}
	}
	phi *= Math::PI * 0.25f;
	out = Vector2(r * cos(phi), r * sin(phi));
}

void RayTracer::SamplerBase::Sample(Vector2& out)
{
	if (m_numSamples == 1)
	{
		out = Vector2(0.5f, 0.5f);
		return;
	}
	if ((int)(m_index % m_numSamples) == 0)
	{
		m_jump = int(GetRandom() * m_numSets) * m_numSamples;
	}

	out = m_samples[m_jump + m_shuffledIndices[m_jump + m_index % m_numSamples]];
	m_index += 1;
}

void RayTracer::SamplerBase::Init(int numSamples, int numSets)
{
	InitSampler(numSamples, numSets);

	m_shuffledIndices = new int[m_numSets * m_numSamples];

	SetupShuffledIndices();
}

void RayTracer::SamplerBase::SetupShuffledIndices()
{
    int* indices = new int[m_numSamples]();
    for (int i = 0; i < m_numSamples; i++)
        indices[i] = i;

    m_shuffledIndices = new int[m_numSamples * m_numSets];
    for (int i = 0; i < m_numSets; i++)
    {
        
        int n = m_numSamples;
        while (n > 1)
        {
            n--;
            int k = int(GetRandom() * (n + 1));
            int value = indices[k];
            indices[k] = indices[n];
            indices[n] = value;
        }

        for (int j = 0; j < m_numSamples; j++)
        {
            m_shuffledIndices[i * m_numSamples + j] = indices[j];
        }
    }

    delete[] indices;
}

RayTracer::RandomSampler::RandomSampler() : SamplerBase()
{
}

RayTracer::RandomSampler::~RandomSampler()
{
}

void RayTracer::RandomSampler::InitSampler(int numSamples, int numSets)
{
	m_numSamples = numSamples;
	m_numSets = numSets;
	m_samples = new Vector2[m_numSets * m_numSamples];

	for (int i = 0; i < numSets; i++)
	{
		for (int j = 0; j < numSamples; j++)
		{
			m_samples[i * numSamples + j] = Vector2(GetRandom(), GetRandom());
		}
	}
}

RayTracer::JitteredSampler::JitteredSampler() : SamplerBase()
{
}

RayTracer::JitteredSampler::~JitteredSampler()
{
}

void RayTracer::JitteredSampler::InitSampler(int numSamples, int numSets)
{
	int n = (int)sqrt(numSamples);
	m_numSamples = n * n;
	m_numSets = numSets;
	m_samples = new Vector2[m_numSets * m_numSamples];
	int index = 0;
	for (int i = 0; i < numSets; i++)
	{
		for (int j = 0; j < n; j++)
		{
			for (int k = 0; k < n; k++)
			{
				Vector2 sp = Vector2((k + GetRandom()) / n, (j + GetRandom()) / n);
				m_samples[index] = sp;
				index += 1;
			}
		}
	}
}

RayTracer::HammersleySampler::HammersleySampler() : SamplerBase()
{
}

RayTracer::HammersleySampler::~HammersleySampler()
{
}

void RayTracer::HammersleySampler::InitSampler(int numSamples, int numSets)
{
	m_numSamples = numSamples;
	m_numSets = numSets;
	m_samples = new Vector2[m_numSets * m_numSamples];
	for (int i = 0; i < numSets; i++)
	{
		for (int j = 0; j < numSamples; j++)
		{
			m_samples[i * numSamples + j] = Vector2(((float)j) / numSamples, Phi(j));
		}
	}
}

float RayTracer::HammersleySampler::Phi(int j)
{
	float x = 0.0f;
    float f = 0.5f;
    while (((int)j) > 0)
    {
        x += f * (j % 2);
        j = j / 2;
        f *= 0.5f;
    }

    return x;
}

RayTracer::RegularSampler::RegularSampler() : SamplerBase()
{
}

RayTracer::RegularSampler::~RegularSampler()
{
}

void RayTracer::RegularSampler::InitSampler(int numSamples, int numSets)
{
	int n = (int)sqrt(numSamples);
	m_numSamples = n * n;
	m_numSets = numSets;
	m_samples = new Vector2[m_numSets * m_numSamples];
	int index = 0;
	for (int i = 0; i < numSets; i++)
	{
		for (int j = 0; j < n; j++)
		{
			for (int k = 0; k < n; k++)
			{
				m_samples[index] = Vector2((0.5f + k) / n, (0.5f + j) / n);
				index += 1;
			}
		}
	}
}

RayTracer::SamplerBase* RayTracer::CreateSampler(SamplerType samplerType, int numSamples, int numSets)
{
	RayTracer::SamplerBase* sampler = nullptr;
	switch (samplerType)
	{
	case SamplerType::Hammersley:
		sampler = new HammersleySampler();
	case SamplerType::Random:
		sampler = new RandomSampler();
	case SamplerType::Regular:
		sampler = new RegularSampler();
	case SamplerType::Jittered:
		sampler = new JitteredSampler();
	default:
		sampler = new RegularSampler();
	}
	if (sampler)
		sampler->Init(numSamples, numSets);
	return sampler;
}
