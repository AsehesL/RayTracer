Texture2D shaderTexture;
SamplerState SampleType;

cbuffer TextureSizeBuffer
{
	float4 textureSize;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 uv0 : TEXCOORD0;
};

float4 PixelFunc(PixelInputType input) : SV_TARGET
{
    float4 col = shaderTexture.Sample(SampleType, input.uv0);
    col += shaderTexture.Sample(SampleType, input.uv0 + float2(textureSize.x, 0.0));
    col += shaderTexture.Sample(SampleType, input.uv0 + float2(textureSize.x, textureSize.y));
    col += shaderTexture.Sample(SampleType, input.uv0 + float2(0.0, textureSize.y));

    col.rgb *= 0.25;
    col.a = 1.0;
    return col;
}
