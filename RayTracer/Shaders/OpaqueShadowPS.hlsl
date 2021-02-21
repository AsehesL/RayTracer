cbuffer ShadowPassBuffer
{
	float4 shadowBias;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 shadowPos : TEXCOORD0;
};

float PixelFunc(PixelInputType input) : SV_TARGET
{
	float depth = input.shadowPos.z / input.shadowPos.w + shadowBias.x / input.shadowPos.w;
	return depth;
}
