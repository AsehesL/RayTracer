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

cbuffer TransparencyBuffer
{
	float4 color;
};

Texture2D _screenCaptureTexture;
SamplerState _screenCaptureSampleType;

struct PixelInputType
{
    float4 position : SV_POSITION;
   	float4 worldPos : TEXCOORD0;
   	float3 worldNormal : NORMAL;
};

float2 GetScreenCaptureUV(float4 worldPos, float3 worldNormal)
{
	float2 uvoffset = mul(float4(normalize(worldNormal.xyz), 0.0), viewMatrix).xy;

	float4 proj = mul(worldPos, viewMatrix);
	proj = mul(proj, projectionMatrix);
	float2 projuv = (proj.xy / proj.w + uvoffset * 0.1) * 0.5 + 0.5;
	projuv.y = 1.0 - projuv.y;
	return projuv;
}

float4 PixelFunc(PixelInputType input) : SV_TARGET
{
	float2 screenCaptureUV = GetScreenCaptureUV(input.worldPos, input.worldNormal);

	return _screenCaptureTexture.Sample(_screenCaptureSampleType, screenCaptureUV) * float4(0.3,0.6,1,1);
}
