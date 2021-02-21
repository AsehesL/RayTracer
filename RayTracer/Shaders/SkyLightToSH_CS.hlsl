
TextureCube<float4> _SkyLight;
SamplerState _SkyLightSamplerState;

RWStructuredBuffer<float3> _ResultSH;

float RadicalInverse_VdC(uint bits) 
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10;
}

float2 Hammersley(int i, uint N)
{
    return float2(float(i)/float(N), RadicalInverse_VdC(i));
} 

float3 SphereSample(float2 sp) 
{
	float cos_phi = cos(2.0 * 3.1415926535 * sp.x);
	float sin_phi = sin(2.0 * 3.1415926535 * sp.x);
	float cos_theta = sp.y * 2.0 - 1.0;
	float sin_theta = sqrt(1.0 - cos_theta * cos_theta);
	float pu = sin_theta * cos_phi;
	float pv = sin_theta * sin_phi;
	float pw = cos_theta;

	return float3(pu, pv, pw);
}

float R_Sphere0(float3 vertex) {
	return 0.5 * sqrt(0.31830988619);
}

float R_Sphere1_0(float3 vertex) {
	return 0.5 * sqrt(3 * 0.31830988619) * vertex.y;
}

float R_Sphere1_1(float3 vertex) {
	return 0.5 * sqrt(3 * 0.31830988619) * vertex.z;
}

float R_Sphere1_2(float3 vertex) {
	return 0.5 * sqrt(3 * 0.31830988619) * vertex.x;
}

float R_Sphere2_0(float3 vertex) {
	return 0.5 * sqrt(15 * 0.31830988619) * vertex.x * vertex.y;
}

float R_Sphere2_1(float3 vertex) {
	return 0.5 * sqrt(15 * 0.31830988619) * vertex.y * vertex.z;
}

float R_Sphere2_2(float3 vertex) {
	return 0.25 * sqrt(5 * 0.31830988619) * (2 * vertex.z * vertex.z - vertex.x * vertex.x - vertex.y * vertex.y);
}

float R_Sphere2_3(float3 vertex) {
	return 0.5 * sqrt(15 * 0.31830988619) * vertex.x * vertex.z;
}

float R_Sphere2_4(float3 vertex) {
	return 0.25 * sqrt(15 * 0.31830988619) * (vertex.x * vertex.x - vertex.y * vertex.y);
}

[numthreads(16,1,1)]
void CSMain (uint3 id : SV_DispatchThreadID)
{
	float3 sh = 0;

	float weight = 4.0 * 3.1415926535 / 1024.0;

	for (int i = 0; i < 1024; i++) 
	{
		float2 sp = Hammersley(i, 1024u);
		float3 dir = SphereSample(sp);
		float3 col = saturate(_SkyLight.SampleLevel(_SkyLightSamplerState, dir, 0).rgb);
		float s = 0;
		if (id.x == 0) {
			s = R_Sphere0(dir);
		}
		else if (id.x == 1) {
			s = R_Sphere1_0(dir);
		}
		else if (id.x == 2) {
			s = R_Sphere1_1(dir);
		}
		else if (id.x == 3) {
			s = R_Sphere1_2(dir);
		}
		else if (id.x == 4) {
			s = R_Sphere2_0(dir);
		}
		else if (id.x == 5) {
			s = R_Sphere2_1(dir);
		}
		else if (id.x == 6) {
			s = R_Sphere2_2(dir);
		}
		else if (id.x == 7) {
			s = R_Sphere2_3(dir);
		}
		else if (id.x == 8) {
			s = R_Sphere2_4(dir);
		}
		sh.r += weight * col.r * s;
		sh.g += weight * col.g * s;
		sh.b += weight * col.b * s;
	}

	_ResultSH[id.x] = sh;
}