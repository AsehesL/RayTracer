
cbuffer SkyBuffer
{
	float4 skyColor;
};

Texture2D skyTexture;
SamplerState skySampleType;

struct PixelInputType
{
    float4 position : SV_POSITION;
    float3 viewDir : TEXCOORD0;
};

float4 PixelFunc(PixelInputType input) : SV_TARGET
{
	float3 vdir = normalize(input.viewDir);

	//float2 uv = float2(atan2(vdir.z, vdir.x), asin(vdir.y));
    //uv *= float2(0.1591, 0.3183);
    //uv += 0.5;
    float2 uv;
    uv.x = atan2(vdir.x, vdir.z) * 0.5 * 0.3183;
    uv.y = 1.0 - acos(vdir.y) * 0.3183;

	return skyTexture.Sample(skySampleType, uv) * skyColor;
}
