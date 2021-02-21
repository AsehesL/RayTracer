cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer ViewBuffer
{
    float4 cameraPosition;
    float4 sunlightDirection;
    float4 sunlightColor;
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
    float3 normal : NORMAL;
    float4 tangent : TANGENT;
    float2 uv0 : TEXCOORD0;
    float3 worldPos : TEXCOORD1;
};

PixelInputType VertexFunc(VertexInputType input)
{
    PixelInputType output;

    float4 pos = float4(input.position.x, input.position.y, input.position.z, 1.0);
   	output.position = mul(pos, worldMatrix);
    output.worldPos = output.position.xyz;
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	output.normal = input.normal;
    output.tangent = input.tangent;

    output.uv0 = input.uv0;

    return output;
}
