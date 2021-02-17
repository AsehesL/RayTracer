cbuffer MatrixBuffer
{
	matrix screenMatrix;
};

struct VertexInputType
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv0 : TEXCOORD0;
    float4 tangent : TANGENT;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 uv0 : TEXCOORD0;
};

PixelInputType VertexFunc(VertexInputType input)
{
    PixelInputType output;

    float4 pos = float4(input.position.x, input.position.y, input.position.z, 1.0);
   	output.position = mul(pos, screenMatrix);
    output.uv0 = input.uv0;

    return output;
}
