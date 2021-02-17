cbuffer MatrixBuffer
{
    matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
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
    float3 viewDir : TEXCOORD0;
};

PixelInputType VertexFunc(VertexInputType input)
{
    PixelInputType output;

    float4 pos = float4(input.position.x, input.position.y, input.position.z, 1.0);
    output.viewDir = normalize(pos.xyz);
    pos.xyz *= 999;
    output.position = mul(pos, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	//output.normal = mul(float4(input.normal.x, input.normal.y, input.normal.z, 0.0), worldMatrix).xyz;

    //output.uv0 = input.uv0;

    return output;
}