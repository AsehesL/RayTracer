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

float RadicalInverse_VdC(uint bits) 
{
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10;
}

float2 Hammersley(int i, uint N)
{
    return float2(float(i)/float(N), RadicalInverse_VdC(i));
} 

float3 ImportanceSampleGGX(float2 Xi, float3 normal, float roughness)
{
        float a = roughness * roughness;

        float phi = 2.0f * 3.1415926f * Xi.x;
        float cosTheta = sqrt((1.0f - Xi.y) / (1.0f + (a * a - 1.0f) * Xi.y));
        float sinTheta = sqrt(1.0f - cosTheta * cosTheta);

        float3 hDir;
        hDir.x = cos(phi) * sinTheta;
        hDir.y = sin(phi) * sinTheta;
        hDir.z = cosTheta;

        float3 up = abs(normal.z) < 0.999f ? float3(0.0f, 0.0f, 1.0f) : float3(1.0f, 0.0f, 0.0f);
        float3 tangent = normalize(cross(up, normal));
        float3 bitangent = cross(normal, tangent);

        float3 sampleVec = tangent * hDir.x + bitangent * hDir.y + normal * hDir.z;
        return normalize(sampleVec);
}

float2 IntegrateBRDF(float ndv, float roughness)
{
    float3 viewDir;
    viewDir.x = sqrt(1.0f - ndv * ndv);
    viewDir.y = 0.0f;
    viewDir.z = ndv;

    float A = 0.0f; 
    float B = 0.0f;

    float3 normal = float3(0.0f, 0.0f, 1.0f);

    for (int i = 0; i < 1024; ++i)
    {
        float2 Xi = Hammersley(i, 1024u);

        float3 hDir = ImportanceSampleGGX(Xi, normal, roughness);
        float3 lightDir = normalize(2.0f * dot(viewDir, hDir) * hDir - viewDir);

        float ndl = max(lightDir.z, 0.0f);
        float ndh = max(hDir.z, 0.0f);
        float vdh = max(dot(viewDir, hDir), 0.0f);
        float ndv = max(viewDir.z, 0.0f);


            if (ndl > 0.0f)
            {
                float G = GeometrySmith(ndv, ndl, roughness);
                float G_Vis = (G * vdh) / (ndh * ndv);
                float Fc = pow(1.0f - vdh, 5.0f);

                A += (1.0f - Fc) * G_Vis;
                B += Fc * G_Vis;
            }
    }
    A /= 1024.0f;
    B /= 1024.0f;
    return float2(A, B);
}

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

float4 PixelFunc(PixelInputType input) : SV_TARGET
{
	float2 col = IntegrateBRDF(input.uv.x, input.uv.y);
    return float4(col.xy, 0.0, 1.0);
}
