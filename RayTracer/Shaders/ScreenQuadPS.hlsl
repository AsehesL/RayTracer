Texture2D shaderTexture;
SamplerState SampleType;

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 uv0 : TEXCOORD0;
};

float4 PixelFunc(PixelInputType input) : SV_TARGET
{
    return shaderTexture.Sample(SampleType, input.uv0);
}
