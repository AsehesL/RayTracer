Texture2D shaderTexture;
SamplerState SampleType;

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 uv0 : TEXCOORD0;
};

float4 PixelFunc(PixelInputType input) : SV_TARGET
{
    float4 col = shaderTexture.Sample(SampleType, input.uv0);
    col.rgb = pow(col.rgb, 0.45); 
    return col;
}
