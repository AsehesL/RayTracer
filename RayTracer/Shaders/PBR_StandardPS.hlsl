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

cbuffer PBRBuffer
{
	float4 _AlbedoColor;
	float4 _Roughness;
	float4 _Metallic;
	float4 _uvTile;
};

cbuffer SkyLightSHBuffer
{
	float4 skyLightDiffuseSH0R;
	float4 skyLightDiffuseSH0G;
	float4 skyLightDiffuseSH0B;
	float4 skyLightDiffuseSH1R;
	float4 skyLightDiffuseSH1G;
	float4 skyLightDiffuseSH1B;
	float4 skyLightDiffuseSH2;
	float4 skyLightDiffuseSH3;
};

cbuffer ShadowUniformBuffer
{
	matrix lightSpaceMatrix;
	matrix shadowProjectionMatrix;
	float4 shadowMapSize;
};

Texture2D albedoTexture;
SamplerState albedoSampleType;

Texture2D mroTexture;
SamplerState mroSampleType;

Texture2D bumpTexture;
SamplerState bumpSampleType;

TextureCube _skyLightCubeMap;
SamplerState _skyLightCubeMapSampleType;

Texture2D _shadowMap;
SamplerState _shadowMapSampleType;

Texture2D _integrateBRDFTexture;
SamplerState _integrateBRDFTextureSampleType;

struct PixelInputType
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float4 tangent : TANGENT;
    float2 uv0 : TEXCOORD0;
    float3 worldPos : TEXCOORD1;
};

float GeometrySchlickGGX(float ndv, float roughness)
{
	float r = (roughness + 1.0f);
	float k = (r * r) / 8.0f;
	
	float nom = ndv;
	float denom = ndv * (1.0f - k) + k;
	
	return nom / denom;
}

float GeometrySmith(float ndv, float ndl, float roughness)
{
	float ggx2 = GeometrySchlickGGX(ndv, roughness);
	float ggx1 = GeometrySchlickGGX(ndl, roughness);
    
	return ggx1 * ggx2;
}

float DistributionGGX(float3 normal, float3 hDir, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float ndh = max(dot(normal, hDir), 0.0f);
	float ndh2 = ndh * ndh;
    
	float nom = a2;
	float denom = (ndh2 * (a2 - 1.0f) + 1.0f);
	denom = 3.141592653 * denom * denom;
    
	return nom / denom;
}
            
float3 FresnelSchlickRoughness(float cosTheta, float3 f0, float roughness)
{
	return f0 + (max(1.0f - roughness, f0) - f0) * pow(1.0f - cosTheta, 5.0f);
}

float SampleShadow(float2 uv, float depth) {
	float sd = _shadowMap.Sample(_shadowMapSampleType, uv).r;
	if(sd < depth)
	{
		return 0.0;
	}
	else
	{
		return 1.0;
	}
}

float3 SampleShadow3(float2 uv, float offset, float depth) {
	float3 atten = float3(0.0, 0.0, 0.0);
	atten.x = SampleShadow((uv + float2(0.0,offset))*shadowMapSize.xy, depth);
	atten.y = SampleShadow((uv + float2(1.0,offset))*shadowMapSize.xy, depth);
	atten.z = SampleShadow((uv + float2(2.0,offset))*shadowMapSize.xy, depth);
	return atten;
}

float PCFShadow(float3 worldPos)
{
	float4 shadowPos = mul(float4(worldPos.xyz, 1.0), lightSpaceMatrix);
	shadowPos = mul(shadowPos, shadowProjectionMatrix);

	float2 shadowUV;
	shadowUV.xy = shadowPos.xy / shadowPos.w * 0.5 + 0.5;
	shadowUV.y = 1.0 - shadowUV.y;

	float shadowDepth = shadowPos.z / shadowPos.w;
	

	float2 texelPos = shadowUV * shadowMapSize.zw - float2(0.5, 0.5);
	float2 fraction = frac(texelPos);
	float2 texelCent = floor(texelPos) + float2(0.5, 0.5);

	float2 sampleTexelCent = texelCent - float2(1.0, 1.0);

	float3 sample0 = SampleShadow3(sampleTexelCent, 0.0, shadowDepth);
	float3 sample1 = SampleShadow3(sampleTexelCent, 1.0, shadowDepth);
	float3 sample2 = SampleShadow3(sampleTexelCent, 2.0, shadowDepth);
	
	float3 shadow = float3(0.0, 0.0, 0.0);

	shadow.x = sample0.x * (1.0f - fraction.x);
	shadow.y = sample1.x * (1.0f - fraction.x);
	shadow.z = sample2.x * (1.0f - fraction.x);
	shadow.x += sample0.y;
	shadow.y += sample1.y;
	shadow.z += sample2.y;
	shadow.x += sample0.z * fraction.x;
	shadow.y += sample1.z * fraction.x;
	shadow.z += sample2.z * fraction.x;

	return clamp(0.25f * dot(shadow, float3(1.0f - fraction.y, 1.0f, fraction.y)), 0.0f, 1.0f);
}

float GetMetallic(float4 mro)
{
	return _Metallic.r * mro.r;
}

float GetRoughness(float4 mro)
{
	return _Roughness.r * mro.g;
}

float3 GetAlbedo(float2 uv)
{
	return (albedoTexture.Sample(albedoSampleType, uv*_uvTile.xy) * _AlbedoColor).rgb;
}

float ComputeSkyLightMipLevel(float roughness, float maxMip)
{
	return (1.7*roughness-0.7*roughness*roughness)*maxMip;
}

float3 GetSkyLight(float3 reflDir, float roughness)
{
	float mipLevel = ComputeSkyLightMipLevel(roughness, 8);
	return _skyLightCubeMap.SampleLevel(_skyLightCubeMapSampleType, reflDir, mipLevel).rgb;
}

float3 GetDiffuseSkyLight(float3 normal)
{
	float4 dir = float4(normal.xyz, 1.0);

	float3 skydiffuse = 0;
				
	skydiffuse.r += dot(dir, skyLightDiffuseSH0R);
	skydiffuse.g += dot(dir, skyLightDiffuseSH0G);
	skydiffuse.b += dot(dir, skyLightDiffuseSH0B);

	float4 nd = dir.xyzz * dir.yzzx;

	skydiffuse.r += dot(nd, skyLightDiffuseSH1R);
	skydiffuse.g += dot(nd, skyLightDiffuseSH1G);
	skydiffuse.b += dot(nd, skyLightDiffuseSH1B);

	skydiffuse.rgb += dir.x * dir.x * skyLightDiffuseSH2.xyz;
	skydiffuse.rgb += dir.y * dir.y * skyLightDiffuseSH3.xyz;

	return skydiffuse;
}

float2 GetEnvBRDF(float ndv, float roughness)
{
	return _integrateBRDFTexture.Sample(_integrateBRDFTextureSampleType, float2(ndv, 1.0f - roughness)).rg;
}

float4 PixelFunc(PixelInputType input) : SV_TARGET
{
	float3 worldNormal = normalize(mul(float4(input.normal.xyz, 0.0), worldMatrix).xyz);
	float3 worldTangent = normalize(mul(float4(input.tangent.xyz, 0.0), worldMatrix).xyz);
	float3 worldBinormal = cross(worldNormal, worldTangent)*input.tangent.w;

	float3 tanNormal = bumpTexture.Sample(bumpSampleType, input.uv0).rgb * 2.0 - 1.0;
	worldNormal = float3(dot(float3(worldTangent.x, worldBinormal.x, worldNormal.x).xyz, tanNormal), dot(float3(worldTangent.y, worldBinormal.y, worldNormal.y).xyz, tanNormal), dot(float3(worldTangent.z, worldBinormal.z, worldNormal.z).xyz, tanNormal));

	float3 viewDir = normalize(cameraPosition.xyz - input.worldPos.xyz).xyz;
	float3 lightDir = normalize(sunlightDirection.xyz);

	float ndv = max(0, dot(viewDir, worldNormal));
	float ndl = max(0, dot(lightDir, worldNormal));
	float3 hdir = normalize(viewDir + lightDir);

	float3 refDir = reflect(viewDir, worldNormal);

 	float4 mro = mroTexture.Sample(mroSampleType, input.uv0);
	float roughness = GetRoughness(mro);
	float metallic = GetMetallic(mro);
	float3 albedo = GetAlbedo(input.uv0);
	float occlusion = mro.b;


	float3 F0 = lerp(float3(0.04, 0.04, 0.04), albedo.rgb, metallic);
	float3 F = FresnelSchlickRoughness(ndv, F0, roughness);
	float2 envbrdf = GetEnvBRDF(ndv, roughness);
	float3 skyspecular = F * envbrdf.x + float3(envbrdf.y, envbrdf.y, envbrdf.y);

	float3 ks = F;
    float3 kd = float3(1.0,1.0,1.0) - ks;
    kd *= 1.0 - metallic;


	float4 color = 0;

	float3 skylightDiffuse = GetDiffuseSkyLight(-worldNormal);
	color.rgb += skylightDiffuse * occlusion * albedo * kd / 3.141592653;

	float3 skylight = GetSkyLight(refDir, roughness);
	color.rgb += skylight * skyspecular;
	

	float NDF = DistributionGGX(worldNormal, hdir, roughness);
	float G = GeometrySmith(ndv, ndl, roughness);
                
    

    float3 nominator = NDF * G * F;
    float denominator = 4.0 * ndv * ndl + 0.001; 
    float3 specular = nominator / denominator;

    float shadow = PCFShadow(input.worldPos.xyz);

    color.rgb += (kd * albedo / 3.141592653 + specular) * sunlightColor.rgb * ndl * shadow;
    color.a = 1.0;

    //color.rgb=color.rgb*0.0001+lerp(skylight, skylightDiffuse, roughness);

	return color;
}
